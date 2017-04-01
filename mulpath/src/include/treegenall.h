#ifndef _TREEGENALL_H
#define _TREEGENALL_H

#include "global.h"
#include "board.h"
#include "tree.h"

class TreeGenAll{
public:
	const Board &m_board;
	const Vector<Point> &m_termset;
	const int m_idx;
	TreeGenAll(const Board &board, int idx):
	m_board(board), m_idx(idx),
	m_termset(board.termsets()[idx - 1]){
	}
	virtual ~TreeGenAll(){
	}
	virtual Vector<Tree> gen() const;
protected:
	void DFSGen(
		Vector<Tree> &res, Tree &tree,
		int i, int j, int n, int m
	) const;
};

#endif // _TREEGENALL_H
