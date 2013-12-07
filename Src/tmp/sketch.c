#include <stdio.h>
#include <math.h>

void lcdWrite(char a, int b, int c) {
	printf("%c", a);
}

void lcdPrintFloat(float fnum, short width, short prec) {
	long i = fnum, j;
	short len;
	
	len = (i == 0 ? 1 : (log10(i) + 1)) + prec;
	i = fnum * powl(10, prec);
	for(j = 0; j < width - len - (prec != 0); j++)
		lcdWrite('0', 1, 1);
	for(j = powl(10, len - 1); j > 0; i %= j, j /= 10, len--) {
		lcdWrite('0' + i/j, 1, 1);
		if(prec > 0 && len - 1 == prec)
			lcdWrite('.', 1, 1);
	}
}

int main(int argc, char *argv[]) {
	lcdPrintFloat(8.3310, 7, 4);
}

