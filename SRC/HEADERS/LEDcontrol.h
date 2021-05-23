#ifndef LEDCONTROL_H_
#define LEDCONTROL_H_

#include <stdbool.h>

void LedControl_ON(int time);
void LedControl_OFF(int time);
void LedControl_set(int value);
void PWM_SetLedBrightness(int val);
void LedOn(int time, bool state);
void LedOff(int time, bool state);

#endif /* LEDCONTROL_H_ */
