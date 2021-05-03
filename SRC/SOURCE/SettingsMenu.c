/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJE */

#include "../HEADERS/SettingsMenu.h"

/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJA METOD */

//------------------------------------------------------------------------
//Wyœwietlanie menu konfiguracyjnego urz¹denia.
//Do metody wysy³ane s¹ wartoœci ustawieñ wraz z adresami
void SettingsMenu_ShowMenu(struct SettingsMenu_values *settinsgMenu_values) {

	//tworzenie tymczasowej struktury z wybranymi ustawieniami w celu prostego odrzucenia zmian w razie takiej potrzeby
	struct SettingsMenu_values tmpValues;

	//przypisanie do tymczasowego obiektu parametrów zgodnych z aktualnym stanem
	tmpValues.pwmBrightness = settinsgMenu_values->pwmBrightness;
	tmpValues.timeDelay = settinsgMenu_values->timeDelay;
	tmpValues.ledStatus = settinsgMenu_values->ledStatus;
	tmpValues.debugMode = settinsgMenu_values->debugMode;
	tmpValues.showChangingPage = settinsgMenu_values->showChangingPage;

	//ostawienie numeru strony w ustawieniach na 1
	int settingsPage = 1;

	//przez 2000ms na ekranie zostaje wyœwietlone znaczenie klawiszy w tym menu
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

	//pêtla g³ówna ustawieñ
	while (1) {

		//je¿eli wartoœæ numeru strony jest wiêksza ni¿ 100 oznacza to, ¿e u¿ytkonik chce zapisaæ obecne ustawienia
		if (settingsPage > 100) {

			//Pierw czyœci ekran i wyœwietla komunikat o zapisie
			LCD_Clear();
			LCD_XYPrintf(0, 0, "Saving...");

			//do zmiennych u¿ytkowych przypisywane s¹ nowe wartoœci przetrzymywane w zmiennych tymczasowych
			settinsgMenu_values->pwmBrightness = tmpValues.pwmBrightness;
			settinsgMenu_values->timeDelay = tmpValues.timeDelay;
			settinsgMenu_values->ledStatus = tmpValues.ledStatus;
			settinsgMenu_values->debugMode = tmpValues.debugMode;
			settinsgMenu_values->showChangingPage = tmpValues.showChangingPage;

			//kolejnie wartoœci s¹ zapisywane do eepromu pod odpowiednie adresy z zachowaniem odstêpu 200ms pomiêdzy ka¿dym zapisem
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

			//kolejnie przez 1000ms wyœwietla siê komunikat informuj¹cy, ¿e zakonczono zapisywanie
			LCD_Clear();
			LCD_SetCursor(0, 0);
			LCD_XYPrintf(0, 0, "Saved");
			DELAY_ms(1000);

			//powrót
			return;
		}

		//je¿eli wartoœæ akualnej strony jest ni¿sza od 0 oznacza to, ¿e u¿ytkownik chce anulowaæ zmiany.
		else if (settingsPage < 0) {

			//W tym wypadku ustawiane jest ponownie podœwietlenie ekranu na tak¹ intensywnoœæ jaka by³a przed zmian¹
			LCDEXPLOITING_SetLctBrightness(settinsgMenu_values->pwmBrightness);

			//wyœwietlany jest komunikat o anulowaniu zmian przez 1000ms.
			//zmiany nie zostan¹ wdro¿one poniewa¿ znajduj¹ siê w tymczasowej zmiennej która nie zosta³a wykorzystana.
			LCD_Clear();
			LCD_SetCursor(0, 0);
			LCD_XYPrintf(0, 0, "Canceled");
			DELAY_ms(1000);

			//powrót
			return;
		}

		//W przypadku kiedy numer strony oscyluje pomiêdzy 0 a 100 wyœwietlane s¹ kolejne strony.
		//Pierwsz¹ stron¹ jest strona numer 1 a ostatni¹ strona numer 6.
		//W tym celu wywo³ana zostaje metoda sprawdzaj¹ca czy zosta³ wciœniêty jakiœ przycisk.
		//Wys³ane zostaj¹ do niej adresy zmiennych przechowuj¹ce strukturê z tymczasowymi ustawieniami oraz nuemr strony
		else
			SettingsMenu_pressedButton(&tmpValues, &settingsPage);
	}
}

