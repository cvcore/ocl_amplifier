/*
 * main.c
 *
 *  Created on: Nov 13, 2013
 *      Author: core
 *
 *  MCU: ATmega8L
 *  Crystal: 8.000000MHz
 *  Fuse: H:D9, L:E1
 */
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <math.h>
#include "LCD1602.c"
#include "AD5291.c"
#include "AD637.c"
#include "myutil.c"
#include "keyboard.c"

int main() {
	int G = 512, keyInterval = 1;
	float rms, power = 0, setPower = 3.0;
	unsigned char keyBuf;
	bool adRefresh = true, pwrValueRefresh = true, gainValueRefresh = true;
	bool enAGC = true, atLimit = false;
//	isAdj = false;

	lcdInit();
	ec11Init();
	adInit();
	lcdBacklit(true);

	lcdPrintStr("Design: CX Wang");
	lcdLocate(2, 1);
	lcdPrintStr("Initializing..");
	idelay_s(1);
	lcdClear();

	for(;;) {
		if(pwrValueRefresh || gainValueRefresh) {
			/*
			 * Display
			 */
			lcdWrite(0x0c,0,1); // Turn cursor off
			if(pwrValueRefresh) {
				pwrValueRefresh = false;

				lcdLocate(1, 0);
				lcdPrintStr("PWR: ");
				if(power < 10.00)
					lcdPrintFloat(power, 3, 1);
				else
					lcdPrintStr(">10");
				lcdPrintStr("W");
				if(enAGC) {
					lcdPrintStr("/");
					lcdPrintFloat(setPower, 3, 1);
					lcdPrintStr("W");
				}
			}
			if(gainValueRefresh) {
				gainValueRefresh = false;
				lcdLocate(0, 1);
				lcdPrintStr("Gain: ");
				lcdPrintInt(G, 4);
				if(G == 0 || G == 2046) {
					lcdLocate(15, 1);
					lcdPrintStr("*");
					atLimit = true;
				} else if(atLimit) {
					atLimit = false;
					lcdLocate(15, 1);
					lcdPrintStr(" ");
				}

			}
			if(enAGC)
				lcdLocate(15, 0);
			else
				lcdLocate(10, 1);
			lcdWrite(0x0e,0,1); // Turn cursor on
		}

		/*
		 * Adjustment
		 */
		rms = (float)adcRead(0) * 10.0 / 1024.0;
		power = rms * rms / 8.0;
		if(enAGC) {
			if(power < setPower) {
				G += imax(1, fabs(power - setPower) * 10);
				if(G > 2046)
					G = 2046;
				adRefresh = true;
				gainValueRefresh = pwrValueRefresh = true;
//				isAdj = true;
			} else if(power > setPower + 0.02) {
				/* Ensure precision for power higher than 0.4watt and reduce oscillation
				 * got x1.03 retired */
				G -= imax(1, fabs(power - setPower) * 10);
				if(G < 0)
					G = 0;
				adRefresh = true;
				gainValueRefresh = pwrValueRefresh = true;
//				isAdj = true;
			}
//			else if(isAdj) {
//				isAdj = false;
//				pwrValueRefresh = pwrValueRefresh = true;
//			}
		} else if(fabs(power - setPower) > 0.05){
			setPower = power;
			if(setPower > 7.50)
				setPower = 7.50;
			pwrValueRefresh = true;
		}
		if(adRefresh) {
			adRefresh = false;
			adSetGain(G);
		}

		if((keyBuf = ec11Check()) == 2) {
			if(enAGC) {
				setPower += 0.10;
				if(setPower > 7.50)
					setPower = 7.50;
				pwrValueRefresh = true;
			} else {
				G += keyInterval;
				if(G > 2046)
					G = 2046;
				gainValueRefresh = adRefresh = true;
				if(keyInterval < 200)
					keyInterval += 8;
			}
		} else if(keyBuf == 1) {
			if(enAGC) {
				setPower -= 0.10;
				if(setPower < 0)
					setPower = 0;
				pwrValueRefresh = true;
			} else {
				G -= keyInterval;
				if(G < 0)
					G = 0;
				gainValueRefresh = adRefresh = true;
				if(keyInterval < 200)
					keyInterval += 8;
			}
		} else if(keyBuf & 0b100) {
			enAGC = !enAGC;
			lcdClear();
			pwrValueRefresh = gainValueRefresh = true;
		}
		if(keyInterval > 1)
			keyInterval--;
	}
}
