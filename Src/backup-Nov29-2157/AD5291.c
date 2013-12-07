/*
 * AD5291.c
 *
 *  Created on: Nov 13, 2013
 *      Author: core
 *      CPOL = 0
 *      CPHA = 1
 *
 */
#ifndef _AD5291_C_
#define _AD5291_C_

#ifndef _AVR_IO_H_
#include <avr/io.h>
#endif

#ifndef _STDBOOL_H_
#include <stdbool.h>
#endif

#ifndef _AVR_INTERRUPT_H_
#include <avr/interrupt.h>
#endif

#ifndef _UTIL_DELAY_H_
#include <util/delay.h>
#endif

#ifndef _MYUTIL_C_
#include "myutil.c"
#endif

#define MOSI 3
#define MISO 4
#define SCK 5
#define SS 2
#define ADPORT (PORTB)
#define ADPIN (PINB)
#define ADDDR (DDRB)
#define ADNUM 3

void spiInit() {
	unsigned char idx;
	cli();
	ADDDR |= (1<<MOSI | 1<<SCK | 1<<SS);
	ADDDR &= ~(1<<MISO);
	for(idx = 0; idx < ADNUM; idx++) {
		ADDDR |= (1<<idx);
		ADPORT |= (1<<idx);
	}
	ADPORT &= ~(1<<MOSI | 1<<MISO | 1<<SCK | 1<<SS);
	SPCR |= (1<<SPE | 1<<MSTR | 1<<CPHA | 1<<SPR1);
	SPSR = 0;
	sei();
}

void spiTransmit(int dat, unsigned char dev) {
	PORTB &= ~(1<<dev);
	SPDR = dat >> 8;
	while(!(SPSR & 0x80));
	SPDR = dat;
	while(!(SPSR & 0x80));
	PORTB |= 1<<dev;
}

void adInit() {
	unsigned char idx;
	spiInit();
	for(idx = 0; idx < ADNUM; idx++)
		spiTransmit(0x1802, idx);
}

void adSetGain(int gain) {
	if(gain >= 0 && gain < 2047) {
		spiTransmit(gain/2 + 0x0400, 1);
		spiTransmit(gain/2 + 0x0400, 0);
	}
}

#endif /* _AD5291_C_ */
