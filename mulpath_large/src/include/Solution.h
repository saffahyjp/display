#ifndef SOLUTION_H
#define SOLUTION_H

#include "Board.h"
#include "Tree.h"
#include "global.h"

class Solution
{
private:
public:
	Solution(const Solution&);
	Solution(){}
	~Solution();
	const Board *board;
	Vector<Tree*> trees;
	// trees.size() == board.terminalSets.size() == real number + 1
	Matrix<int> map;
	friend OStream &operator <<(OStream &ost, const Solution &solution);
	void computeMap();
};

#endif