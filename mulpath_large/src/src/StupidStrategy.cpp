#include "../include/StupidStrategy.h"

Solution StupidSolve::solve() const
{
	Solution solution;
	solution.board = solver->board;
	solution.trees.resize(solver->board->terminalSets.size(), NULL);
	solution.map = solver->board->map;
	for(auto &col: solution.map)
		for(auto &row: col)
			if(row > 0)
				row = 0;
	return solution;
}
