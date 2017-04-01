#include "../include/GRBFactory.h"

GRBEnv *GRBFactory::grbEnv = NULL;

GRBModel &GRBFactory::createModel()
{
	if(grbEnv == NULL)
	{
		grbEnv = new GRBEnv;
		grbEnv->set(GRB_IntParam_Threads, 1);
		grbEnv->set(GRB_IntParam_LogToConsole, 0);
		grbEnv->set(GRB_StringParam_LogFile, "Gurobi.log");
	}
	static GRBModel *lastModel = NULL;
	if(lastModel != NULL)
		delete lastModel;
	return *(lastModel = new GRBModel(*grbEnv));
}
