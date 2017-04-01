#ifndef GRBFACTORY_H
#define GRBFACTORY_H

#include "gurobi_c++.h"

class GRBFactory
{
private:
	GRBFactory(){}
	static GRBEnv *grbEnv;
public:
	static GRBModel &createModel();
};

#endif // GRBFACTORY_H
