/*
 * LCD1602.c
 *
 *  Created on: Nov 13, 2013
 *      Author: core
 */
#ifndef _LCD1602_C_
#define _LCD1602_C_

#ifndef _AVR_IO_H_
#include <avr/io.h>
#endif

#ifndef _STDBOOL_H
#include <stdbool.h>
#endif

#ifndef _UTIL_DELAY_H_
#include <util/delay.h>
#endif

#ifndef __MATH_H
#include <math.h>
#endif

#ifndef _MYUTIL_C_
#include "myutil.c"
#endif


/* Port Definition */
#define RS 5
#define RW 6
#define EN 7
#define BKL 4
#define DAT_DDR (DDRD)
#define DAT_PORT (PORTD)
#define DAT_PIN (PIND)

#define CON_DDR (DDRD)
#define CON_PORT (PORTD)
#define CON_PIN (PIND)


void lcdWait()
{
	CON_PORT&=~( (1<<RS) | (1<<RW) | (1<<EN) );
	DAT_PORT&=~0x0f;
	DAT_DDR&=~0x0f;
	CON_PORT|=(1<<RW);
	CON_PORT|=(1<<EN);
	_delay_us(1); //waiting for rising edge. useful in high clock freq.
	while(DAT_PIN&0x08);
	CON_PORT&=~(1<<EN);
}

void lcdWrite(unsigned char dat,bool cmd,bool tstbusy) //0:cmd 1:not cmd
{
	if(tstbusy) lcdWait();
	CON_PORT&=~( (1<<RS) | (1<<RW) | (1<<EN) );
	DAT_DDR|=0x0f;
	DAT_PORT&=~0x0f;
	CON_PORT|=(cmd<<RS);
	//CON_PORT&=~(1<<RW);
	DAT_PORT|=((dat&0xf0) >> 4);
	CON_PORT|=(1<<EN);
	CON_PORT&=~(1<<EN);
	DAT_PORT&=~0x0f;
	DAT_PORT|= dat & 0x0f;
	CON_PORT|=(1<<EN);
	CON_PORT&=~(1<<EN);
	DAT_PORT&=~0x0f;
}

unsigned char lcdRead(bool cmd)
{
	 unsigned char ret;
	 ret=0;
	 CON_PORT&=~( (1<<RS) | (1<<RW) | (1<<EN) );
	 DAT_PORT&=~0x0f;
	 DAT_DDR&=~0x0f;
	 CON_PORT|=(cmd<<RS);
	 CON_PORT|=(1<<RW);
	 CON_PORT|=(1<<EN);
	 ret|=(DAT_PIN&0x0f);
	 CON_PORT&=~(1<<EN);
	 CON_PORT|=(1<<EN);
	 ret|=((DAT_PIN << 4) & 0xf0);
	 CON_PORT&=~(1<<EN);
	 return ret;
}

void lcdClear()
{
 	lcdWrite(1,0,1);
}

void lcdLocate(unsigned char lx,unsigned char ly) //(x, y)
{
	 unsigned char locdat;
	 if(ly) locdat=0x40;
	 else locdat=0;
	 locdat=locdat+lx;
	 locdat|=0x80;
	 lcdWrite(locdat,0,1);
}

void lcdPrintStr(char * printdat)
{
	 while(*printdat)
	 {
	  	lcdWrite(*(printdat++),1,1);
	 }
}

/* RETIRED
void lcdPrintFloat(float fnum, char width, char prec) {
	int i, num = fnum * pow(10, prec);
	if(fnum < 0) {
		lcdWrite('-',1,1);
		num = -num;
		width--;
	}
	width -= (log10(num) + 1) + (prec != 0);
	i = pow(10, width - 1);
	for(; width > 0 || prec > 0; width--, prec--) {
		lcdWrite('0',1,1);
		if(prec == 0)
			lcdWrite('.', 1, 1);
	}
	for(; i > 0; num %= i, i /= 10)
		lcdWrite('0' + num/i, 1, 1);
}
*/

void lcdPrintFloat(float fnum, char width, short prec) {
	long i , j;
	char len;

	if(fnum < 0) {
		lcdWrite('-', 1, 1);
		i = -fnum;
	} else {
		i = fnum;
	}
	len = (i == 0 ? 1 : (ilog10(i) + 1)) + prec;
	i = fnum * ipow10(prec);
	for(j = 0; j < width - len - (prec != 0); j++)
		lcdWrite('0', 1, 1);
	for(j = ipow10(len - 1); j > 0; i %= j, j /= 10, len--) {
		lcdWrite('0' + i/j, 1, 1);
		if(prec > 0 && len - 1 == prec)
			lcdWrite('.', 1, 1);
	}
}

void lcdPrintInt(int inum, char width) {
	char len = (inum == 0? 1 : (ilog10(inum) + 1));
	int i;
	for(i = 0; i < width - len; i++)
		lcdWrite('0', 1, 1);
	for(i = ipow10(len - 1); i > 0; inum %= i, i /= 10, len--)
			lcdWrite('0' + inum/i, 1, 1);
}

void lcdBacklit(bool isOn) {
	if(isOn) {
		CON_PORT |= 1<<BKL;
	} else {
		CON_PORT &= ~(1<<BKL);
	}
}


void lcdInit()
{
	DAT_PORT&=~0xf0;
//	if(en_bkl) CON_PORT&=~(1<<BKL);
//	else CON_PORT|=(1<<BKL);
	CON_DDR|=((1<<RS)|(1<<RW)|(1<<EN)|(1<<BKL));
	lcdWrite(0x28,0,0);//4位数据接口 两行 5*7点阵
	_delay_ms(4);
	lcdWrite(0x28,0,0);
	_delay_ms(4);
	lcdWrite(0x28,0,1);
	_delay_ms(4);
	lcdWrite(0x0e,0,1);//开显示，无光标，不闪烁。0x0c 0x0f
	_delay_ms(4);
	lcdWrite(0x06,0,1);//输入设置，AC自增，位置不动
	lcdClear();
}

#endif /* _LCD1602_C_ */
