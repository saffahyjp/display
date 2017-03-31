#include <cstdio>

char buf[233333];

int main(){
	while(gets(buf)) if(buf[0] != '#') printf("%s\n", buf);
	return 0;
}
