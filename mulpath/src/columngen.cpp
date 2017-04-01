#include "include/columngen.h"
#include "include/treegenall.h"
#include "include/treegenone.h"

double ColumnGen::solveLP(
	Vector<Vector<Tree>> &treesets,
	int mode,
	Matrix<double> &mapPi,
	Vector<double> &vecLambda,
	int ignoreIdx
) const{
	int t = treesets.size();
	int n = this->m_board.n();
	int m = this->m_board.m();
	
	/*
		Create a model. It is not created on
		stack because the caller also need to
		access some information of it.
	*/
	
	static GRBEnv env;
	env.set(GRB_IntParam_Threads, 1);
	env.set(GRB_IntParam_LogToConsole, 0);
	env.set(GRB_StringParam_LogFile, "Gurobi.log");
	static GRBModel *modelStar = NULL;
	if(modelStar)
		delete modelStar;
	modelStar = new GRBModel(env);
	GRBModel &model = *modelStar;
	
	// Create variables for trees
	for(int i = 0; i < t; i++)
	if(i != ignoreIdx - 1)
		for(auto &tree: treesets[i])
			tree.m_grbvar = model.addVar(
				0, 1, 0,
				mode ? GRB_BINARY
					: GRB_CONTINUOUS
			);
	
	// Create variables for sets
	Vector<GRBVar> varCanRoute;
	for(int i = 0; i < t; i++)
		varCanRoute.push_back(model.addVar(
			0, 1, 0,
			mode ? GRB_BINARY : GRB_CONTINUOUS
		));
	model.update();
	
	// Create target
	GRBLinExpr target;
	for(int i = 0; i < t; i++){
		target += 10000 * varCanRoute[i];
		if(i != ignoreIdx - 1)
		for(const auto &tree: treesets[i])
			target += tree.cost()
				* tree.m_grbvar;
	}
	model.setObjective(target, GRB_MINIMIZE);
	
	// Create constraints for sets
	Vector<GRBConstr> constrCanRoute;
	for(int i = 0; i < t; i++){
		GRBLinExpr consLeft;
		consLeft += varCanRoute[i];
		if(i != ignoreIdx - 1)
		for(const auto &tree: treesets[i])
			consLeft += tree.m_grbvar;
		constrCanRoute.push_back(
			model.addConstr(consLeft == 1)
		);
	}
	
	// Create constraints for grids
	Matrix<GRBConstr> constrNode;
	constrNode.resize(n, m);
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++){
		GRBLinExpr consLeft;
		for(int idx = 0; idx < t; idx++)
		if(idx != ignoreIdx - 1)
			for(const auto &tree: treesets[idx])
			if(tree.m_tree[i][j])
				consLeft += tree.m_grbvar;
		if(this->m_board.map()[i][j] == -1)
			constrNode[i][j] =
				model.addConstr(consLeft == 0);
		else
			constrNode[i][j] =
				model.addConstr(consLeft <= 1);
	}
	
	// Optimize!
	model.optimize();
	
	double ans =
		model.get(GRB_DoubleAttr_ObjVal);
	
	// If mode is binary, dual values are not
	// calculated, we only return the answer.
	if(mode)
		return ans;
	
	// Get dual values of each grid
	mapPi.resize(n, m);
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
		mapPi[i][j] = constrNode[i][j].get(
			GRB_DoubleAttr_Pi
		);
	
	// Get dual values of each set
	vecLambda.clear();
	for(const auto &cons: constrCanRoute)
		vecLambda.push_back(
			cons.get(GRB_DoubleAttr_Pi)
		);
	
	return ans;
}

