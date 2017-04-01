#ifndef SOLVER_H
#define SOLVER_H

#include "global.h"
#include "Solution.h"
#include "SolveStrategy.h"
#include "Board.h"

class Solver
{
public:
	const Board *board;
	Solver(const Board *board_)
	{
		board = board_;
		solve = NULL;
		check = NULL;
		optimize = NULL;
	}
	Solver *clone() const
	{
		Solver *ans = new Solver(board);
		if(solve)
		{
			ans->solve = solve->clone();
			ans->solve->solver = ans;
		}
		else
			ans->solve = NULL;
		if(check)
		{
			ans->check = check->clone();
			ans->check->solver = ans;
		}
		else
			ans->check = NULL;
		if(optimize)
		{
			ans->optimize = optimize->clone();
			ans->optimize->solver = ans;
		}
		else
			ans->optimize = NULL;
		return ans;
	}
	~Solver()
	{
		delete solve;
		delete check;
		delete optimize;
	}
	Solution run();
	void setSolveStrategy(SolveStrategy *solve_)
	{
		if(solve)
			delete solve;
		solve = solve_;
		solve->solver = this;
	}
	void setCheckStrategy(CheckStrategy *check_)
	{
		if(check)
			delete check;
		check = check_;
		check->solver = this;
	}
	void setOptimizeStrategy(OptimizeStrategy *optimize_)
	{
		if(optimize)
			delete optimize;
		optimize = optimize_;
		optimize->solver = this;
	}
private:
	SolveStrategy *solve;
	CheckStrategy *check;
	OptimizeStrategy *optimize;
};

#endif