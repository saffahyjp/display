#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *f_in = fopen(argv[1],"rb");
	FILE *f_out = fopen(argv[2],"wb");
	
	while (!feof(f_in)) {
		int lo, hi;
		lo = fgetc(f_in);
		hi = fgetc(f_in);
		if (lo == 0 && (hi >> 3) == 29) {  // 11101 x   JR -> JALR
			lo = 192;  // 11000000
		}
		if (lo == 0 && (hi >> 3) == 30) {  // 11110 x   MFIH -> JRRA
			hi = 29 << 3;
			lo = 1 << 5;
		}
		fputc(lo, f_out);
		fputc(hi, f_out);
	}
	
	fclose(f_in);
	fclose(f_out);
}