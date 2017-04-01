#ifndef _TREE_H
#define _TREE_H

#include "global.h"
#include "board.h"

class Tree{
public:
	const Board &m_board;
	const Vector<Point> &m_termset;
	Matrix<bool> m_tree;
	const int m_idx;
	GRBVar m_grbvar;
	Tree(const Board &board, int idx):
	m_board(board), m_idx(idx),
	m_termset(board.termsets()[idx - 1]){
		this->m_tree.resize(
			board.n(), board.m()
		);
	}
	bool valid() const;
	int cost() const;
	Tree &operator =(const Tree &rhs);
	static int Branches(
		Matrix<bool> tree, int n, int m
	);
	static Vector<Matrix<bool>> GetBranches(
		Matrix<bool> tree, int n, int m
	);
	static bool IsCut(
		Matrix<bool> tree,
		int i, int j, int n, int m
	);
protected:
	static void DFSBranches(
		Matrix<bool> &tree,
		int i, int j, int n, int m
	);
	static void DFSGetBranches(
		Matrix<bool> &tree, Matrix<bool> &res,
		int i, int j, int n, int m
	);
};

bool operator <(
	const Tree &lhs, const Tree &rhs
);
bool operator ==(
	const Tree &lhs, const Tree &rhs
);

OStream &operator <<(
	OStream &ost, const Tree &tree
);

#endif // _TREE_H
