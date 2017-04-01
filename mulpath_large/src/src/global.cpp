#include "../include/global.h"

int doAssert(bool x, const char *file, int line)
{
	if(x)
		return 0;
	cout << "Assertion failed at file " << file << " line " << line << "\n";
	exit(1);
}
