#include <cstdlib>
#include <cstdio>

using namespace std;

int main()
{
	for (int i = 1;i <= 30; ++i)
	{
		char command[233];
		sprintf(command, "./main.exe > statistics/%d", i);
		printf("%s\n", command);
		system(command);
	}
	return 0;
}