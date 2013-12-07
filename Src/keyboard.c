/*
 * keyboard.c
 *
 *  Created on: Nov 18, 2013
 *      Author: core
 */
#ifndef _KEYBOARD_C_
#define _KEYBOARD_C_

#ifndef _AVR_IO_H_
#include <avr/io.h>
#endif

#define KEY0 2
#define KEY1 3
#define KEY2 4
#define KEY3 5

#define KEYPORT (PORTC)
#define KEYDDR (DDRC)
#define KEYPIN (PINC)

void keyInit() {
	DDRC &= ~(1<<KEY0 | 1<<KEY1 | 1<<KEY2 | 1<<KEY3);
	PORTC |= (1<<KEY0 | 1<<KEY1 | 1<<KEY2 | 1<<KEY3);
}

unsigned char getKey(bool wait) {
/*
 * 3 means no key
 */
	unsigned char k = ~KEYPIN, r;
	const unsigned char keyArray[4] = {1<<KEY0, 1<<KEY1, 1<<KEY2, 1<<KEY3};

	for(r = 0; (r < 4) && (k & keyArray[r]); r++);
	while(wait && (~KEYPIN & (1<<KEY0 | 1<<KEY1 | 1<<KEY2 | 1<<KEY3)));
	return r;
}

void ec11Init() {
	KEYDDR &= ~(1<<KEY0 | 1<<KEY2 | 1<<KEY3);
//	KEYDDR |= 1<<KEY1;
	KEYPORT |= (1<<KEY0 | 1<<KEY2 | 1<<KEY3);
//	KEYPORT &= ~(1<<KEY1);
}

unsigned char ec11Check() {
/*
 * this function should be wake up at least every several hundred microseconds to work properly
 */
	static bool valid = false;
	unsigned char tmp, ret = 0;

	tmp = (~KEYPIN) & (1<<KEY0 | 1<<KEY2 | 1<<KEY3);
	if(tmp) {
		if(valid) {
			valid = false;
			if(tmp & 1<<KEY2)
				ret = 0b10; //counter clockwise
			else
				ret = 0b01; //clockwise
			if(tmp & 1<<KEY3)
				ret += 0b100; //pushed
		}
	} else {
		valid = true;
	}
	return ret;
}

int keyLevel(int interval) {
	if(interval < 50)
		return 1;
	else if(interval < 70)
		return 10;
	else if(interval < 200)
		return 20;
	else if(interval < 300)
		return 50;
	else if(interval < 500)
		return 100;
	else
		return 200;
}

#endif /* _KEYBOARD_C_ */
