/*
 * LEDcontrol.c
 *
 *  Created on: 8 lut 2019
 *      Author: Tomasz Bielas
 */

#include "../HEADERS/LEDcontrol.h"
#include "../HEADERS/PWM.h"
#include "../../Libraries/Headers/delay.h"

		/* FUNCTIONS */
		void LedControl_ON(int time) {
			for (int clk = 0; clk < 100; clk++) {
				PWM_SetLedBrightness(clk);
				DELAY_ms(time / 100);
			}
		}

		void LedControl_OFF(int time) {
			for (int clk = 100; clk > 0; clk--) {
				PWM_SetLedBrightness(clk);
				DELAY_ms(time / 100);
			}
		}

		void LedControl_set(int value) {
			PWM_SetLedBrightness(value);
		}

		void PWM_SetLedBrightness(int val) {
			if (val < 0 || val > 100)
				return;
			int tmp = 100 - val;
			PWM_SetDutyCycle(2, tmp);
		}
