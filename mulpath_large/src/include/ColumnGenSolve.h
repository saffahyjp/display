#ifndef COLUMNGENSOLVE_H
#define COLUMNGENSOLVE_H

#include "SolveStrategy.h"
#include "Tree.h"
#include "gurobi_c++.h"

class ColumnGenSolve: public SolveStrategy
{
public:
	const double M;
	ColumnGenSolve(Solver *solver_): SolveStrategy(solver_), M(solver_->board->width * solver_->board->height){}
	virtual ~ColumnGenSolve(){}
	virtual Solution solve() const;
	virtual SolveStrategy *clone() const
	{
		return new ColumnGenSolve(*this);
	}
protected:
	struct expandFinished{};

	static const int THREAD_CNT = 2;
	static Thread threads[THREAD_CNT];
	static void sync();
	struct parDijkstraParams
	{
	private:
		parDijkstraParams(const parDijkstraParams &);
		parDijkstraParams &operator =(const parDijkstraParams &);
	public:
		parDijkstraParams(){}
		const BitMatrix *baseStar;
		const Matrix<double> *mapWStar;
		int n; int m;
		Matrix<Pair<double, Point>> *ansStar;
		Mutex mutex;
		bool finished;
		bool exiting;
	};
	static parDijkstraParams paramsList[THREAD_CNT];
	static void parDijkstraAtExit();
	static void parDijkstra(parDijkstraParams &params);
	static void *parDijkstraInit();
	static const void *parDijkstraInitRes;
	
	Vector<Tree *> route(int tim) const;
	
	double solveLP(
		Vector<Vector<Pair<Tree, GRBVar>>> &treesets,
		int mode, Matrix<double> *mapPi = NULL,
		Vector<double> *vecLambda = NULL, int ignoreIdx = -1
	) const;
	
	/*void ColumnGenSolve::expand(
		const Matrix<double> &mapPi, double lambda,
		Vector<Pair<Tree, GRBVar>> &treeset, int n, int m, int idx, int &r
	) const;
	
	void ColumnGenSolve::expand(
		const Matrix<double> &mapPi, double lambda,
		Vector<Pair<Tree, GRBVar>> &treeset, const Tree &tree,
		int n, int m, int idx, int &r
	) const;
	
	void ColumnGenSolve::expand(
		const Matrix<double> &mapPi, double lambda,
		Vector<Pair<Tree, GRBVar>> &treeset, const BitMatrix &base,
		int n, int m, int idx, int &r
	) const;*/
	
	void dijkstra(
		const BitMatrix &base, const Matrix<double> &mapW, int n, int m,
		Matrix<Pair<double, Point>> &ans
	) const;

	bool suggestTree(
		const Vector<TerminalSet *> &termsets,
		Vector<Vector<Pair<Tree, GRBVar>>> &treesets,
		const Matrix<double> &mapW, int n, int m, int idx
	) const;

	Tree suggestTree(
		const TerminalSet *terminalSet, const Matrix<double> &mapW, int n, int m
	) const;
	
	void dfsEdge(
		BitMatrix &tree, const Matrix<int> &id, Vector<Point> &points,
		int x, int y, int n, int m, int &s, int &t
	) const;
	
	int dfsFather(Vector<int> &father, int x) const;

	void removeNonCuts(
		const Matrix<int> &map, int idx, BitMatrix &tree, int n, int m,
		const Matrix<double> *mapW = NULL
		//, int exx = -1, int exy = -1
	) const;

	double removeNonCuts(
		Matrix<bool> &visited, const Matrix<int> &map, int idx,
		const Matrix<double> &mapW, BitMatrix &tree, int n, int m
	) const;

	bool pushTreeSet(
		Vector<Pair<Tree, GRBVar>> &treeset, Tree &tree
	) const;
	
	int treeBranches(BitMatrix tree, int n, int m) const;
	void treeDFSBranches(BitMatrix &tree, int i, int j, int n, int m) const;
	Vector<BitMatrix> treeGetBranches(BitMatrix tree, int n, int m) const;
	void treeDFSGetBranches(
		BitMatrix &tree, BitMatrix &res, int i, int j, int n, int m
	) const;
	bool treeIsCut(BitMatrix tree, int i, int j, int n, int m) const;
};

#endif // COLUMNGENSOLVE_H
