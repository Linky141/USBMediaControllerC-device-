/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJE */
#include <avr\io.h>
#include <stdbool.h>
#include "../../Libraries/Headers/uart.h"
#include "../../Libraries/Headers/delay.h"
#include "../../Libraries/Headers/lcd.h"
#include "../HEADERS/DEV_COMMAND.h"
#include "../HEADERS/LCD_EXPLOITING.h"
#include "../HEADERS/pwm.h"
#include "../../Libraries/Headers/eeprom.h"
#include "../HEADERS/LEDcontrol.h"
#include "../HEADERS/UART_COMMUNICATION.h"
#include "../HEADERS/SettingsMenu.h"

/*#######################################################################*/
/*#######################################################################*/
/* DEFINICJE */

//definicje pinów w porcie B które służą do wykrywania wciśnięć przycisków
#define BTN_RIGHT (!(PINB & 0x2))
#define BTN_LEFT (!(PINB & 0x1))
#define BTN_RL (!(PINB & 0x3))
#define BTN_1 (!(PINB & 0x4))
#define BTN_2 (!(PINB & 0x8))
#define BTN_3 (!(PINB & 0x10))
#define BTN_4 (!(PINB & 0x20))

/*#######################################################################*/
/*#######################################################################*/
/* DEKLARACJE METOD */
void Initialization(struct SettingsMenu_values *settinsgMenu_values, int *page,
		struct LCDEXPLOITING_AllPages *lcdPageLabbles);

void MainLoop(struct SettingsMenu_values *settinsgMenu_values, int *page,
		struct LCDEXPLOITING_AllPages *lcdPageLabbles);

void ShowPage(int time, int page, bool willShow);

/*#######################################################################*/
/*#######################################################################*/
/* GŁÓWNA METODA PROGRAMU */
int main() {

	//Tworzenie struktury do przechowywania etykiet do wszystkich pól wyświetlających się na ekranie
	struct LCDEXPLOITING_AllPages lcdPageLabbles;

	//Tworzenie struktury do przechowywania wartości konfiguracji urządzenia oraz ich adresy w pamięci eeprom
	struct SettingsMenu_values settinsgMenu_values;

	//tworzenie zmiennej definiującej która strona jest obecnie aktywna oraz ustawienie jej wartości na 0 (brak strony)
	int page = 0;

	//Wywołanie metody inicjalizacji urządzenia. Do metody zostają przekazane adresy powyżej zadeklarowanych obiektów.
	Initialization(&settinsgMenu_values, &page, &lcdPageLabbles);

	//Wywołanie metody zawierającą główną pętlę programu. Tutaj również przekazane są adresy wyżej zadeklarowanych oraz wypełnionych obiektów.
	MainLoop(&settinsgMenu_values, &page, &lcdPageLabbles);

	//return (0);
}

/*#######################################################################*/
/*#######################################################################*/
/* METODY */

