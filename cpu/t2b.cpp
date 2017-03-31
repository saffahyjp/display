#include <cstdio>
#include <cctype>

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("usage: t2b [infile] [outfile]\n");
		return 1;
	}
	FILE *fin = fopen(argv[1], "r");
	FILE *fout = fopen(argv[2], "wb");
	if(!fin || !fout){
		printf("cannot open file\n");
		return 1;
	}
	char chi = 0, clo = 0, ccnt = 0; char cch;
	while(fscanf(fin, "%c", &cch) != EOF){
		if(!isdigit(cch)) continue;
		if(ccnt < 8) chi = chi * 2 + (cch - '0');
		else clo = clo * 2 + (cch - '0');
		if((++ccnt) == 16){
			fprintf(fout, "%c%c", clo, chi);
			clo = chi = ccnt = 0;
		}
	}
	return 0;
	
}
