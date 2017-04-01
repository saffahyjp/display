#ifndef SOLVESTRATEGY_H
#define SOLVESTRATEGY_H

#include "global.h"
#include "Solution.h"
class Solver;
// #include "Solver.h"

class SolveStrategy
{
protected:
public:
	Solver *solver;
	SolveStrategy(Solver *solver_)
	{
		solver = solver_;
	}
	virtual ~SolveStrategy(){}
	virtual Solution solve() const = 0;
	virtual SolveStrategy *clone() const = 0;
};

class CheckStrategy
{
protected:
public:
	Solver *solver;
	CheckStrategy(Solver *solver_)
	{
		solver = solver_;
	}
	virtual ~CheckStrategy(){}
	virtual bool check(const Solution &solution) const = 0;
	virtual CheckStrategy *clone() const = 0;
};

class OptimizeStrategy
{
protected:
public:
	Solver *solver;
	OptimizeStrategy(Solver *solver_)
	{
		solver = solver_;
	}
	virtual ~OptimizeStrategy(){}
	virtual Solution optimize(const Solution &solution) const = 0;
	virtual OptimizeStrategy *clone() const = 0;
};

#endif

#include "Solver.h"
