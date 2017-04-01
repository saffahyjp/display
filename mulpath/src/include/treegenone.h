#ifndef _TREEGENONE_H
#define _TREEGENONE_H

#include "global.h"
#include "board.h"
#include "tree.h"

class TreeGenOne{
public:
	const Board &m_board;
	const Vector<Point> &m_termset;
	const int m_idx;
	TreeGenOne(const Board &board, int idx):
	m_board(board), m_idx(idx),
	m_termset(board.termsets()[idx - 1]){
	}
	virtual ~TreeGenOne(){
	}
	virtual Tree gen() const;
protected:
	struct TreeGenOneFinished{
	};
	void DFSGen(
		Tree &tree,
		int i, int j, int n, int m
	) const;
};

#endif // _TREEGENONE_H
