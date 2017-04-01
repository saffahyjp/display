#include "include/treegenall.h"

Vector<Tree> TreeGenAll::gen() const{
	Vector<Tree> res;
	Tree tree(this->m_board, this->m_idx);
	this->DFSGen(
		res, tree, 0, 0,
		this->m_board.n(), this->m_board.m()
	);
	return res;
}

void TreeGenAll::DFSGen(
	Vector<Tree> &res, Tree &tree,
	int i, int j, int n, int m
) const{
	// Very simple brute-force DFS method
	if(i >= n){
		if(tree.valid())
			res.push_back(tree);
	}else if(j >= m)
		DFSGen(res, tree, i + 1, 0, n, m);
	else{
		int pos = this->m_board.map()[i][j];
		if(pos != -1){
			tree.m_tree[i][j] = 1;
			DFSGen(res, tree, i, j + 1, n, m);
		}
		if(pos != this->m_idx){
			tree.m_tree[i][j] = 0;
			DFSGen(res, tree, i, j + 1, n, m);
		}
	}
}
