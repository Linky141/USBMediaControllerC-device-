/*
 * SettingsMenu.h
 *
 *  Created on: 17 gru 2019
 *      Author: Tomasz Bielas
 */
#ifndef SRC_HEADERS_SETTINGSMENU_H_
#define SRC_HEADERS_SETTINGSMENU_H_

/* IMPLEMENTATIONS */
#include <avr\io.h>
#include <stdbool.h>
#include "../../Libraries/Headers/lcd.h"
#include "../../Libraries/Headers/delay.h"
#include "../HEADERS/LEDcontrol.h"
#include "../HEADERS/pwm.h"
#include "../../Libraries/Headers/eeprom.h"

/* DEFINITHIONS */
#define BTN_RIGHT (!(PINB & 0x1))
#define BTN_LEFT (!(PINB & 0x2))
#define BTN_RIGHT_AND_LEFT (!(PINB & 0x3))
#define BTN_1 (!(PINB & 0x4))
#define BTN_2 (!(PINB & 0x8))
#define BTN_3 (!(PINB & 0x10))
#define BTN_4 (!(PINB & 0x20))

/* STRUCTURES */
struct SettingsMenu_values {
	unsigned char eepromAddressBrightness;
	unsigned char eepromAddressLedStatus;
	unsigned char eepromAddressTimeDelay;
	unsigned char eepromAddressDebugMode;
	unsigned char eepromAddressShowChangingPage;
	int pwmBrightness;
	bool ledStatus;
	bool debugMode;
	int timeDelay;
	bool showChangingPage;
};

/* FUNCTIONS */
void SettingsMenu_ShowMenu(struct SettingsMenu_values *settinsgMenu_values);

void SettingsMenu_pressedButton(
		struct SettingsMenu_values *tmpValues, int* currentPage);

void SettingsMenu_PrintBoolean(bool status, char* label);
void SettingsMenu_PrintValue(int value, char* label);
void SettingsMenu_PrintLabel(char* label);



#endif /* SRC_HEADERS_SETTINGSMENU_H_ */

