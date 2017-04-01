#ifndef DACSOLVE_H
#define DACSOLVE_H

#include "SolveStrategy.h"

class DACSolve: public SolveStrategy
{
protected:
	void divide(Matrix<int> &map, int x1, int x2, int y1, int y2) const;
public:
	SolveStrategy *subSolve;
	DACSolve(Solver *solver_, SolveStrategy *subsolve_)
		: SolveStrategy(solver_), subSolve(subsolve_){}
	virtual ~DACSolve()
	{
		if(subSolve)
			delete subSolve;
	}
	virtual Solution solve() const;
	DACSolve *clone() const
	{
		DACSolve *ans = new DACSolve(solver, subSolve);
		if(subSolve)
		{
			ans->subSolve = subSolve->clone();
		}
		else
			ans->subSolve = NULL;
		return ans;
	}
};

#endif // DACSOLVE_H
