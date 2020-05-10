/*
 * LEDcontrol.h
 *
 *  Created on: 8 lut 2019
 *      Author: Tomasz Bielas
 */

#ifndef LEDCONTROL_H_
#define LEDCONTROL_H_

	void LedControl_ON(int time);
	void LedControl_OFF(int time);
	void LedControl_set(int value);
	void PWM_SetLedBrightness(int val);

#endif /* LEDCONTROL_H_ */
