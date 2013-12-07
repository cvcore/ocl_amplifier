/*
 * myutil.c
 *
 *  Created on: Nov 18, 2013
 *      Author: core
 *
 *  Note: Optimized for this task only, not guaranteed to work in other occasion.
 */
#ifndef _MYUTIL_C_
#define _MYUTIL_C_

#ifndef _UTIL_DELAY_H_
#include <util/delay.h>
#endif

int imin(int a, int b) {
	return (a>b? b: a);
}

int imax(int a, int b) {
	return (a>b? a: b);
}

long ipow10(short p) {
//	long ret = 1;
//	for(; p > 0; p--)
//		ret *= 10;
//	return ret;
	const long ret[] = {1, 10, 100, 1000, 10000, 100000UL, 1000000UL, 10000000UL, 100000000UL, 1000000000UL};
	if(p >= 0 && p < 10)
		return ret[p];
	else
		return 1;
}

void idelay_s(int s) {
	s *= 50;
	while(s-- > 0)
		_delay_ms(10);
}

int ilog10(long l) {
	if(l > 0) {
		if(l < 10)
			return 0;
		else if(l < 100)
			return 1;
		else if(l < 1000)
			return 2;
		else if(l < 10000)
			return 3;
		else if(l < 100000)
			return 4;
		else if(l < 1000000)
			return 5;
		else if(l < 10000000)
			return 6;
		else if(l < 100000000)
			return 7;
		else if(l < 1000000000)
			return 8;
		else
			return 9;
	}
	return 0;
}

#endif /*_MYUTIL_C_*/