Vector<Tree *> ColumnGen::route(int tim) const{
	// Open a console for output
	OFStream con("con");
	auto clkStart = clock();
	
	// Constant definition
	const Vector<Vector<Point>> &termsets
		= this->m_board.termsets();
	const Matrix<int> &map
		= this->m_board.map();
	const int t = termsets.size();
	const int n = this->m_board.n();
	const int m = this->m_board.m();
	
	// Unused maps
	Matrix<double> fakeMapPi;
	Vector<double> fakeVecLambda;
	
	// Two useful Vectors
	Vector<Point> allPoints;
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
		allPoints.push_back(Point(i, j));
	
	Matrix<double> all1;
	all1.resize(n, m);
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
		all1[i][j] = 1;
	
	// The tree set, initially empty
	Vector<Vector<Tree>> treesets;
	
	for(int idx = 1; idx <= t; idx++){
		// Current tree set
		Vector<Tree> vec;
		// If "Precise", all points can be joint
		if(this->m_mode == "Precise")
		for(auto joint: allPoints){
			// Call Dijkstra
			Matrix<bool> base;
			base.resize(n, m);
			base[joint.first][joint.second] = 1;
			Matrix<Pair<double, Matrix<bool>>>
				dijRes = this->dijkstra(
					base, all1, n, m, idx
				);
			// Generate a tree with Dijkstra
			// result
			Tree tree(this->m_board, idx);
			tree.m_tree[joint.first]
				[joint.second] = 1;
			for(auto term: termsets[idx - 1]){
				tree.m_tree[term.first]
					[term.second] = 1;
				for(int i = 0; i < n; i++)
				for(int j = 0; j < m; j++)
				if(dijRes[term].second[i][j])
					tree.m_tree[i][j] = 1;
			}
			this->RemoveNonCuts(
				map, idx, tree.m_tree,
				n, m, joint.first, joint.second
			);
			// Add the tree to the set
			vec.push_back(tree);
		}
		// Add the set to the set list
		treesets.push_back(vec);
	}
	
	// Answer history
	Vector<double> tarAns;
	for(int T = 0;; T++){
		// Solve the integer (binary)
		// programming problem
		this->solveLP(
			treesets, 1,
			fakeMapPi, fakeVecLambda
		);
		
		// Construct current best answer
		Vector<Tree *> ans;
		Matrix<int> map = this->m_board.map();
		for(const auto &treeset: treesets){
			for(const auto &tree: treeset)
			if(
				tree.m_grbvar.
					get(GRB_DoubleAttr_X) > 0.5
			){
				for(int i = 0; i < n; i++)
				for(int j = 0; j < m; j++)
				if(tree.m_tree[i][j])
					map[i][j] = tree.m_idx;
				
				ans.push_back(new Tree(tree));
				goto found;
			}
			ans.push_back(NULL);
			found:;
		}
		this->output(ans, con);
		this->output(ans, cout);
		
		bool updated = false;
		// Build weight map for unrouted set
		Matrix<double> mapObs;
		mapObs.resize(n, m);
		for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
		if(this->m_board.map()[i][j] == -1)
			mapObs[i][j] = 10000;
		else
			mapObs[i][j] = 1;
		
		for(int idx = 1; idx <= t; idx++)
		if(ans[idx - 1])
			for(int i = 0; i < n; i++)
			for(int j = 0; j < m; j++)
			if(ans[idx - 1]->m_tree[i][j])
				mapObs[i][j] = 10000;
		// Try to generate from each set
		for(int idx = 1; idx <= t; idx++)
		if(!ans[idx - 1]){
			// If unrouted, try to generate
			// from the map above
			if(this->suggestTree(
				termsets, treesets, mapObs,
				n, m, idx
			)) updated = true;
		}else{
			// If routed
			{
			// Try to reroute current set
			Matrix<double> mapObs2 = mapObs;
			for(int i = 0; i < n; i++)
			for(int j = 0; j < m; j++)
			if(
				mapObs2[i][j] >= 5000 &&
				ans[idx - 1]->m_tree[i][j]
			) mapObs2[i][j] = 1;
			
			if(this->suggestTree(
				termsets, treesets,
				mapObs2, n, m, idx
			)) updated = true;
			}
			{
			/*
				Try to reroute current set and
				try to route another tree
				rather than current tree
			*/
			Matrix<double> mapObs2 = mapObs;
			for(int i = 0; i < n; i++)
			for(int j = 0; j < m; j++)
			if(
				mapObs2[i][j] >= 5000 &&
				!ans[idx - 1]->m_tree[i][j]
			) mapObs2[i][j] = 1e8;
			if(this->suggestTree(
				termsets, treesets, mapObs2,
				n, m, idx
			)) updated = true;
			}
			{
			/*
				Try to solve integer programming
				without current set and try to
				avoid overlap between current
				tree and the solution above
			*/
			this->solveLP(
				treesets, 1,
				fakeMapPi, fakeVecLambda, idx
			);
			
			Vector<Tree *> tmpAns;
			
			for(int i = 0; i < t; i++)
			if(i != idx - 1){
				for(const auto &tree:
					treesets[i])
				if(tree.m_grbvar.get(
					GRB_DoubleAttr_X
				) > 0.5){
					tmpAns.push_back(
						new Tree(tree)
					);
					goto found2;
				}
				tmpAns.push_back(NULL);
				found2:;
			}else
				tmpAns.push_back(NULL);
			
			// OK, let's construct weight map
			Matrix<double> mapObs2;
			mapObs2.resize(n, m);
			for(int i = 0; i < n; i++)
			for(int j = 0; j < m; j++)
			if(map[i][j] == -1)
				mapObs2[i][j] = 1e8;
			else
				mapObs2[i][j] = 1;
			
			for(int nidx = 1; nidx <= t; nidx++)
			if(tmpAns[nidx - 1])
				for(int i = 0; i < n; i++)
				for(int j = 0; j < m; j++)
				if(
					tmpAns[nidx - 1]
						->m_tree[i][j]
				) mapObs2[i][j] = 10000;
			
			if(this->suggestTree(
				termsets, treesets, mapObs2,
				n, m, idx
			)) updated = true;
			
			// clean up
			for(auto tree: tmpAns)
				delete tree;
			}
		}
		
		auto clkNow = clock();
		if(
			!updated &&
			(int) ((clkNow - clkStart)
				/ CLOCKS_PER_SEC) > tim
		){
			con << "time up, aborting\n";
			con <<
				(int) ((clkNow - clkStart)
					/ CLOCKS_PER_SEC)
				<< " seconds passed\n";
			return ans;
		}
		
		// If solution didn't improve recently,
		// cut!
		if(
			!updated && T >= 21 &&
			tarAns[T - 21] == tarAns[T - 1]
		) return ans;
		
		// Output current colution info
		cout << "iteration " << T << "\n";
		con << "current time: "
			<< (int) ((clkNow - clkStart)
				/ CLOCKS_PER_SEC)
			<< " seconds\n";
		con << "iteration " << T
			<< "\ncolumn sizes: ";
		for(const auto &treeset: treesets){
			cout << "size "
				<< treeset.size() << "\n";
			con << treeset.size() << " ";
		}
		con << "\n";
		con.flush();
		fflush(stdout);
		
		// Solve LP
		Matrix<double> mapPi;
		Vector<double> vecLambda;
		
		tarAns.push_back(this->solveLP(
			treesets, 0, mapPi, vecLambda
		));
		
		// Only try to expand when Dijkstra
		// cannot find any solution
		if(!updated && this->m_mode != "Fast")
		for(int i = 0; i < t; i++){
			con << "Generating "
				<< i + 1 << "\n";
			con.flush();
			int oldSize = treesets[i].size();
			int r = 10000;
			sort(
				treesets[i].begin(),
				treesets[i].end()
			);
			try{
				this->expand(
					mapPi, vecLambda[i],
					treesets[i], n, m, i + 1, r
				);
			}catch(expandFinished){
			}
			if(treesets[i].size() != oldSize)
				updated = true;
		}
		
		// Still cannot generate, cut
		if(!updated)
			return ans;
		
		// Clean up
		for(auto tree: ans)
			delete tree;
	}
	
}

