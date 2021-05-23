#include "../HEADERS/SettingsMenu.h"

void SettingsMenu_ShowMenu(struct SettingsMenu_values *settinsgMenu_values) {
	struct SettingsMenu_values tmpValues;
	tmpValues.pwmBrightness = settinsgMenu_values->pwmBrightness;
	tmpValues.timeDelay = settinsgMenu_values->timeDelay;
	tmpValues.ledStatus = settinsgMenu_values->ledStatus;
	tmpValues.debugMode = settinsgMenu_values->debugMode;
	tmpValues.showChangingPage = settinsgMenu_values->showChangingPage;
	int settingsPage = 1;
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
	while (1) {
		if (settingsPage > 100) {
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
		else if (settingsPage < 0) {
			LCDEXPLOITING_SetLctBrightness(settinsgMenu_values->pwmBrightness);
			LCD_Clear();
			LCD_SetCursor(0, 0);
			LCD_XYPrintf(0, 0, "Canceled");
			DELAY_ms(1000);
			return;
		}
		else
			SettingsMenu_pressedButton(&tmpValues, &settingsPage);
	}
}

void SettingsMenu_pressedButton(struct SettingsMenu_values *tmpValues,
		int *currentPage) {
	long refreshClock = 0;
	long refreshClockMax = 5000;
	while (1) {
		if (*currentPage == 1) {
			if (refreshClock < refreshClockMax)
				refreshClock++;
			else {
				refreshClock = 0;
				SettingsMenu_PrintLabel("Standby");
			}
			if (BTN_LEFT) {
				*currentPage = 6;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) {
				*currentPage = 2;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3 || BTN_4) {
				LedControl_OFF(0);
				LCD_Clear();
				LCDEXPLOITING_SetLctBrightness(0);
				DELAY_ms(1000);
				while (1)
					if (BTN_1 || BTN_2 || BTN_3 || BTN_4 || BTN_LEFT
							|| BTN_RIGHT)
						break;
				DELAY_ms(500);
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);
				SettingsMenu_PrintLabel("Standby");
			}
		}
		if (*currentPage == 2) {
			if (refreshClock < refreshClockMax)
				refreshClock++;
			else {
				refreshClock = 0;
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness");
			}
			if (BTN_LEFT) {
				*currentPage = 1;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) {
				*currentPage = 3;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3 && tmpValues->pwmBrightness > 0) {
				tmpValues->pwmBrightness -= 10;
				if (tmpValues->pwmBrightness < 0)
					tmpValues->pwmBrightness = 0;
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness");
				DELAY_ms(200);
			}
			if (BTN_4 && tmpValues->pwmBrightness < 100) {
				tmpValues->pwmBrightness += 10;
				if (tmpValues->pwmBrightness > 100)
					tmpValues->pwmBrightness = 100;
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness");
				DELAY_ms(200);
			}
		}
		if (*currentPage == 3) {
			if (refreshClock < refreshClockMax)
				refreshClock++;
			else {
				refreshClock = 0;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info");
			}
			if (BTN_LEFT) {
				*currentPage = 2;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) {
				*currentPage = 4;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3) {
				tmpValues->ledStatus = false;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info");
				DELAY_ms(200);
			}
			if (BTN_4) {
				tmpValues->ledStatus = true;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info");
				DELAY_ms(200);
			}
		}
		if (*currentPage == 4) {
			if (refreshClock < refreshClockMax)
				refreshClock++;
			else {
				refreshClock = 0;
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time");
			}
			if (BTN_LEFT) {
				*currentPage = 3;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) {
				*currentPage = 5;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3 && tmpValues->timeDelay > 50) {
				tmpValues->timeDelay -= 25;
				if (tmpValues->timeDelay < 50)
					tmpValues->timeDelay = 50;
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time");
				DELAY_ms(200);
			}
			if (BTN_4 && tmpValues->timeDelay < 250) {
				tmpValues->timeDelay += 25;
				if (tmpValues->timeDelay > 250)
					tmpValues->timeDelay = 250;
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time");
				DELAY_ms(200);
			}
		}
		if (*currentPage == 5) {
			if (refreshClock < refreshClockMax)
				refreshClock++;
			else {
				refreshClock = 0;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode");
			}
			if (BTN_LEFT) {
				*currentPage = 4;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) {
				*currentPage = 6;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3) {
				tmpValues->debugMode = false;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode");
				DELAY_ms(200);
			}
			if (BTN_4) {
				tmpValues->debugMode = true;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode");
				DELAY_ms(200);
			}
		}
		if (*currentPage == 6) {
			if (refreshClock < refreshClockMax)
				refreshClock++;
			else {
				refreshClock = 0;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number");
			}
			if (BTN_LEFT) {
				*currentPage = 5;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_RIGHT) {
				*currentPage = 1;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}
			if (BTN_3) {
				tmpValues->showChangingPage = false;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number");
				DELAY_ms(200);
			}
			if (BTN_4) {
				tmpValues->showChangingPage = true;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number");
				DELAY_ms(200);
			}
		}
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