//------------------------------------------------------------------------
//Metoda inicjalizująca urządzenie. Odpowiada za odczyt z pamięci eeprom ustawień,
//odbiór etykiet z komputera oraz inicjalizację i konfigurację pinów I/O, ekranu LCD
//i UART
void Initialization(struct SettingsMenu_values *settinsgMenu_values, int *page,
		struct LCDEXPLOITING_AllPages *lcdPageLabbles) {

	//------------------INICJALIZACJA UART
	//Inichalizacja UART z baud rate na poziomie 9600 bits-per-second
	UART_Init(9600);

	//------------------INICJALIZACJA LCD
	//Przypisywanie pinów z portu A do obsługi wyświetlacza.
	//PINA0 -> RS, PINA1 -> RW, PINA2 -> EN, PINA3 -> D4, PINA4 -> D5, PINA5 -> D6, PINA6 -> D7
	LCD_SetUp(PA_0, PA_1, PA_2, P_NC, P_NC, P_NC, P_NC, PA_3, PA_4, PA_5, PA_6);

	//Inicjalizacja ekranu z rozdzielczością 2x16 znaków
	LCD_Init(2, 16);

	//------------------INICJALIZACJA PRZYCISKÓW
	//Ustawienie portu B jako wejście
	DDRB = 0x00;

	//Przypisanie portowi B wartość wysoką. Przyciski będą zwierać do masy.
	PORTB = 0xff;

	//------------------INICJALIZACJA PWM DIODY POWIADOMIEŃ
	//Wywołanie metod inicjalizacji oraz startu PWM
	PWM_Init();
	PWM_Start();

	//Wyłączenie diody powiadomień z szybkością 0ms.
	LedControl_OFF(0);

	//------------------ODCZYTYWANIE WARTOŚCI Z EEPROM

	//Tworzenie zmiennej informującej czy urządzenie startuje po raz pierwszy.
	//Jest to potrzebne w celu ustalenia czy zmienne ustawień mają zostać pobrane z pamięci czy
	//zainicjalizowane jako domyślne.
	bool firstStart;

	//Tworzenie zmiennej z adresem w pamięci eeprom który zawiera informacje czy jest to pierwsze
	//uruchomienie urządzenia. Po zaprogramowaniu wszystkie wartości w pamięci mają wartość 1.
	//W efekcie gdy w tej komórce będzie wartość 1 to program wie, że reszta informacji musi
	//być zadeklarowana na nowo(TODO: sprawdzić czy to tak działa na pewno)
	unsigned char eepromAddressFirstStart = 0x01;

	//Odczytywanie informacji czy urządzenie uruchamia się po raz pierwszy z pamięci eeprom
	firstStart = EEPROM_ReadByte(eepromAddressFirstStart);

	//Jeżeli program nie startuje po raz pierwszy będzie odczytywał wartości z pamięci
	if (!firstStart) {
		settinsgMenu_values->eepromAddressBrightness = 0x02;//Przypisywanie adresu w pamięci eeprom dla komórki zawierającej wartość podświetlenia ektanu
		settinsgMenu_values->pwmBrightness = EEPROM_ReadByte(//odczytywanie wartości podświetlenia ekranu z pamięci eeprom
				settinsgMenu_values->eepromAddressBrightness);

		settinsgMenu_values->eepromAddressLedStatus = 0x03;	//Przypisywanie adresu w pamięci eeprom dla komórki zawierającej informacje o aktywności kontrolki powiadomień
		settinsgMenu_values->ledStatus = EEPROM_ReadByte(//odczytywanie z pamięci eeprom informacji o aktywności kontrolki powiadomień
				settinsgMenu_values->eepromAddressLedStatus);

		settinsgMenu_values->eepromAddressTimeDelay = 0x04;	//Przypisywanie adresu w pamięci eeprom dla komórki zawierającej wartość opuźmienia podczas reakcji na przyciski
		settinsgMenu_values->timeDelay = EEPROM_ReadByte(//odczytywanie wartości opuźnienia przycisków z pamięci eeprom
				settinsgMenu_values->eepromAddressTimeDelay);

		settinsgMenu_values->eepromAddressDebugMode = 0x05;	//Przypisywanie adresu w pamięci eeprom dla komórki zawierającej informacje o stanie aktywności trybu debugowego
		settinsgMenu_values->debugMode = EEPROM_ReadByte(//odczytywanie z pamięci eeprom informacji o aktywności trybu debugowania
				settinsgMenu_values->eepromAddressDebugMode);

		settinsgMenu_values->eepromAddressShowChangingPage = 0x06;//Przypisywanie adresu w pamięci eeprom dla komórki zawierającej informacje czy wyświetlać numerowanie stron
		settinsgMenu_values->showChangingPage = EEPROM_ReadByte(//odczytywanie z pamięci eeprom informacji o aktywności wyświetlania numerów stron
				settinsgMenu_values->eepromAddressShowChangingPage);
	}
	//Jeżeli urządzenie zostało włączone po raz pierwszy po zaporogramowaniu
	else {
		LCD_Clear();				//najpierw następuje wyczyszczenie ekranu
		LCD_XYPrintf(0, 0, "* First start  *"); //informacja o pierwszym starcie urządzenia
		LCD_XYPrintf(0, 1, "memory config...");
		settinsgMenu_values->showChangingPage = false; //przypisanie wszystkim zmiennym wartości domyślnych
		settinsgMenu_values->pwmBrightness = 50;
		settinsgMenu_values->ledStatus = true;
		settinsgMenu_values->timeDelay = 100;
		settinsgMenu_values->debugMode = false;
		firstStart = false;	//ustawienie zmiennej informującej o pierwszym uruchomieniu urządzenia na false
		EEPROM_WriteByte(eepromAddressFirstStart, firstStart);//zapisywanie do pamięci eeprom informacji o pierwszym uruchomienu urządzenia
		DELAY_ms(200);
		EEPROM_WriteByte(settinsgMenu_values->eepromAddressBrightness,//zapisywanie do pamięci eeprom informacji o jasności ekranu
				settinsgMenu_values->pwmBrightness);
		DELAY_ms(200);//czekanie 200ms w celu pewności, że operacja zapisu się zakonczyła
		EEPROM_WriteByte(settinsgMenu_values->eepromAddressLedStatus, //zapisywanie do pamięci eeprom informacji o aktywności kontrolki powiadomień
				settinsgMenu_values->ledStatus);
		DELAY_ms(200); //czekanie 200ms w celu pewności, że operacja zapisu się zakonczyła
		EEPROM_WriteByte(settinsgMenu_values->eepromAddressTimeDelay, //zapisywanie do pamięci eeprom informacji o czasie opuźnienia wciskania przycisków
				settinsgMenu_values->timeDelay);
		DELAY_ms(200); //czekanie 200ms w celu pewności, że operacja zapisu się zakonczyła
		EEPROM_WriteByte(settinsgMenu_values->eepromAddressDebugMode, //zapisywanie do pamięci eeprom informacji o aktywności trybu debugowania
				settinsgMenu_values->debugMode);
		DELAY_ms(200); //czekanie 200ms w celu pewności, że operacja zapisu się zakonczyła
		EEPROM_WriteByte(settinsgMenu_values->eepromAddressShowChangingPage, //zapisywanie do pamięci eeprom informacji o aktywności wyświetlania numerów stron
				settinsgMenu_values->showChangingPage);
		DELAY_ms(400);
	}

	//------------------INICJALIZACJA URZĄDZENIA

	//Inicjalizacja specjalnych symboli
	LCD_InitializeSymbols();

	//Wyłączenie kursora
	LCD_CmdWrite(0x0C);

	//Załączanie kontrolki powiadomień w czasie 1000ms. Jeżeli kontrolka jest nieaktywna to czeka 1000ms.
	LedOn(1000, settinsgMenu_values->ledStatus);

	//wyczyść ekran oraz napisz nazwę urządzenia
	LCD_Clear();
	LCD_XYPrintf(3, 0, "USB Media");
	LCD_XYPrintf(3, 1, "Controller");

	//wczytaj ustawioną jasność ekranu w animacji rozjaśniania od 0 do ustalonej wartości
	for (int clk = 0; clk < settinsgMenu_values->pwmBrightness; clk++) {
		LCDEXPLOITING_SetLctBrightness(clk);
		DELAY_ms(20);
	}
	DELAY_ms(200);
	LCD_Clear();

	//ustawienie aktualnej strony na 1
	*page = 1;

	//miganie kontrolką na znak potwierdzenia
	LedOff(100, settinsgMenu_values->ledStatus);
	LedOn(100, settinsgMenu_values->ledStatus);

	//wczytanie etykiet z uart.
	screen_show_initialize(lcdPageLabbles, settinsgMenu_values);

	//Wyświetlanie strony
	LCDEXPLOITING_showPage(*page, lcdPageLabbles);

	//todo: nie pamiętam po co to. Sprawdzić
	PORTB |= (1 << 0);
}

