#include "../include/global.h"
#include "../include/Solver.h"
#include "../include/StupidStrategy.h"
#include "../include/DACSolve.h"
#include "../include/ColumnGenSolve.h"
#include <set>
typedef std::pair<int, int> PII;

void rp(OStream &ost, int n, int m){
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

int main()
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
		
	Board board;
	board.input(1, inf);
	Solver solver(&board);
	solver.setSolveStrategy(new DACSolve(&solver, new ColumnGenSolve(&solver)));
	solver.setCheckStrategy(new StupidCheck(&solver));
	solver.setOptimizeStrategy(new StupidOptimize(&solver));
	Solution solution = solver.run();
	OFStream fileOut("result.txt");
	fileOut << solution;
	return 0;
}
