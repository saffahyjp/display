#include "../include/test.h"

#define TEST(testname)                      \
	class testname: public Test             \
	{                                       \
	private:                                \
		static Test *instance;              \
	protected:                              \
		testname(): Test(#testname){}       \
		virtual void run() const;           \
	};                                      \
	Test *testname::instance = new testname;\
	void testname::run() const

#include "../include/Solution.h"
#include "../include/CleverOptimize.h"
	
// TEST(CleverOptimizeTest)
void _233()
{
	String ins =
		"5 6 \n"
		"1 0 0 0 0 0 \n"
		"0 0 0 0 0 0 \n"
		"1 0 0 0 2 1 \n"
		"2 0 0 0 0 0 \n"
		"0 0 0 0 0 0 \n"
	;
	StringStream inf(ins);
	Board board;
	board.input(0, inf);
	Solution solution;
	solution.board = &board;
	TerminalSet &terminalSet1 = *board.terminalSets[1];
	TerminalSet &terminalSet2 = *board.terminalSets[2];
	Tree tree1(&terminalSet1);
	Tree tree2(&terminalSet2);
	BitMatrix bitMatrix1(5,6);
	BitMatrix bitMatrix2(5,6);
	bitMatrix1.set(0,0);
	bitMatrix1.set(0,3);
	bitMatrix1.set(0,2);
	bitMatrix1.set(0,4);
	bitMatrix1.set(1,0);
	bitMatrix1.set(1,2);
	bitMatrix1.set(1,1);
	bitMatrix1.set(1,4);
	bitMatrix1.set(1,5);
	bitMatrix1.set(2,0);
	bitMatrix1.set(2,5);
	bitMatrix2.set(2,4);
	bitMatrix2.set(3,1);
	bitMatrix2.set(3,0);
	bitMatrix2.set(3,4);
	bitMatrix2.set(3,3);
	bitMatrix2.set(4,3);
	bitMatrix2.set(4,2);
	bitMatrix2.set(4,1);
	tree1.map = bitMatrix1;
	tree1.computeLength();
	tree2.map = bitMatrix2;
	tree2.computeLength();
	solution.trees.push_back(NULL);
	solution.trees.push_back(new Tree(tree1));
	solution.trees.push_back(new Tree(tree2));
	solution.computeMap();
	CleverOptimize cleverOptimize(NULL);
	Solution cleverSolution = cleverOptimize.optimize(solution);
	cout<<cleverSolution;
}

/*
TEST(PlusTest)
{
	assert(1 + 1 == 2);
	assert(1 + 2 != 4);
}

#include <thread>

void output(int x)
{
	int ans = 0;
	for(int i = 0; i < x; i++)
		for(int j = 0; j < x; j++)
			ans += i * j;
	printf("%d\n", ans);
}
TEST(ThreadTest)
{
	using std::thread;
	Vector<thread> th;
	for(int i = 0; i < 2; i++)
		th.push_back(thread(output, 100000));
	for(auto &x: th)
		x.join();
}

TEST(PXTest)
{
	output(100000);
}
*/
#include "../include/BitMatrix.h"
// TEST(BitMatrixTest)
void BitMatrixTest()
{
	BitMatrix bmx(2, 3);
	cout << bmx;
	bmx.set(1, 2);
	cout << bmx;
	bmx.set(1, 0);
	cout << bmx;
	bmx.reset(1, 0);
	cout << bmx;
}

#include "../include/Board.h"
// TEST(BoardIOTest)
void BoardIOTest()
{
	String ins =
		"3 4\n"
		"0 -1 1 2 \n"
		"2 0 -1 2 \n"
		"3 4 2 2 \n"
	;
	StringStream inf(ins);
	StringStream ouf, ouf1, ouf2;
	Board board;
	board.input(0, inf);
	board.output(0, ouf);
	assert(ins == ouf.str());
	board.output(1, ouf1);
	Board board1;
	board1.input(1, ouf1);
	board1.output(1,ouf2);
	assert(ouf1.str() == ouf2.str());
}

#include "../include/Solver.h"
#include "../include/StupidStrategy.h"
#include "../include/DACSolve.h"
#include "../include/ColumnGenSolve.h"
// TEST(StupidStrategyTest)
void StupidStrategyTest()
{
	// String ins; 
	// StringStream inf(ins);
	Board board;
	board.input(0);
	Solver solver(&board);
	// solver.setSolveStrategy(new ColumnGenSolve(&solver));
	solver.setSolveStrategy(new DACSolve(&solver, new ColumnGenSolve(&solver)));
	solver.setCheckStrategy(new StupidCheck(&solver));
	solver.setOptimizeStrategy(new CleverOptimize(&solver));
	Solution solution = solver.run();
	cout << solution;
}

// TEST(LargeTest)
void LargeTest()
{
	String ins =
		"50 50\n"
		"1 6\n"
			"1 1 2 2 4 4 8 8 16 16 32 32\n"
		"2 4\n"
			"10 8 8 10 49 49 0 30\n"
		"3 3\n"
			"1 6 0 38 1 27\n"
		"-1 10\n"
			"3 0 3 1 3 2 3 3 3 4 3 5 3 6 3 7 3 8 3 9\n"
	;
	StringStream inf(ins);
	Board board;
	board.input(1, inf);
	Solver solver(&board);
	// solver.setSolveStrategy(new ColumnGenSolve(&solver));
	solver.setSolveStrategy(new DACSolve(&solver, new ColumnGenSolve(&solver)));
	solver.setCheckStrategy(new StupidCheck(&solver));
	solver.setOptimizeStrategy(new StupidOptimize(&solver));
	Solution solution = solver.run();
	cout << solution;
}

#include <set>
typedef std::pair<int, int> PII;

static void rp(OStream &ost, int n, int m){
	static std::set<PII> S;
	int x, y;
	do
	{
		x = rand() % n;
		y = rand() % m;
	} while(S.count(PII(x, y)));
	S.insert(PII(x, y));
	ost << x << ' ' << y << ' ';
}

TEST(LargeTest2)
// void LargeTest2()
{
	int n = 300, m = 300, t = 10, tl = 3, tr = 3, ob = rand() % 10086;
	StringStream inf;
	
	srand(time(0));
	inf << n << ' ' << m << ' ';
	for(int i = 1; i <= t; i++)
	{
		inf << i << ' ';
		int nt = rand() % (tr - tl + 1) + tl;
		inf << nt << ' ';
		while(nt--) rp(inf, n, m);
	}
	inf << -1 << ' ' << ob << ' ';
	while(ob--)
		rp(inf, n, m);
	
	// cout << inf.str();
	
	Board board;
	board.input(1, inf);
	// board.output();
	// cout.flush();
	Solver solver(&board);
	// solver.setSolveStrategy(new ColumnGenSolve(&solver));
	solver.setSolveStrategy(new DACSolve(&solver, new ColumnGenSolve(&solver)));
	solver.setCheckStrategy(new StupidCheck(&solver));
	solver.setOptimizeStrategy(new StupidOptimize(&solver));
	Solution solution = solver.run();
	OFStream fileOut("result.txt");
	fileOut << solution;
	// cout << solution.map;
}

#include "../include/GRBFactory.h"
// TEST(GurobiTest)
void GurobiTest()
{
  try {
    GRBModel &model = GRBFactory::createModel();

    // Create variables

    GRBVar x1 = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS, "x1");
    GRBVar x2 = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS, "x2");
    // GRBVar x3 = model.addVar(0, GRB_INFINITY, 0, GRB_INTEGER, "x3");

    // Integrate new variables

    model.update();

    // Set objective: maximize x + y + 2 z

    model.setObjective(5 * x1 + 4 * x2, GRB_MAXIMIZE);

    // Add constraint: x + 2 y + 3 z <= 4

    GRBConstr c0 = model.addConstr(x1 + 3 * x2 <= 90, "c0");
    GRBConstr c1 = model.addConstr(2 * x1 + x2 <= 80, "c1");
    GRBConstr c2 = model.addConstr(x1 + x2 <= 45, "c2");

    // Optimize model

    model.optimize();

    cout << x1.get(GRB_StringAttr_VarName) << " "
         << x1.get(GRB_DoubleAttr_X) << endl;
    cout << x2.get(GRB_StringAttr_VarName) << " "
         << x2.get(GRB_DoubleAttr_X) << endl;

    cout << c0.get(GRB_StringAttr_ConstrName) << " "
         << c0.get(GRB_DoubleAttr_Pi) << endl;

    cout << c1.get(GRB_StringAttr_ConstrName) << " "
         << c1.get(GRB_DoubleAttr_Pi) << endl;

    cout << c2.get(GRB_StringAttr_ConstrName) << " "
         << c2.get(GRB_DoubleAttr_Pi) << endl;

    cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }
}
