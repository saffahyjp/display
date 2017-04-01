#include "bits/stdc++.h"

typedef std::pair<int, int> PII;
std::set<PII> S;
int n, m, t, tl, tr, ob, T;

void rp(){
	int x, y;
	do{
		x = rand() % n;
		y = rand() % m;
	}while(S.count(PII(x, y)));
	S.insert(PII(x, y));
	printf("%d %d\n", x, y);
}

int main(int argc, char *argv[]){
	sscanf(argv[1], "%d", &n);
	sscanf(argv[2], "%d", &m);
	sscanf(argv[3], "%d", &t);
	sscanf(argv[4], "%d", &tl);
	sscanf(argv[5], "%d", &tr);
	sscanf(argv[6], "%d", &ob);
	sscanf(argv[7], "%d", &T);
	srand(121449137 + 23456789 * n + 171792544 * m + 644240545 * t + 998244353 * tl + 10007 * tr + 10009 * ob + 23 * 120);
	printf("%d %d\n", n, m);
	while(t--){
		printf("Terminal\n");
		int nt = rand() % (tr - tl + 1) + tl;
		printf("%d\n", nt);
		while(nt--) rp();
	}
	while(ob--){
		printf("Obstacle\n");
		rp();
	}
	printf("Route %d\nBalanced\n", T);
	return 0;
}
