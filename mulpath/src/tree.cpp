#include "include/tree.h"

bool Tree::valid() const{
	int n = this->m_board.n();
	int m = this->m_board.m();
	// Not connected, invalid
	if(this->Branches(this->m_tree, n, m) != 1)
		return false;
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++){
		if(!this->m_tree[i][j])
			continue;
		int pos = this->m_board.map()[i][j];
		if(pos == this->m_idx)
			continue;
		// Have terminal of other set, invalid
		if(pos && pos != this->m_idx)
			return false;
		// Have non-cutpoint, invalid
		if(!this->IsCut(
			this->m_tree, i, j, n, m
		)) return false;
	}
	return true;
}

Tree &Tree::operator =(const Tree &rhs){
	this->m_tree = rhs.m_tree;
	this->m_grbvar = rhs.m_grbvar;
	return *this;
}

int Tree::Branches(
	Matrix<bool> tree, int n, int m
){
	// Simple method finding how many branches
	int ans = 0;
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
		if(tree[i][j]){
			DFSBranches(tree, i, j, n, m);
			++ans;
		}
	return ans;
}

Vector<Matrix<bool>> Tree::GetBranches(
	Matrix<bool> tree, int n, int m
){
	// Simple method finding every branch
	Vector<Matrix<bool>> ans;
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
		if(tree[i][j]){
			Matrix<bool> res;
			res.resize(n, m);
			DFSGetBranches(
				tree, res, i, j, n, m
			);
			ans.push_back(res);
		}
	return ans;
}

void Tree::DFSGetBranches(
	Matrix<bool> &tree, Matrix<bool> &res,
	int i, int j, int n, int m
){
	// Simple DFS
	if(
		i < 0 || i >= n || j < 0 || j >= m
		|| !tree[i][j]
	) return;
	tree[i][j] = 0; res[i][j] = 1;
	DFSGetBranches(tree, res, i - 1, j, n, m);
	DFSGetBranches(tree, res, i + 1, j, n, m);
	DFSGetBranches(tree, res, i, j - 1, n, m);
	DFSGetBranches(tree, res, i, j + 1, n, m);
}

bool Tree::IsCut(
	Matrix<bool> tree,
	int i, int j, int n, int m
){
	// A simple method judging a cut-grid
	int b0 = Branches(tree, n, m);
	tree[i][j] = 0;
	return Branches(tree, n, m) != b0;
}

void Tree::DFSBranches(
	Matrix<bool> &tree,
	int i, int j, int n, int m
){
	// Simple DFS
	if(
		i < 0 || i >= n || j < 0 || j >= m
		|| !tree[i][j]
	) return;
	tree[i][j] = 0;
	DFSBranches(tree, i - 1, j, n, m);
	DFSBranches(tree, i + 1, j, n, m);
	DFSBranches(tree, i, j - 1, n, m);
	DFSBranches(tree, i, j + 1, n, m);
}

int Tree::cost() const{
	int ans = 0;
	for(const auto &row: this->m_tree)
		for(auto col: row)
			ans += (int) col;
	return ans;
}

bool operator <(
	const Tree &lhs, const Tree &rhs
){
	return lhs.m_grbvar.get(GRB_DoubleAttr_X)
		> rhs.m_grbvar.get(GRB_DoubleAttr_X);
}
bool operator ==(
	const Tree &lhs, const Tree &rhs
){
	return lhs.m_tree == rhs.m_tree;
}

OStream &operator <<(
	OStream &ost, const Tree &tree
){
	ost << "Tree:\n";
	ost << tree.m_tree;
	return ost;
}
