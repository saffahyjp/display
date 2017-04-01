#include "include/treegenone.h"

Tree TreeGenOne::gen() const{
	Tree tree(this->m_board, this->m_idx);
	try{
		this->DFSGen(
			tree, 0, 0,
			this->m_board.n(), this->m_board.m()
		);
	}catch(TreeGenOneFinished){
		return tree;
	}
	/*
		This function is only for debug use,
		since the returned tree is not always
		valid. It is not good to return it, so
		we throw an error instead.
	*/
	cout << "TreeGenOne error\n";
	exit(1);
	// return tree;
}

void TreeGenOne::DFSGen(
	Tree &tree,
	int i, int j, int n, int m
) const{
	if(i >= n){
		if(tree.valid())
			throw TreeGenOneFinished();
	}else if(j >= m)
		DFSGen(tree, i + 1, 0, n, m);
	else{
		int pos = this->m_board.map()[i][j];
		tree.m_tree[i][j] = 1;
		DFSGen(tree, i, j + 1, n, m);
		if(pos != this->m_idx){
			tree.m_tree[i][j] = 0;
			DFSGen(tree, i, j + 1, n, m);
		}
	}
}