double ColumnGen::CalcTreeLambda(
	const Matrix<double> &mapPi,
	const Tree &tree, int n, int m
){
	double ans = 0;
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
		if(tree.m_tree[i][j])
			ans += 1 - mapPi[i][j];
	return ans;
}

// no real return value, for extension
bool ColumnGen::expand(
	const Matrix<double> &mapPi, double lambda,
	Vector<Tree> &treeset,
	int n, int m, int idx, int &r
) const{
	for(
		int i = 0;
		i < (int) treeset.size();
		i++
	) this->expand(
		mapPi, lambda,
		treeset, treeset[i], n, m, idx, r
	);
	return true;
}

// no real return value, for extension
bool ColumnGen::expand(
	const Matrix<double> &mapPi, double lambda,
	Vector<Tree> &treeset, const Tree &tree,
	int n, int m, int idx, int &r
) const{
	const Matrix<int> &map
		= this->m_board.map();
	
	Matrix<bool> visited, fakeVisited;
	visited.resize(n, m);
	fakeVisited.resize(n, m);
	Matrix<double> mapW;
	mapW.resize(n, m);
	
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
		mapW[i][j] = 1 - mapPi[i][j];
	
	// base with weight
	Vector<Pair<double, Matrix<bool>>> wbases;
	
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
	if(tree.m_tree[i][j] && !visited[i][j]){
		visited[i][j] = 1;
		int pos = map[i][j];
		// can't remove a terminal
		if(pos == idx)
			continue;
		// calculate current degree
		int deg = 0;
		if(i > 0 && tree.m_tree[i - 1][j])
			++deg;
		if(i + 1 < n && tree.m_tree[i + 1][j])
			++deg;
		if(j > 0 && tree.m_tree[i][j - 1])
			++deg;
		if(j + 1 < m && tree.m_tree[i][j + 1])
			++deg;
		// remove current grid
		Matrix<bool> newTree = tree.m_tree;
		newTree[i][j] = 0;
		double cw = mapW[i][j];
		// remove non-cut grids
		if(deg == 2)
			cw += this->RemoveNonCuts(
				visited, map,
				idx, mapW, newTree, n, m
			);
		else
			cw += this->RemoveNonCuts(
				fakeVisited, map,
				idx, mapW, newTree, n, m
			);
		wbases.push_back(
			Pair<double, Matrix<bool>>(
				cw, newTree
			)
		);
	}
	
	sort(wbases.begin(), wbases.end());
	reverse(wbases.begin(), wbases.end());
	
	// for each remove solution, try it
	for(const auto &wbase: wbases)
		this->expand(
			mapW, lambda, treeset, wbase.second,
			n, m, idx, r
		);
	return true;
}