//------------------------------------------------------------------------
//metoda odpowiedzialna za sprawdzanie oraz wykonywanie akcji po wciœniêciu przycisków
void SettingsMenu_pressedButton(struct SettingsMenu_values *tmpValues,
		int *currentPage) {

	//deklaracja zmiennych licznika odœwie¿ania ekranu wraz z zmienn¹ informuj¹c¹ przy którym obiegu ma odœwierzyæ
	long refreshClock = 0;
	long refreshClockMax = 5000;

	//g³ówna pêtla oczekiwania na wykonanie akcji
	while (1) {

		//------------------OBECNIE Standby
		//Je¿eli numer strony wynosi 1
		if (*currentPage == 1) {

			//je¿eli licznik odœwie¿ania jest mniejszy od wartoœci maksymalnej wtedy go zwiêksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je¿eli dojdzie do maksymalnej wartoœci wtedy go zeruje a nastepnie wyœwietla
			//stronê Standby
			else {
				refreshClock = 0;
				SettingsMenu_PrintLabel("Standby"); //wywo³uje metodê wyœwietlania strony z sam¹ etykiet¹
			}

			//Je¿eli wciœniêto przycisk w lewo to ustawia obecn¹ stronê na maksymaln¹ wartoœæ.
			//Jest to spowodowane zapêtleniem siê stron(nie ma strony o ni¿szym numerze ni¿ 1, wiêc wskakuje na maksymaln¹)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 6;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w prawo to ustawia obecn¹ stronê na kolejn¹ wartoœæ (2)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 2;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk zapisz to ustawia obecn¹ stronê na dodatni¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk anuluj to ustawia obecn¹ stronê na ujemn¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//W przypadku wciœniêcia przycisku zmiany stanu:
			//dioda powiadomieñ zostanie wy³¹czona, kolejnie podœwietlenie na wartoœæ 0 oraz ekran zostanie wyczyszczony
			//kolejnie program poczeka 1000ms zanim bêdzie mo¿na przywróciæ do stanu poprzedniego
			if (BTN_3 || BTN_4) {
				LedControl_OFF(0);
				LCD_Clear();
				LCDEXPLOITING_SetLctBrightness(0);
				DELAY_ms(1000);

				//po wygaszeniu wszystkiego wykonuje siê pêtla w której znajduje siê jedynie warunek wciœniêcia dowolnego przycisku
				//Gdy siê nic nie wciœnie pêtla bêdzie wykonywa³a siê ci¹gle. Po wciœniêciu dowolnego przycisku program wyjdzie z pêtli
				while (1)
					if (BTN_1 || BTN_2 || BTN_3 || BTN_4 || BTN_LEFT
							|| BTN_RIGHT)
						break;

				//kolejnie poczeka 500ms a nastepnie ustawi jadnoœæ ekranu na poprzedni¹ wartoœæ i wyœwietli stronê Standby
				DELAY_ms(500);
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);
				SettingsMenu_PrintLabel("Standby"); //wywo³uje metodê wyœwietlania strony z sam¹ etykiet¹
			}
		}

		//------------------OBECNIE Brightness
		//Je¿eli numer strony wynosi 2
		if (*currentPage == 2) {

			//je¿eli licznik odœwie¿ania jest mniejszy od wartoœci maksymalnej wtedy go zwiêksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je¿eli dojdzie do maksymalnej wartoœci wtedy go zeruje a nastepnie wyœwietla
			//stronê Brightness
			else {
				refreshClock = 0;
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness"); //wywo³uje wyœwietlanie strony z etykiet¹, licznikiem oraz podpisami dodaj i odejmnij
			}

			//Je¿eli wciœniêto przycisk w lewo to ustawia obecn¹ stronê na poprzedni¹ wartoœæ (1)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 1;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w prawo to ustawia obecn¹ stronê na kolejn¹ wartoœæ (3)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 3;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w zapisz to ustawia obecn¹ stronê na dodatni¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk anuluj to ustawia obecn¹ stronê na ujemn¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//je¿eli wciœniêto przycisk zmniejszania jasnoœci ekranu oraz jasnoœæ by³a wiêksza ni¿ minimalna to wykonuje poni¿szy warunek
			if (BTN_3 && tmpValues->pwmBrightness > 0) {

				//od obecnej jasnoœci odejmuje 10
				tmpValues->pwmBrightness -= 10;

				//kolejnie je¿eli wartoœæ po odjêciu 10 bêdzie mniejsza od 0 to ustawia 0
				if (tmpValues->pwmBrightness < 0)
					tmpValues->pwmBrightness = 0;

				//koljenie ustawia jasnoœæ ekranu
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);

				//oraz wyœwietla na ekranie stronê z zmian¹ jasnoœci
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness"); //wywo³uje wyœwietlanie strony z etykiet¹, licznikiem oraz podpisami dodaj i odejmnij
				DELAY_ms(200);
			}

			//je¿eli wciœniêto przycisk zwiêkszania jasnoœci ekranu oraz jasnoœæ by³a mniejsza ni¿ maksymalna to wykonuje poni¿szy warunek
			if (BTN_4 && tmpValues->pwmBrightness < 100) {

				//dodaje do ebecnej wartoœci jasnoœci 10
				tmpValues->pwmBrightness += 10;

				//kolejnie je¿eli wartoœæ po dodaniu jest wiêksza ni¿ 100(maksymalna) to ustawia 100
				if (tmpValues->pwmBrightness > 100)
					tmpValues->pwmBrightness = 100;

				//koljenie ustawia jasnoœæ ekranu
				LCDEXPLOITING_SetLctBrightness(tmpValues->pwmBrightness);

				//oraz wyœwietla na ekranie stronê z zmian¹ jasnoœci
				SettingsMenu_PrintValue(tmpValues->pwmBrightness, "Brightness"); //wywo³uje wyœwietlanie strony z etykiet¹, licznikiem oraz podpisami dodaj i odejmnij
				DELAY_ms(200);
			}
		}

		//------------------OBECNIE Led info
		//Je¿eli numer strony wynosi 3
		if (*currentPage == 3) {

			//je¿eli licznik odœwie¿ania jest mniejszy od wartoœci maksymalnej wtedy go zwiêksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je¿eli dojdzie do maksymalnej wartoœci wtedy go zeruje a nastepnie wyœwietla
			//stronê standby
			else {
				refreshClock = 0;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info"); //wywo³uje wyœwietlenie strony zawieraj¹cej stykietê, opcje ON/OFF oraz wypisany obecny stan
			}

			//Je¿eli wciœniêto przycisk w lewo to ustawia obecn¹ stronê na poprzedni¹ wartoœæ (2)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 2;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w prawo to ustawia obecn¹ stronê na kolejn¹ wartoœæ (4)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 4;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w zapisz to ustawia obecn¹ stronê na dodatni¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk anuluj to ustawia obecn¹ stronê na ujemn¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//je¿eli wciœniêto przycisk odpowiedzialny za wy³¹czenie to ustawia status diody powiadomieñ na wy³¹czony (false)
			//nastêpnie wyœwietla na ekranie stronê z ustawieniem diody powiadomieñ
			if (BTN_3) {
				tmpValues->ledStatus = false;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info"); //wywo³uje wyœwietlenie strony zawieraj¹cej stykietê, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}

			//je¿eli wciœniêto przycisk odpowiedzialny za w³¹czenie to ustawia status diody powiadomieñ na w³¹czony (true)
			//nastêpnie wyœwietla na ekranie stronê z ustawieniem diody powiadomieñ
			if (BTN_4) {
				tmpValues->ledStatus = true;
				SettingsMenu_PrintBoolean(tmpValues->ledStatus, "Led info"); //wywo³uje wyœwietlenie strony zawieraj¹cej stykietê, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}
		}

		//------------------OBECNIE Delay time
		//Je¿eli numer strony wynosi 4
		if (*currentPage == 4) {

			//je¿eli licznik odœwie¿ania jest mniejszy od wartoœci maksymalnej wtedy go zwiêksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je¿eli dojdzie do maksymalnej wartoœci wtedy go zeruje a nastepnie wyœwietla
			//stronê Delay time
			else {
				refreshClock = 0;
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time"); //wywo³uje wyœwietlanie strony z etykiet¹, licznikiem oraz podpisami dodaj i odejmnij
			}

			//Je¿eli wciœniêto przycisk w lewo to ustawia obecn¹ stronê na poprzedni¹ wartoœæ (3)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 3;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w prawo to ustawia obecn¹ stronê na kolejn¹ wartoœæ (5)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 5;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w zapisz to ustawia obecn¹ stronê na dodatni¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk anuluj to ustawia obecn¹ stronê na ujemn¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przcysik zmiejszania wartoœci opuŸnienia oraz wartoœæ by³a wiêksza od minimalnej(50)
			if (BTN_3 && tmpValues->timeDelay > 50) {

				//od obecnej wartoœci odejmuje 25
				tmpValues->timeDelay -= 25;

				//je¿eli obecna wartoœæ po odjêciu jest mniejsza ni¿ minimalna(50) to ustawia minimaln¹
				if (tmpValues->timeDelay < 50)
					tmpValues->timeDelay = 50;

				//wyœwietla stronê ustawieñ czasu opuŸnienia
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time"); //wywo³uje wyœwietlanie strony z etykiet¹, licznikiem oraz podpisami dodaj i odejmnij
				DELAY_ms(200);
			}

			//Je¿eli wciœniêto przcysik zwiêkszania wartoœci opuŸnienia oraz wartoœæ by³a mniejsza od maksymalnej(250)
			if (BTN_4 && tmpValues->timeDelay < 250) {

				//od obecnej wartoœci dodaje 25
				tmpValues->timeDelay += 25;

				//je¿eli obecna wartoœæ po dodaniu jest wiêksza ni¿ maksymalna(250) to ustawia maskymaln¹
				if (tmpValues->timeDelay > 250)
					tmpValues->timeDelay = 250;

				//wyœwietla stronê ustawieñ czasu opuŸnienia
				SettingsMenu_PrintValue(tmpValues->timeDelay, "Delay time"); //wywo³uje wyœwietlanie strony z etykiet¹, licznikiem oraz podpisami dodaj i odejmnij
				DELAY_ms(200);
			}
		}

		//------------------OBECNIE Debug mode
		//Je¿eli numer strony wynosi 5
		if (*currentPage == 5) {

			//je¿eli licznik odœwie¿ania jest mniejszy od wartoœci maksymalnej wtedy go zwiêksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je¿eli dojdzie do maksymalnej wartoœci wtedy go zeruje a nastepnie wyœwietla
			//stronê Debug mode
			else {
				refreshClock = 0;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode"); //wywo³uje wyœwietlenie strony zawieraj¹cej stykietê, opcje ON/OFF oraz wypisany obecny stan
			}

			//Je¿eli wciœniêto przycisk w lewo to ustawia obecn¹ stronê na poprzedni¹ wartoœæ (4)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 4;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w prawo to ustawia obecn¹ stronê na kolejn¹ wartoœæ (6)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 6;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w zapisz to ustawia obecn¹ stronê na dodatni¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk anuluj to ustawia obecn¹ stronê na ujemn¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}


			//je¿eli wciœniêto przycisk odpowiedzialny za wy³¹czenie to ustawia status trybu debugowego na wy³¹czony (false)
			//nastêpnie wyœwietla na ekranie stronê z ustawieniem trybu debugowego
			if (BTN_3) {
				tmpValues->debugMode = false;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode"); //wywo³uje wyœwietlenie strony zawieraj¹cej stykietê, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}


			//je¿eli wciœniêto przycisk odpowiedzialny za w³¹czenie to ustawia status trybu debugowego na w³¹czony (true)
			//nastêpnie wyœwietla na ekranie stronê z ustawieniem trybu debugowego
			if (BTN_4) {
				tmpValues->debugMode = true;
				SettingsMenu_PrintBoolean(tmpValues->debugMode, "Debug mode"); //wywo³uje wyœwietlenie strony zawieraj¹cej stykietê, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}
		}

		//------------------OBECNIE Page number
		//Je¿eli numer strony wynosi 6
		if (*currentPage == 6) {

			//je¿eli licznik odœwie¿ania jest mniejszy od wartoœci maksymalnej wtedy go zwiêksza
			if (refreshClock < refreshClockMax)
				refreshClock++;

			//Je¿eli dojdzie do maksymalnej wartoœci wtedy go zeruje a nastepnie wyœwietla
			//stronê standby
			else {
				refreshClock = 0;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number"); //wywo³uje wyœwietlenie strony zawieraj¹cej stykietê, opcje ON/OFF oraz wypisany obecny stan
			}

			//Je¿eli wciœniêto przycisk w lewo to ustawia obecn¹ stronê na poprzedni¹ wartoœæ (5)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_LEFT) {
				*currentPage = 5;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w prawo to ustawia obecn¹ stronê na minimaln¹ wartoœæ.
			//Jest to spowodowane zapêtleniem siê stron(nie ma strony o wy¿szym numerze ni¿ 6, wiêc wskakuje na minimaln¹)
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_RIGHT) {
				*currentPage = 1;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk w zapisz to ustawia obecn¹ stronê na dodatni¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to zapisanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_1) {
				*currentPage = 150;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//Je¿eli wciœniêto przycisk anuluj to ustawia obecn¹ stronê na ujemn¹ wartoœæ poza zakresem przewijania stron.
			//Spowoduje to anulowanie ustawieñ w nadrzêdnej metodzie
			//nastêpnie czeka ustaion¹ iloœæ czasu w ms*4 i wychodzi z metody
			if (BTN_2) {
				*currentPage = -10;
				DELAY_ms(tmpValues->timeDelay * 4);
				return;
			}

			//je¿eli wciœniêto przycisk odpowiedzialny za wy³¹czenie to ustawia status numerowania stron na wy³¹czony (false)
			//nastêpnie wyœwietla na ekranie stronê z ustawieniem numerowania stron
			if (BTN_3) {
				tmpValues->showChangingPage = false;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number"); //wywo³uje wyœwietlenie strony zawieraj¹cej stykietê, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}

			//je¿eli wciœniêto przycisk odpowiedzialny za w³¹czenie to ustawia status numerowania stron na w³¹czony (true)
			//nastêpnie wyœwietla na ekranie stronê z ustawieniem numerowania stron
			if (BTN_4) {
				tmpValues->showChangingPage = true;
				SettingsMenu_PrintBoolean(tmpValues->showChangingPage,
						"Page number"); //wywo³uje wyœwietlenie strony zawieraj¹cej stykietê, opcje ON/OFF oraz wypisany obecny stan
				DELAY_ms(200);
			}
		}
	}
}

//------------------------------------------------------------------------
//metoda do wyœwietlania strony zawieraj¹cej opcje wyboru prawda/fa³sz(true/false)
void SettingsMenu_PrintBoolean(bool status, char *label) {

	//najpierw nastêpuje czyszczenie ekranu
	LCD_Clear();

	//kolejnie w zale¿noœci od tego czy obecnie jest zmienna aktywna wyœwietlana jest jej etykeita
	//i po ':' stan.
	if (status)
		LCD_XYPrintf(0, 0, "%s: ON", label);
	else
		LCD_XYPrintf(0, 0, "%s: OFF", label);

	//kolejnie wyœwietlane s¹ podpisy pod przyciski
	LCD_XYPrintf(0, 1, "OFF");
	LCD_XYPrintf(14, 1, "ON");
}

//------------------------------------------------------------------------
//metoda do wyœwietlania strony zawieraj¹cej etykietê oraz wartoœæ
void SettingsMenu_PrintValue(int value, char *label) {

	//najpierw czyœci ekran
	LCD_Clear();

	//kolejnie wyœwietla etykierê oraz po ':' wartoœæ
	LCD_XYPrintf(0, 0, "%s: %d", label, value);

	//nastêpnie podpisy pod przyciski
	LCD_XYPrintf(0, 1, "-");
	LCD_XYPrintf(15, 1, "+");
}

//------------------------------------------------------------------------
//metoda do wyœwietlania samej etykiety
void SettingsMenu_PrintLabel(char *label) {

	//najpierw nastêpuje czyszczenie ekranu
	LCD_Clear();

	//kolejnie wyœwietla etykietê
	LCD_XYPrintf(0, 0, "%s", label);

	//oraz podpisy pod przyciski
	LCD_XYPrintf(0, 1, "ok");
	LCD_XYPrintf(14, 1, "ok");
}
