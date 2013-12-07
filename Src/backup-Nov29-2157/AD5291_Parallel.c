/*
 * AD5291.c
 *
 *  Created on: Nov 13, 2013
 *      Author: core
 *      CPOL = 0
 *      CPHA = 1
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

#ifndef _MYUTIL_C_
#include "myutil.c"
#endif

static bool isSpiInited = false;

#define EN0 1
#define EN1 2
#define MOSI 3
#define MISO 4
#define SCK 5
#define ADPORT (PORTB)
#define ADPIN (PINB)
#define ADDDR (DDRB)

void spiInit() {
	cli();
	ADDDR &= ~(1<<MOSI | 1<<SCK | 1<<EN0 | 1<<EN1);
	ADDDR |= 1<<MISO;
	ADPORT |= 1<<MISO;
	SPCR |= 1<<SPE | 1<<MSTR | 1<<CPHA | 1<<SPR1;
	SPSR = 0;
	isSpiInited = true;
	sei();
}

int spiTransmit(unsigned char cmd, unsigned char dat, unsigned char dev) {
	int ret = 0;
	PORTB |= 1<<dev;
	SPDR = cmd;
	while(!(SPSR & 0x80));
	ret |= SPDR<<8;
	SPDR = dat;
	while(!(SPSR & 0x80));
	ret |= SPDR;
	PORTB &= ~(1<<dev);
	return ret;
}

int adWrite(int dat, unsigned char dev) {
	unsigned char tx1, tx2;
	tx1 = 0b100 | (dat>>8 & 0x03);
	tx2 = dat;
	return spiTransmit(tx1, tx2, dev);
}

int adRead(unsigned char dev) {
	return spiTransmit(0, 0, dev);
}

int adReadRDAC(unsigned char dev) {
	spiTransmit(0b1000, 0, dev);
	return adRead(dev);
}

void adStoreRDAC(unsigned char dev) {
	spiTransmit(0b1100, 0, dev);
}

void adReset(unsigned char dev) {
	spiTransmit(0x10, 0, dev);
}

int adReadMemory(unsigned char addr, unsigned char dev) {
	spiTransmit(0b10100, addr, dev);
	return adRead(dev);
}

void adWriteCtrlReg(unsigned char dat, unsigned char dev) {
	spiTransmit(0b11000, dat, dev);
}

int adReadCtrlReg(unsigned char dev) {
	spiTransmit(0b11100, 0, dev);
	return adRead(dev);
}

void adShutdown(bool isTrue, unsigned char dev) {
	spiTransmit(0b100000, isTrue, dev);
}

void adInit(unsigned char dev) {
	if(!isSpiInited)
		spiInit();
	adWriteCtrlReg(0b11, dev);
}

void adGain(int total) {
/* Note
 * first level:
 * 4k / 50k * 256 = 20.48
 * second level:
 * 4k / 20k *256 = 51.2
 *
 * due to the limitation of current in AD5291's A, W, B port is only 3mA
 * the two level's control value shouldn't be smaller than above.
 *
 * total in range 0 .. 692
 */
	unsigned char att, amp;

	att = imin(1023, total);
	if(total > 1023) {
		amp = total - 1023;
	}

}

#endif /* _AD5291_C_ */
