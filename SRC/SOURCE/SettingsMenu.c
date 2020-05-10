/*
 * SettingsMenu.c
 *
 *  Created on: 17 gru 2019
 *      Author: Tomasz Bielas
 */

#include "../HEADERS/SettingsMenu.h"

/* FUNCTIONS */
void SettingsMenu_ShowMenu(struct SettingsMenu_values *settinsgMenu_values) {

	//creating temporary files for discard changes way
	struct SettingsMenu_values tmpValues;

	tmpValues.pwmBrightness = settinsgMenu_values->pwmBrightness;
	tmpValues.timeDelay = settinsgMenu_values->timeDelay;
	tmpValues.ledStatus = settinsgMenu_values->ledStatus;
	tmpValues.debugMode = settinsgMenu_values->debugMode;
	tmpValues.showChangingPage = settinsgMenu_values->showChangingPage;

	int settingsPage = 1;

	//showing button maping on screen
	LCD_Clear();
	LCD_XYPrintf(0, 0, "%[");
	LCD_XYPrintf(15, 0, "%]");
	LCD_XYPrintf(0, 1, "%[");
	LCD_XYPrintf(15, 1, "%]");
	LCD_XYPrintf(3, 0, "SAVE");
	LCD_XYPrintf(9, 0, "EXIT");
	LCD_XYPrintf(3, 1, "%<%>");
	LCD_XYPrintf(11, 1, "%<%>");
	LCD_XYPrintf(2, 0, "|");
	LCD_XYPrintf(2, 1, "|");
	LCD_XYPrintf(13, 0, "|");
	LCD_XYPrintf(13, 1, "|");
	DELAY_ms(2000);
	LCD_Clear();

	//entering to settings menu loop
	while (1) {

		// showing 1 page of settings "standby"
		if (settingsPage == 1) {
			SettingsMenu_PrintLabel("Standby"); //print page on screen
			SettingsMenu_pressedButton(&tmpValues, &settingsPage); //function to chage pages or change status of selected option
		}
		//showing 2 page of settings "brightness settings"
		else if (settingsPage == 2) {
			SettingsMenu_PrintValue(tmpValues.pwmBrightness, "Brightness"); //print page on screen
			SettingsMenu_pressedButton(&tmpValues, &settingsPage); //function to chage pages or change status of selected option
		}
		//showing 3 page of settings "turning on or off led notification"
		else if (settingsPage == 3) {
			SettingsMenu_PrintBoolean(tmpValues.ledStatus, "Led info"); //print page on screen
			SettingsMenu_pressedButton(&tmpValues, &settingsPage); //function to chage pages or change status of selected option
		}
		//showing 4 page of settings "changing time of delay beetwen changing pages"
		else if (settingsPage == 4) {
			SettingsMenu_PrintValue(tmpValues.timeDelay, "Delay time"); //print page on screen
			SettingsMenu_pressedButton(&tmpValues, &settingsPage); //function to chage pages or change status of selected option
		}
		//showing 5 page of settings "turning on debug info mode"
		else if (settingsPage == 5) {
			SettingsMenu_PrintBoolean(tmpValues.debugMode, "Debug mode"); //print page on screen
			SettingsMenu_pressedButton(&tmpValues, &settingsPage); //function to chage pages or change status of selected option
		}
		//showing 5 page of settings "turning on debug info mode"
		else if (settingsPage == 6) {
			SettingsMenu_PrintBoolean(tmpValues.showChangingPage,
					"Page number"); //print page on screen
			SettingsMenu_pressedButton(&tmpValues, &settingsPage); //function to chage pages or change status of selected option
		}
		// save settings and exit of settings loop
		else if (settingsPage > 100) {
			LCD_Clear();
			LCD_XYPrintf(0, 0, "Saving...");
			settinsgMenu_values->pwmBrightness = tmpValues.pwmBrightness;
			settinsgMenu_values->timeDelay = tmpValues.timeDelay;
			settinsgMenu_values->ledStatus = tmpValues.ledStatus;
			settinsgMenu_values->debugMode = tmpValues.debugMode;
			settinsgMenu_values->showChangingPage = tmpValues.showChangingPage;
			EEPROM_WriteByte(settinsgMenu_values->eepromAddressBrightness,
					settinsgMenu_values->pwmBrightness);
			DELAY_ms(200);
			EEPROM_WriteByte(settinsgMenu_values->eepromAddressLedStatus,
					settinsgMenu_values->ledStatus);
			DELAY_ms(200);
			EEPROM_WriteByte(settinsgMenu_values->eepromAddressTimeDelay,
					settinsgMenu_values->timeDelay);
			DELAY_ms(200);
			EEPROM_WriteByte(settinsgMenu_values->eepromAddressDebugMode,
					settinsgMenu_values->debugMode);
			DELAY_ms(200);
			EEPROM_WriteByte(settinsgMenu_values->eepromAddressShowChangingPage,
					settinsgMenu_values->showChangingPage);
			DELAY_ms(200);
			LCD_Clear();
			LCD_SetCursor(0, 0);
			LCD_XYPrintf(0, 0, "Saved");
			DELAY_ms(1000);
			return;
		}
		//discard settings and exit of settings loop
		else if (settingsPage < 0) {
			LCDEXPLOITING_SetLctBrightness(settinsgMenu_values->pwmBrightness);
			LCD_Clear();
			LCD_SetCursor(0, 0);
			LCD_XYPrintf(0, 0, "Canceled");
			DELAY_ms(1000);
			return;
		}

	}
}