bool ColumnGen::expand(
	const Matrix<double> &mapW,
	double lambda,
	Vector<Tree> &treeset,
	const Matrix<bool> &base,
	int n, int m, int idx, int &r
) const{
	// split the branch
	Vector<Matrix<bool>> branches
		= Tree::GetBranches(base, n, m);
	int nb = branches.size();
	// Dijkstra from each branch
	Vector<Matrix<Pair<double, Matrix<bool>>>>
		dijMatrices;
	for(const auto &branch: branches)
		dijMatrices.push_back(this->dijkstra(
			branch, mapW, n, m, idx
		));
	double bestw = 1e40;
	Tree btree(this->m_board, idx);
	// enumerate the joint
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++){
		double cw = 0;
		// construct the tree
		Tree tree(this->m_board, idx);
		tree.m_tree = base;
		for(const auto &dijMatrix: dijMatrices)
		for(int i1 = 0; i1 < n; i1++)
		for(int j1 = 0; j1 < m; j1++){
			if(dijMatrix[i][j].second.empty())
				goto fail;
			if(dijMatrix[i][j].second[i1][j1])
				tree.m_tree[i1][j1] = 1;
		}
		tree.m_tree[i][j] = 1;
		this->RemoveNonCuts(
			this->m_board.map(),
			idx, tree.m_tree, n, m
		);
		for(int i1 = 0; i1 < n; i1++)
		for(int j1 = 0; j1 < m; j1++)
			if(tree.m_tree[i1][j1])
				cw += mapW[i1][j1];
		if(cw < bestw){
			bestw = cw; btree = tree;
		}
		fail:;
	}
	// if the tree is sufficiently good, add it!
	if(bestw < lambda){
		if(std::find(
			treeset.begin(),
			treeset.end(), btree
		) == treeset.end()){
			treeset.push_back(btree);
			// to avoid infinite loop
			if((--r) <= 0)
				throw expandFinished();
		}
	}
	return false;
}

