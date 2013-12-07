/*
 * AD637.c
 *
 *  Created on: Nov 14, 2013
 *      Author: core
 */
#ifndef _AD637_C_
#define _AD637_C_

#ifndef _AVR_INTERRUPT_H_
#include <avr/interrupt.h>
#endif

// float rms = 0.0;
// bool adcCompleted = false;

//depreciated
//void adcInit() {
//	ADMUX = 0;
//	ADCSRA |= 1<<ADEN | 1<<ADSC| 1<<ADPS2 | 1<<ADPS1; /* 8MHz / 64 = 125kSPS */
//}

int adcRead(short ch) { /* Channel should be in range of 0 .. 7 */
	DDRC &= ~(1<<ch);
	ADMUX = ch;
	ADCSRA |= 1<<ADEN | 1<<ADSC | 1<<ADPS2 | 1<<ADPS1;/* 8MHz / 64 = 125kSPS */
	while(!(ADCSRA & (1 << ADIF)));
	return ADC;
}

/*
ISR(ADC_vect) {
	rms = ADC;
	rms = rms / 1024.0 * 10.0;
	adcCompleted = true;
}
*/

#endif /* _AD637_C_ */
