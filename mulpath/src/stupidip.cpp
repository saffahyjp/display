#include "include/stupidip.h"
#include "include/treegenall.h"

Vector<Tree *> StupidIP::route(int) const{
	// Generate full tree set
	Vector<Vector<Tree>> treesets;
	const Vector<Vector<Point>> &termsets
		= this->m_board.termsets();
	int t = termsets.size();
	for(int i = 1; i <= t; i++){
		TreeGenAll *gen = new TreeGenAll(
			this->m_board, i
		);
		treesets.push_back(gen->gen());
		delete gen;
	}
	
	// Initialize Gurobi
	
	GRBEnv env;
    GRBModel model(env);
	
	// Create variables
	
	for(auto &treeset: treesets)
	for(auto &tree: treeset)
		tree.m_grbvar = model.addVar(
			0, 1, 0, GRB_BINARY
		);
	Vector<GRBVar> varCanRoute;
	for(int i = 0; i < t; i++)
		varCanRoute.push_back(model.addVar(
			0, 1, 0, GRB_BINARY
		));
    model.update();
	
	// Create target function
	
	GRBLinExpr target;
	for(int i = 0; i < t; i++){
		target += 10000 * varCanRoute[i];
		for(const auto &tree: treesets[i])
			target += tree.cost()
				* tree.m_grbvar;
	}
    model.setObjective(target, GRB_MINIMIZE);
	
	// Create constraints
	
	for(int i = 0; i < t; i++){
		GRBLinExpr consLeft;
		consLeft += varCanRoute[i];
		for(const auto &tree: treesets[i])
			consLeft += tree.m_grbvar;
		GRBConstr unused = model.addConstr(
			consLeft == 1
		);
	}
	
	int n = this->m_board.n();
	int m = this->m_board.m();
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++){
		GRBLinExpr consLeft;
		for(const auto &treeset: treesets)
		for(const auto &tree: treeset)
			if(tree.m_tree[i][j])
				consLeft += tree.m_grbvar;
		GRBConstr unused = model.addConstr(
			consLeft <= 1
		);
	}
	
	// Optimize!

    model.optimize();
	
	// Check each tree if chosen

    cout << "Obj: "
		<< model.get(GRB_DoubleAttr_ObjVal)
		<< "\n";
	
	Matrix<int> map = this->m_board.map();
	for(const auto &treeset: treesets)
	for(const auto &tree: treeset)
		if(tree.m_grbvar.get(GRB_DoubleAttr_X)
			> 0.5)
			for(int i = 0; i < n; i++)
			for(int j = 0; j < m; j++)
				if(tree.m_tree[i][j])
					map[i][j] = tree.m_idx;
	
	cout << map;
	
	// Just for debug use, return nothing
	// (not implemented)
	
	return Vector<Tree *>();
}