Matrix<Pair<double, Matrix<bool>>>
ColumnGen::dijkstra(
	const Matrix<bool> &base,
	const Matrix<double> &mapW,
	int n, int m, int idx
) const{
	// simple Dijkstra algorithm
	Matrix<Pair<double, Matrix<bool>>> dist;
	dist.resize(n, m);
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
		if(base[i][j])
			dist[i][j].second = base;
		else
			dist[i][j].first = 1e30;
	Matrix<bool> visited;
	visited.resize(n, m);
	int T = n * m;
	int dx[4] = {1, -1, 0, 0};
	int dy[4] = {0, 0, 1, -1};
	while(T--){
		double mdist = 1e31; int cx, cy;
		for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
			if(!visited[i][j]
				&& dist[i][j].first < mdist){
				mdist = dist[i][j].first;
				cx = i; cy = j;
			}
		visited[cx][cy] = 1;
		const Pair<double, Matrix<bool>> &cd
			= dist[cx][cy];
		for(int d = 0; d < 4; d++){
			int tx = cx + dx[d];
			int ty = cy + dy[d];
			if(tx < 0 || tx >= n
				|| ty < 0 || ty >= m
			) continue;
			Pair<double, Matrix<bool>> td = cd;
			td.first += mapW[tx][ty];
			td.second[tx][ty] = 1;
			dist[tx][ty] = std::min(
				dist[tx][ty], td
			);
		}
	}
	Matrix<Pair<double, Matrix<bool>>> ans
		= dist;
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++){
		for(int d = 0; d < 4; d++){
			int tx = i + dx[d], ty = j + dy[d];
			if(tx < 0 || tx >= n
				|| ty < 0 || ty >= m
			) continue;
			ans[i][j] = min(
				ans[i][j], dist[tx][ty]
			);
		}
		ans[i][j].second[i][j] = 1;
	}
	return ans;
}

bool ColumnGen::suggestTree(
	const Vector<Vector<Point>> &termsets,
	Vector<Vector<Tree>> &treesets,
	const Matrix<double> &mapW,
	int n, int m, int idx
) const{
	auto &treeset = treesets[idx - 1];
	Tree tree = this->suggestTree(
		termsets[idx - 1], mapW, n, m, idx
	);
	// The tree may have some useless grids
	this->RemoveNonCuts(
		this->m_board.map(), idx,
		tree.m_tree, n, m
	);
	if(std::find(
		treeset.begin(), treeset.end(),
		tree) == treeset.end()
	){
		treeset.push_back(tree);
		return true;
	}
	return false;
}

Tree ColumnGen::suggestTree(
	const Vector<Point> &termset,
	const Matrix<double> &mapW,
	int n, int m, int idx
) const{
	// similar but different
	// to the expand process
	Matrix<bool> base;
	base.resize(n, m);
	Vector<Matrix<Pair<double, Matrix<bool>>>>
		dijMatrices;
	for(const auto &term: termset){
		Matrix<bool> branch;
		branch.resize(n, m);
		branch[term.first][term.second] = 1;
		base[term.first][term.second] = 1;
		dijMatrices.push_back(this->dijkstra(
			branch, mapW, n, m, idx
		));
	}
	double bestw = 1e40;
	Tree btree(this->m_board, idx);
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++){
		double cw = 0;
		Tree tree(this->m_board, idx);
		tree.m_tree = base;
		for(const auto &dijMatrix: dijMatrices)
		for(int i1 = 0; i1 < n; i1++)
		for(int j1 = 0; j1 < m; j1++){
			if(dijMatrix[i][j].second.empty())
				goto fail;
			if(dijMatrix[i][j].second[i1][j1])
				tree.m_tree[i1][j1] = 1;
		}
		tree.m_tree[i][j] = 1;
		for(int i1 = 0; i1 < n; i1++)
		for(int j1 = 0; j1 < m; j1++)
			if(tree.m_tree[i1][j1])
				cw += mapW[i1][j1];
		if(cw < bestw){
			bestw = cw; btree = tree;
		}
		fail:;
	}
	return btree;
}

void ColumnGen::RemoveNonCuts(
	const Matrix<int> &map, int idx,
	Matrix<bool> &tree, int n, int m,
	int exx, int exy
){
	begin:
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
	if(
		tree[i][j] && map[i][j] != idx
		&& (i != exx || j != exy) &&
		!Tree::IsCut(tree, i, j, n, m)
	){
		tree[i][j] = 0;
		goto begin;
	}
}

double ColumnGen::RemoveNonCuts(
	Matrix<bool> &visited,
	const Matrix<int> &map, int idx,
	const Matrix<double> &mapW,
	Matrix<bool> &tree, int n, int m
){
	double ans = 0;
	begin:
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
	if(
		tree[i][j] && map[i][j] != idx &&
		!Tree::IsCut(tree, i, j, n, m)
	){
		visited[i][j] = 1;
		tree[i][j] = 0;
		ans += mapW[i][j];
		goto begin;
	}
	return ans;
}
