/*
 * main.c
 *
 *  Created on: Nov 13, 2013
 *      Author: core
 *
 *  MCU: ATmega8L
 *  Crystal: 8.000000MHz
 */
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "LCD1602.c"
#include "AD5291.c"
#include "AD637.c"
#include "myutil.c"
#include "keyboard.c"

int main() {
	int G = 512;
	float rms, power = 0, setPower = 3.0;
	unsigned char keyBuf;
	bool adRefresh = true, pwrValueRefresh = true, gainValueRefresh = true;
	bool enAGC = true, atLimit = false, isAdj = false;

	lcdInit();
	ec11Init();
	adInit();
	lcdBacklit(true);

	for(;;) {
		if(pwrValueRefresh || gainValueRefresh) {
			/*
			 * Display
			 */
			lcdWrite(0x0c,0,1); // Turn curson off
			if(pwrValueRefresh) {
				pwrValueRefresh = false;

				lcdLocate(1, 0);
				lcdPrintStr("PWR: ");
				lcdPrintFloat(power, 3, 1);
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
			lcdWrite(0x0e,0,1); // Turn cursor on
		}

		rms = (float)adcRead(0) * 10.0 / 1024.0;
		power = rms * rms / 8.0;
		if(enAGC) {
			if((!isAdj && power < setPower) || (isAdj && power < setPower + 0.1)) {
				if(G < 2047 - 1)
					G++;
				adRefresh = true;
				gainValueRefresh = true;
				isAdj = true;
			} else if((!isAdj && power > setPower + 0.03) || (isAdj && power > setPower + 0.02)) {
				/* Ensure precision for power higher than 0.4watt and reduce oscillation
				 * got x1.03 retired */
				if(G > 0)
					G--;
				adRefresh = true;
				gainValueRefresh = true;
				isAdj = true;
			} else if(isAdj) {
				isAdj = false;
				pwrValueRefresh = true;
			}
		}
		if(adRefresh) {
			adRefresh = false;
			adSetGain(G);
		}

		if((keyBuf = ec11Check()) == 2) {
			if(enAGC) {
				setPower += 0.10;
				pwrValueRefresh = true;
			} else {
				G++;
				gainValueRefresh = true;
			}
		} else if(keyBuf == 1) {
			if(enAGC) {
				setPower -= 0.10;
				pwrValueRefresh = true;
			} else {
				G--;
				gainValueRefresh = true;
			}
		} else if(keyBuf & 0b100) {
			enAGC = !enAGC;
			lcdClear();
			pwrValueRefresh = gainValueRefresh = true;
		}
	}
}
