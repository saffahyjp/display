#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <winsock.h>
#include <windows.h>
#include <string.h>
#include <cstring>
#include <conio.h>
#define f(x, y, z) for(int x = (y); x <= (z); ++x)
#define g(x, y, z) for(int x = (y); x < (z); ++x)
#define h(x, y, z) for(int x = (y); x >= (z); --x)

HANDLE com = INVALID_HANDLE_VALUE;

inline bool open(int com_id){
	char com_name[233];
	sprintf(com_name, "COM%d", com_id);
	DCB dcb;
	com = CreateFileA(com_name,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	if (com == INVALID_HANDLE_VALUE) {
		printf("Can not open %s\n",com_name);
		return false;
	}
	GetCommState(com, &dcb);
	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(com,&dcb);
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;//奇校验
	dcb.StopBits = ONESTOPBIT;
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	SetCommState(com, &dcb);
	return true;
}
inline void write(char ch){
	DWORD size=0;
	while(size==0) WriteFile(com,&ch,1,&size,NULL);
	if(com == INVALID_HANDLE_VALUE){
		printf("  COM lost...\n");
		exit(1);
	}
	Sleep(2);
}
inline char read(){
	char ch; DWORD size = 0;
	while(size != 1) ReadFile(com,&ch,1,&size,NULL);
	if(com == INVALID_HANDLE_VALUE){
		printf("  COM lost...\n");
		exit(1);
	}
	return ch;
}
inline void writeint(int x){
	write((x / 16384) & 127);
	write((x / 128) & 127);
	write((x / 1) & 127);
}

int n, a[65536];

// std::vector<char> V;
unsigned char V[65537];

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("usage: boot X.bin\n"); return 1;
	}
	// V.clear();
	FILE *bin = fopen(argv[1], "rb");
	if(!bin){
		printf("invalid file %s\n", argv[1]); return 1;
	}
	// char ch;
	// while((ch = fgetc(bin)) != EOF) V.push_back(ch);
	int n = fread(V, 1, 65536, bin);
	printf("total size %d byte\n", n);
	// int n = V.size();
	if(n % 2){
		printf("odd size file %s\n", argv[1]); return 1;
	}
	n /= 2;
	if(!open(1) && !open(2) && !open(3) && !open(4) && !open(5) && !open(6) && !open(7) && !open(8) && !open(9)){
		begin:
		printf("Input COM id: ");
		int com_id; scanf("%d", &com_id);
		if(!open(com_id)) goto begin;
	}
	// while(1){
		// int x; scanf("%d", &x); write(x & 255);
	// }
	// char buf[233];
	// int n, m; scanf("%d%d", &n, &m); gets(buf);
	int ct = clock();
	// srand(233);
	// g(i, 0, n){
		// a[i] = 2048;
		// a[i] = (rand() * 32768 + rand()) % 65536;
		// a[i] &= (~(1 << 7));
		// a[i] &= (~(1 << 15));
	// }
	printf("boot begin\n");
	writeint(n);
	int ans = 0;
	g(i, 0, n){
		printf("sending %d\n", i);
		a[i] = (int) V[2 * i] + (int) V[2 * i + 1] * 256;
		writeint(a[i]);
		ans ^= a[i];
		printf("sent %d\n", i);
		h(j, 15, 0) putchar((ans >> j & 1) + '0'); putchar('\n');
		// if((i + 1) % m == 0) gets(buf);
	}
	printf("\ntotal time %d\n", (int) clock() - ct);
	while(1) putchar(read());
	return 0;
}




