//------------------------------------------------------------------------
//metoda zawierająca główną pętlę programu
void MainLoop(struct SettingsMenu_values *settinsgMenu_values, int *page,
		struct LCDEXPLOITING_AllPages *lcdPageLabbles) {

	//ustawienie czasu przerwy pomiędzy ponowną akcją wciśniętego przycisku na 200ms.
	int time = 200;

	//tworzenie licznika wykonanych akcji podczas trzymania wciśniętego przycisku
	int clkPressedButton = 0;

	//Tworzenie zmiennej licznika cykli do odświeżenia LCD oraz
	//zmiennej określającej po ilu cyklach ma odświeżyć LCD
	long refreshClock = 0;
	long refreshClockMax = 10000;

	//główna pętla programu
	while (1) {

		//Jeżeli został wciśnięty któryś z przycisków zmiany strony
		if (BTN_LEFT || BTN_RIGHT) {

			//Jeżeli wciśnięto przycisk zmiany strony w prawo oraz numer strony będzie mniejszy niż 4
			if (BTN_RIGHT && ((*page) < 4)) {

				//to dioda powiadomień się zaświeci
				LedOn(100, settinsgMenu_values->ledStatus);

				//zwięszy numer strony
				(*page)++;

				//Wyświetli numer strony przez 500ms jeżeli jest ustawiona taka opcja
				ShowPage(500, *page, settinsgMenu_values->showChangingPage);

				//Wyświetli nowo ustawioną stronę na LCD
				LCDEXPLOITING_showPage(*page, lcdPageLabbles);

				//zgasi diodę powiadomień
				LedOff(100, settinsgMenu_values->ledStatus);

				//kolejnie odczeka czas opuźnienia wciśnięcia przycisku pomnożony przez 4
				DELAY_ms(settinsgMenu_values->timeDelay * 4);

				//Jeżeli wciśnięto przycisk zmiany strony w lewo oraz numer strony będzie większy niż 1
			} else if (BTN_LEFT && ((*page) > 1)) {

				//to dioda powiadomień się zaświeci
				LedOn(100, settinsgMenu_values->ledStatus);

				//zmniejszy numer strony	
				(*page)--;

				//Wyświetli numer strony przez 500ms jeżeli jest ustawiona taka opcja
				ShowPage(500, *page, settinsgMenu_values->showChangingPage);

				//Wyświetli nowo ustawioną stronę na LCD
				LCDEXPLOITING_showPage(*page, lcdPageLabbles);

				//zgasi diodę powiadomień
				LedOff(100, settinsgMenu_values->ledStatus);

				//kolejnie odczeka czas opuźnienia wciśnięcia przycisku pomnożony przez 4
				DELAY_ms(settinsgMenu_values->timeDelay * 4);
			}
		}

		//jeżeli zostały wciśnięte oba przyciski zmiany strony, czyli wejście w menu konfiguracyjne
		//Następnie po wyjściu z menu ustawi aktualną stronę na 1 i ją wyświetli
		if (BTN_RL) {
			SettingsMenu_ShowMenu(settinsgMenu_values);
			*page = 1;
			LCDEXPLOITING_showPage(*page, lcdPageLabbles);
		}

		//jeżeli został wciśnięty któryś z przycisków akcji
		if (BTN_1 || BTN_2 || BTN_3 || BTN_4) {

			//Jeżeli wciśnięto przycisk akcji to wykonaj metodę odpowiedzialną za wysłanie polecenia.
			//Wyśyła do metody numer przycsku który został wciśnięty, stronę na której aktualnie
			//się znajduje oraz czas który trzeba odczekać po wykonaniu akcji
			if (BTN_1) {
				DEVCOMMAND_ExecuteCommand(1, *page,
						settinsgMenu_values->ledStatus, time);
			} else if (BTN_2) {
				DEVCOMMAND_ExecuteCommand(2, *page,
						settinsgMenu_values->ledStatus, time);
			} else if (BTN_3) {
				DEVCOMMAND_ExecuteCommand(3, *page,
						settinsgMenu_values->ledStatus, time);
			} else if (BTN_4) {
				DEVCOMMAND_ExecuteCommand(4, *page,
						settinsgMenu_values->ledStatus, time);
			}

			//zwiększ zegar iteracji akcji podczas trzymania wciśniętego przyciska
			clkPressedButton++;

			//Jeżeli już wykonało 3 akcje to zmniejsz czas opuźnienia po wykonaniu akcji do 150ms
			if (clkPressedButton == 3)
				time = 150;

			//Jeżeli już wykonało 5 akcjj to zmniejsz czas opuźnienia po wykonaniu akcji do 100ms
			else if (clkPressedButton == 5)
				time = 100;

			//Jeżeli już wykonało 8 akcji to zmniejsz czas opuźnienia po wykonaniu akcji do 0ms
			else if (clkPressedButton == 8)
				time = 0;
		}

		//jezeli puszczono wszystkie przyciski akcji to ustaw licznik akcji podczas trzymania przycisku 
		//na 0 oraz ustaw czas pomiędzy kolejnymi akcjami na 200ms
		else {
			clkPressedButton = 0;
			time = 200;
		}

		//Jeżeli licznik odświeżania ekranu jest mniejszy niż wartość przy ktorej ma
		//odświeżyć to zwiększa wartość licznika
		if (refreshClock < refreshClockMax)
			refreshClock++;

		//Jeżeli licznik osiągnął wartość przy której musi odświeżyć LCD to ustawia licznik na 0
		//oraz wywołuje metodę wyświetlenia aktualnej strony.
		else {
			refreshClock = 0;
			LCDEXPLOITING_showPage(*page, lcdPageLabbles);
		}
		//LCD_RefreshScreen(&refreshClock, 500);
	}
}

//------------------------------------------------------------------------
//metoda odpowiedialna za wyświetlanie numeru strony podczas zmiany
//Jeżeli ustawiono że ma być wyswietlana strona to zostanie wyświetlona przez określoną ilość czasu
void ShowPage(int time, int page, bool willShow) {
	if (willShow) {
		LCD_Clear();
		LCD_XYPrintf(5, 0, "Page:");
		LCD_XYPrintf(7, 1, "%d", page);
		DELAY_ms(time);
	}
}
