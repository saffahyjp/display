#include "../include/Solver.h"

Solution Solver::run(){
	assert(solve != NULL);
	assert(check != NULL);
	assert(optimize != NULL);
	Solution solution = solve->solve();
	assert(check->check(solution));
	return optimize->optimize(solution);
}
