/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJE */

#include "../HEADERS/SettingsMenu.h"

/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJA METOD */

//------------------------------------------------------------------------
//Wy�wietlanie menu konfiguracyjnego urz�denia.
//Do metody wysy�ane s� warto�ci ustawie� wraz z adresami
void SettingsMenu_ShowMenu(struct SettingsMenu_values *settinsgMenu_values) {

	//tworzenie tymczasowej struktury z wybranymi ustawieniami w celu prostego odrzucenia zmian w razie takiej potrzeby
	struct SettingsMenu_values tmpValues;

	//przypisanie do tymczasowego obiektu parametr�w zgodnych z aktualnym stanem
	tmpValues.pwmBrightness = settinsgMenu_values->pwmBrightness;
	tmpValues.timeDelay = settinsgMenu_values->timeDelay;
	tmpValues.ledStatus = settinsgMenu_values->ledStatus;
	tmpValues.debugMode = settinsgMenu_values->debugMode;
	tmpValues.showChangingPage = settinsgMenu_values->showChangingPage;

	//ostawienie numeru strony w ustawieniach na 1
	int settingsPage = 1;

	//przez 2000ms na ekranie zostaje wy�wietlone znaczenie klawiszy w tym menu
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

	//p�tla g��wna ustawie�
	while (1) {

		//je�eli warto�� numeru strony jest wi�ksza ni� 100 oznacza to, �e u�ytkonik chce zapisa� obecne ustawienia
		if (settingsPage > 100) {

			//Pierw czy�ci ekran i wy�wietla komunikat o zapisie
			LCD_Clear();
			LCD_XYPrintf(0, 0, "Saving...");

			//do zmiennych u�ytkowych przypisywane s� nowe warto�ci przetrzymywane w zmiennych tymczasowych
			settinsgMenu_values->pwmBrightness = tmpValues.pwmBrightness;
			settinsgMenu_values->timeDelay = tmpValues.timeDelay;
			settinsgMenu_values->ledStatus = tmpValues.ledStatus;
			settinsgMenu_values->debugMode = tmpValues.debugMode;
			settinsgMenu_values->showChangingPage = tmpValues.showChangingPage;

			//kolejnie warto�ci s� zapisywane do eepromu pod odpowiednie adresy z zachowaniem odst�pu 200ms pomi�dzy ka�dym zapisem
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

			//kolejnie przez 1000ms wy�wietla si� komunikat informuj�cy, �e zakonczono zapisywanie
			LCD_Clear();
			LCD_SetCursor(0, 0);
			LCD_XYPrintf(0, 0, "Saved");
			DELAY_ms(1000);

			//powr�t
			return;
		}

		//je�eli warto�� akualnej strony jest ni�sza od 0 oznacza to, �e u�ytkownik chce anulowa� zmiany.
		else if (settingsPage < 0) {

			//W tym wypadku ustawiane jest ponownie pod�wietlenie ekranu na tak� intensywno�� jaka by�a przed zmian�
			LCDEXPLOITING_SetLctBrightness(settinsgMenu_values->pwmBrightness);

			//wy�wietlany jest komunikat o anulowaniu zmian przez 1000ms.
			//zmiany nie zostan� wdro�one poniewa� znajduj� si� w tymczasowej zmiennej kt�ra nie zosta�a wykorzystana.
			LCD_Clear();
			LCD_SetCursor(0, 0);
			LCD_XYPrintf(0, 0, "Canceled");
			DELAY_ms(1000);

			//powr�t
			return;
		}

		//W przypadku kiedy numer strony oscyluje pomi�dzy 0 a 100 wy�wietlane s� kolejne strony.
		//Pierwsz� stron� jest strona numer 1 a ostatni� strona numer 6.
		//W tym celu wywo�ana zostaje metoda sprawdzaj�ca czy zosta� wci�ni�ty jaki� przycisk.
		//Wys�ane zostaj� do niej adresy zmiennych przechowuj�ce struktur� z tymczasowymi ustawieniami oraz nuemr strony
		else
			SettingsMenu_pressedButton(&tmpValues, &settingsPage);
	}
}