void SettingsMenu_pressedButton(struct SettingsMenu_values *tmpValues,
		int *currentPage) {
	int refreshClock = 0;
	while (1) { //loop of operation
		//execute standby
		if (*currentPage == 1) {
			if (BTN_LEFT) { //if pressed < then change page to last
				*currentPage = 6;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) { //if pressed > then change to next page
				*currentPage = 2;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) { //if pressed save button then change value of current page more than 100 and exit (go to save statement)
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) { //if pressed discard button then change value of current page under 0 and exit (go to discard statement)
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3 || BTN_4) {  //standby
				LedControl_OFF(0); //turn off notification led
				LCD_Clear(); //clear screen
				LCDEXPLOITING_SetLctBrightness(0); //turn off lcd light
				DELAY_ms(1000);
				while (1)
					if (BTN_1 || BTN_2 || BTN_3 || BTN_4 || BTN_LEFT
							|| BTN_RIGHT)
						break; //if press any button then exit of standby
				DELAY_ms(500);
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness); //set lcd light to previous value
				SettingsMenu_PrintLabel("Standby"); //draw on lcd gui
			}
		}
		//execute brightness
		if (*currentPage == 2) {
			if (BTN_LEFT) {  //if pressed < then change to last page
				*currentPage = 1;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) { //if pressed > then change to next page
				*currentPage = 3;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) { //if pressed save button then change value of current page more than 100 and exit (go to save statement)
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) { //if pressed discard button then change value of current page under 0 and exit (go to discard statement)
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3 && tmpValues->pwmBrightness > 0) { //brightness-
				tmpValues->pwmBrightness -= 10;
				if (tmpValues->pwmBrightness < 0)
					tmpValues->pwmBrightness = 0;
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness");
				DELAY_ms(200);
			}
			if (BTN_4 && tmpValues->pwmBrightness < 100) { //brightness+
				tmpValues->pwmBrightness += 10;
				if (tmpValues->pwmBrightness > 100)
					tmpValues->pwmBrightness = 100;
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness");
				DELAY_ms(200);
			}
		}
		//execute led status
		if (*currentPage == 3) {
			if (BTN_LEFT) { //if pressed < then change to last page
				*currentPage = 2;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) { //if pressed > then change to next page
				*currentPage = 4;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) { //if pressed save button then change value of current page more than 100 and exit (go to save statement)
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) { //if pressed discard button then change value of current page under 0 and exit (go to discard statement)
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3) { //led off
				tmpValues->ledStatus = false;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info");
				DELAY_ms(200);
			}
			if (BTN_4) { //led on
				tmpValues->ledStatus = true;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info");
				DELAY_ms(200);
			}
		}
		//execute time delay
		if (*currentPage == 4) {
			if (BTN_LEFT) { //if pressed < then change to last page
				*currentPage = 3;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) { //if pressed > then change to next page
				*currentPage = 5;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) { //if pressed save button then change value of current page more than 100 and exit (go to save statement)
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) { //if pressed discard button then change value of current page under 0 and exit (go to discard statement)
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3 && tmpValues->timeDelay > 50) { //delay-
				tmpValues->timeDelay -= 25;
				if(tmpValues->timeDelay < 50)tmpValues->timeDelay = 50;
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time");
				DELAY_ms(200);
			}
			if (BTN_4 && tmpValues->timeDelay < 250) { //delay+
				tmpValues->timeDelay += 25;
				if(tmpValues->timeDelay > 250) tmpValues->timeDelay = 250;
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time");
				DELAY_ms(200);
			}
		}
		//execute debug mode
		if (*currentPage == 5) {
			if (BTN_LEFT) { //if pressed < then change to last page
				*currentPage = 4;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) { //if pressed > then change to first page
				*currentPage = 6;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) { //if pressed save button then change value of current page more than 100 and exit (go to save statement)
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) { //if pressed discard button then change value of current page under 0 and exit (go to discard statement)
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3) { //debug mode off
				tmpValues->debugMode = false;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode");
				DELAY_ms(200);
			}
			if (BTN_4) { //debug mode on
				tmpValues->debugMode = true;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode");
				DELAY_ms(200);
			}
		}
		//show number of page when changing
		if (*currentPage == 6) {
			if (BTN_LEFT) { //if pressed < then change to last page
				*currentPage = 5;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) { //if pressed > then change to first page
				*currentPage = 1;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) { //if pressed save button then change value of current page more than 100 and exit (go to save statement)
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) { //if pressed discard button then change value of current page under 0 and exit (go to discard statement)
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3) { //debug mode off
				tmpValues->showChangingPage = false;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number"); //print page on screen
				DELAY_ms(200);
			}
			if (BTN_4) { //debug mode on
				tmpValues->showChangingPage = true;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number"); //print page on screen
				DELAY_ms(200);
			}
		}
		LCD_RefreshScreen(&refreshClock, 500);
	}
}

void SettingsMenu_PrintBoolean(bool status, char *label) {
	LCD_Clear();
	if (status)
		LCD_XYPrintf(0, 0, "%s: ON", label);
	else
		LCD_XYPrintf(0, 0, "%s: OFF", label);
	LCD_XYPrintf(0, 1, "OFF");
	LCD_XYPrintf(14, 1, "ON");
}

void SettingsMenu_PrintValue(int value, char *label) {
	LCD_Clear();
	LCD_XYPrintf(0, 0, "%s: %d", label, value);
	LCD_XYPrintf(0, 1, "-");
	LCD_XYPrintf(15, 1, "+");
}

void SettingsMenu_PrintLabel(char *label) {
	LCD_Clear();
	LCD_XYPrintf(0, 0, "%s", label);
	LCD_XYPrintf(0, 1, "ok");
	LCD_XYPrintf(14, 1, "ok");
}
