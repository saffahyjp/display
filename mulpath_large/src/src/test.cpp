#include "../include/test.h"

Vector<Test *> Test::tests;

void Test::runTests()
{
	for(auto test: tests)
	{
		cout << "Running test " << test->name << "\n";
		auto clkBegin = clock();
		test->run();
		auto clkEnd = clock();
		cout << "Test " << test->name << " finished in "
			<< (double) (clkEnd - clkBegin) / CLOCKS_PER_SEC << " seconds.\n";
	}
}