//------------------------------------------------------------------------
//metoda odpowiedzialna za sprawdzanie oraz wykonywanie akcji po wci�ni�ciu przycisk�w
void SettingsMenu_pressedButton(struct SettingsMenu_values *tmpValues,
		int *currentPage) {

	//deklaracja zmiennych licznika od�wie�ania ekranu wraz z zmienn� informuj�c� przy kt�rym obiegu ma od�wierzy�
	long refreshClock = 0;
	long refreshClockMax = 5000;

	//g��wna p�tla oczekiwania na wykonanie akcji
	while (1) {

		//------------------OBECNIE Standby
		//Je�eli numer strony wynosi 1
		if (*currentPage == 1) {

			//je�eli licznik od�wie�ania jest mniejszy od warto�ci maksymalnej wtedy go zwi�ksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je�eli dojdzie do maksymalnej warto�ci wtedy go zeruje a nastepnie wy�wietla
			//stron� Standby
			else {
				refreshClock = 0;
				SettingsMenu_PrintLabel("Standby"); //wywo�uje metod� wy�wietlania strony z sam� etykiet�
			}

			//Je�eli wci�ni�to przycisk w lewo to ustawia obecn� stron� na maksymaln� warto��.
			//Jest to spowodowane zap�tleniem si� stron(nie ma strony o ni�szym numerze ni� 1, wi�c wskakuje na maksymaln�)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 6;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w prawo to ustawia obecn� stron� na kolejn� warto�� (2)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 2;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk zapisz to ustawia obecn� stron� na dodatni� warto�� poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk anuluj to ustawia obecn� stron� na ujemn� warto�� poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//W przypadku wci�ni�cia przycisku zmiany stanu:
			//dioda powiadomie� zostanie wy��czona, kolejnie pod�wietlenie na warto�� 0 oraz ekran zostanie wyczyszczony
			//kolejnie program poczeka 1000ms zanim b�dzie mo�na przywr�ci� do stanu poprzedniego
			if (BTN_3 || BTN_4) {
				LedControl_OFF(0);
				LCD_Clear();
				LCDEXPLOITING_SetLctBrightness(0);
				DELAY_ms(1000);

				//po wygaszeniu wszystkiego wykonuje si� p�tla w kt�rej znajduje si� jedynie warunek wci�ni�cia dowolnego przycisku
				//Gdy si� nic nie wci�nie p�tla b�dzie wykonywa�a si� ci�gle. Po wci�ni�ciu dowolnego przycisku program wyjdzie z p�tli
				while (1)
					if (BTN_1 || BTN_2 || BTN_3 || BTN_4 || BTN_LEFT
							|| BTN_RIGHT)
						break;

				//kolejnie poczeka 500ms a nastepnie ustawi jadno�� ekranu na poprzedni� warto�� i wy�wietli stron� Standby
				DELAY_ms(500);
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);
				SettingsMenu_PrintLabel("Standby"); //wywo�uje metod� wy�wietlania strony z sam� etykiet�
			}
		}

		//------------------OBECNIE Brightness
		//Je�eli numer strony wynosi 2
		if (*currentPage == 2) {

			//je�eli licznik od�wie�ania jest mniejszy od warto�ci maksymalnej wtedy go zwi�ksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je�eli dojdzie do maksymalnej warto�ci wtedy go zeruje a nastepnie wy�wietla
			//stron� Brightness
			else {
				refreshClock = 0;
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness"); //wywo�uje wy�wietlanie strony z etykiet�, licznikiem oraz podpisami dodaj i odejmnij
			}

			//Je�eli wci�ni�to przycisk w lewo to ustawia obecn� stron� na poprzedni� warto�� (1)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 1;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w prawo to ustawia obecn� stron� na kolejn� warto�� (3)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 3;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w zapisz to ustawia obecn� stron� na dodatni� warto�� poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk anuluj to ustawia obecn� stron� na ujemn� warto�� poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//je�eli wci�ni�to przycisk zmniejszania jasno�ci ekranu oraz jasno�� by�a wi�ksza ni� minimalna to wykonuje poni�szy warunek
			if (BTN_3 && tmpValues->pwmBrightness > 0) {

				//od obecnej jasno�ci odejmuje 10
				tmpValues->pwmBrightness -= 10;

				//kolejnie je�eli warto�� po odj�ciu 10 b�dzie mniejsza od 0 to ustawia 0
				if (tmpValues->pwmBrightness < 0)
					tmpValues->pwmBrightness = 0;

				//koljenie ustawia jasno�� ekranu
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);

				//oraz wy�wietla na ekranie stron� z zmian� jasno�ci
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness"); //wywo�uje wy�wietlanie strony z etykiet�, licznikiem oraz podpisami dodaj i odejmnij
				DELAY_ms(200);
			}

			//je�eli wci�ni�to przycisk zwi�kszania jasno�ci ekranu oraz jasno�� by�a mniejsza ni� maksymalna to wykonuje poni�szy warunek
			if (BTN_4 && tmpValues->pwmBrightness < 100) {

				//dodaje do ebecnej warto�ci jasno�ci 10
				tmpValues->pwmBrightness += 10;

				//kolejnie je�eli warto�� po dodaniu jest wi�ksza ni� 100(maksymalna) to ustawia 100
				if (tmpValues->pwmBrightness > 100)
					tmpValues->pwmBrightness = 100;

				//koljenie ustawia jasno�� ekranu
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);

				//oraz wy�wietla na ekranie stron� z zmian� jasno�ci
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness"); //wywo�uje wy�wietlanie strony z etykiet�, licznikiem oraz podpisami dodaj i odejmnij
				DELAY_ms(200);
			}
		}

		//------------------OBECNIE Led info
		//Je�eli numer strony wynosi 3
		if (*currentPage == 3) {

			//je�eli licznik od�wie�ania jest mniejszy od warto�ci maksymalnej wtedy go zwi�ksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je�eli dojdzie do maksymalnej warto�ci wtedy go zeruje a nastepnie wy�wietla
			//stron� standby
			else {
				refreshClock = 0;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info"); //wywo�uje wy�wietlenie strony zawieraj�cej stykiet�, opcje ON/OFF oraz wypisany obecny stan
			}

			//Je�eli wci�ni�to przycisk w lewo to ustawia obecn� stron� na poprzedni� warto�� (2)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 2;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w prawo to ustawia obecn� stron� na kolejn� warto�� (4)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 4;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w zapisz to ustawia obecn� stron� na dodatni� warto�� poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk anuluj to ustawia obecn� stron� na ujemn� warto�� poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//je�eli wci�ni�to przycisk odpowiedzialny za wy��czenie to ustawia status diody powiadomie� na wy��czony (false)
			//nast�pnie wy�wietla na ekranie stron� z ustawieniem diody powiadomie�
			if (BTN_3) {
				tmpValues->ledStatus = false;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info"); //wywo�uje wy�wietlenie strony zawieraj�cej stykiet�, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}

			//je�eli wci�ni�to przycisk odpowiedzialny za w��czenie to ustawia status diody powiadomie� na w��czony (true)
			//nast�pnie wy�wietla na ekranie stron� z ustawieniem diody powiadomie�
			if (BTN_4) {
				tmpValues->ledStatus = true;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info"); //wywo�uje wy�wietlenie strony zawieraj�cej stykiet�, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}
		}

		//------------------OBECNIE Delay time
		//Je�eli numer strony wynosi 4
		if (*currentPage == 4) {

			//je�eli licznik od�wie�ania jest mniejszy od warto�ci maksymalnej wtedy go zwi�ksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je�eli dojdzie do maksymalnej warto�ci wtedy go zeruje a nastepnie wy�wietla
			//stron� Delay time
			else {
				refreshClock = 0;
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time"); //wywo�uje wy�wietlanie strony z etykiet�, licznikiem oraz podpisami dodaj i odejmnij
			}

			//Je�eli wci�ni�to przycisk w lewo to ustawia obecn� stron� na poprzedni� warto�� (3)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 3;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w prawo to ustawia obecn� stron� na kolejn� warto�� (5)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 5;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w zapisz to ustawia obecn� stron� na dodatni� warto�� poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk anuluj to ustawia obecn� stron� na ujemn� warto�� poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przcysik zmiejszania warto�ci opu�nienia oraz warto�� by�a wi�ksza od minimalnej(50)
			if (BTN_3 && tmpValues->timeDelay > 50) {

				//od obecnej warto�ci odejmuje 25
				tmpValues->timeDelay -= 25;

				//je�eli obecna warto�� po odj�ciu jest mniejsza ni� minimalna(50) to ustawia minimaln�
				if (tmpValues->timeDelay < 50)
					tmpValues->timeDelay = 50;

				//wy�wietla stron� ustawie� czasu opu�nienia
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time"); //wywo�uje wy�wietlanie strony z etykiet�, licznikiem oraz podpisami dodaj i odejmnij
				DELAY_ms(200);
			}

			//Je�eli wci�ni�to przcysik zwi�kszania warto�ci opu�nienia oraz warto�� by�a mniejsza od maksymalnej(250)
			if (BTN_4 && tmpValues->timeDelay < 250) {

				//od obecnej warto�ci dodaje 25
				tmpValues->timeDelay += 25;

				//je�eli obecna warto�� po dodaniu jest wi�ksza ni� maksymalna(250) to ustawia maskymaln�
				if (tmpValues->timeDelay > 250)
					tmpValues->timeDelay = 250;

				//wy�wietla stron� ustawie� czasu opu�nienia
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time"); //wywo�uje wy�wietlanie strony z etykiet�, licznikiem oraz podpisami dodaj i odejmnij
				DELAY_ms(200);
			}
		}

		//------------------OBECNIE Debug mode
		//Je�eli numer strony wynosi 5
		if (*currentPage == 5) {

			//je�eli licznik od�wie�ania jest mniejszy od warto�ci maksymalnej wtedy go zwi�ksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je�eli dojdzie do maksymalnej warto�ci wtedy go zeruje a nastepnie wy�wietla
			//stron� Debug mode
			else {
				refreshClock = 0;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode"); //wywo�uje wy�wietlenie strony zawieraj�cej stykiet�, opcje ON/OFF oraz wypisany obecny stan
			}

			//Je�eli wci�ni�to przycisk w lewo to ustawia obecn� stron� na poprzedni� warto�� (4)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 4;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w prawo to ustawia obecn� stron� na kolejn� warto�� (6)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 6;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w zapisz to ustawia obecn� stron� na dodatni� warto�� poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk anuluj to ustawia obecn� stron� na ujemn� warto�� poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}


			//je�eli wci�ni�to przycisk odpowiedzialny za wy��czenie to ustawia status trybu debugowego na wy��czony (false)
			//nast�pnie wy�wietla na ekranie stron� z ustawieniem trybu debugowego
			if (BTN_3) {
				tmpValues->debugMode = false;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode"); //wywo�uje wy�wietlenie strony zawieraj�cej stykiet�, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}


			//je�eli wci�ni�to przycisk odpowiedzialny za w��czenie to ustawia status trybu debugowego na w��czony (true)
			//nast�pnie wy�wietla na ekranie stron� z ustawieniem trybu debugowego
			if (BTN_4) {
				tmpValues->debugMode = true;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode"); //wywo�uje wy�wietlenie strony zawieraj�cej stykiet�, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}
		}

		//------------------OBECNIE Page number
		//Je�eli numer strony wynosi 6
		if (*currentPage == 6) {

			//je�eli licznik od�wie�ania jest mniejszy od warto�ci maksymalnej wtedy go zwi�ksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je�eli dojdzie do maksymalnej warto�ci wtedy go zeruje a nastepnie wy�wietla
			//stron� standby
			else {
				refreshClock = 0;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number"); //wywo�uje wy�wietlenie strony zawieraj�cej stykiet�, opcje ON/OFF oraz wypisany obecny stan
			}

			//Je�eli wci�ni�to przycisk w lewo to ustawia obecn� stron� na poprzedni� warto�� (5)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 5;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w prawo to ustawia obecn� stron� na minimaln� warto��.
			//Jest to spowodowane zap�tleniem si� stron(nie ma strony o wy�szym numerze ni� 6, wi�c wskakuje na minimaln�)
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 1;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk w zapisz to ustawia obecn� stron� na dodatni� warto�� poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je�eli wci�ni�to przycisk anuluj to ustawia obecn� stron� na ujemn� warto�� poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawie� w nadrz�dnej metodzie
			//nast�pnie czeka ustaion� ilo�� czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//je�eli wci�ni�to przycisk odpowiedzialny za wy��czenie to ustawia status numerowania stron na wy��czony (false)
			//nast�pnie wy�wietla na ekranie stron� z ustawieniem numerowania stron
			if (BTN_3) {
				tmpValues->showChangingPage = false;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number"); //wywo�uje wy�wietlenie strony zawieraj�cej stykiet�, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}

			//je�eli wci�ni�to przycisk odpowiedzialny za w��czenie to ustawia status numerowania stron na w��czony (true)
			//nast�pnie wy�wietla na ekranie stron� z ustawieniem numerowania stron
			if (BTN_4) {
				tmpValues->showChangingPage = true;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number"); //wywo�uje wy�wietlenie strony zawieraj�cej stykiet�, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}
		}
	}
}

//------------------------------------------------------------------------
//metoda do wy�wietlania strony zawieraj�cej opcje wyboru prawda/fa�sz(true/false)
void SettingsMenu_PrintBoolean(bool status, char *label) {

	//najpierw nast�puje czyszczenie ekranu
	LCD_Clear();

	//kolejnie w zale�no�ci od tego czy obecnie jest zmienna aktywna wy�wietlana jest jej etykeita
	//i po ':' stan.
	if (status)
		LCD_XYPrintf(0, 0, "%s: ON", label);
	else
		LCD_XYPrintf(0, 0, "%s: OFF", label);

	//kolejnie wy�wietlane s� podpisy pod przyciski
	LCD_XYPrintf(0, 1, "OFF");
	LCD_XYPrintf(14, 1, "ON");
}

//------------------------------------------------------------------------
//metoda do wy�wietlania strony zawieraj�cej etykiet� oraz warto��
void SettingsMenu_PrintValue(int value, char *label) {

	//najpierw czy�ci ekran
	LCD_Clear();

	//kolejnie wy�wietla etykier� oraz po ':' warto��
	LCD_XYPrintf(0, 0, "%s: %d", label, value);

	//nast�pnie podpisy pod przyciski
	LCD_XYPrintf(0, 1, "-");
	LCD_XYPrintf(15, 1, "+");
}

//------------------------------------------------------------------------
//metoda do wy�wietlania samej etykiety
void SettingsMenu_PrintLabel(char *label) {

	//najpierw nast�puje czyszczenie ekranu
	LCD_Clear();

	//kolejnie wy�wietla etykiet�
	LCD_XYPrintf(0, 0, "%s", label);

	//oraz podpisy pod przyciski
	LCD_XYPrintf(0, 1, "ok");
	LCD_XYPrintf(14, 1, "ok");
}
