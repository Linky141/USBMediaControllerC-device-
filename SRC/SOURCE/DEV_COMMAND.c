/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJE */
#include "../HEADERS/DEV_COMMAND.h"

/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJA METOD */

//------------------------------------------------------------------------
//metoda s�u��ca do wysy�ania odpowiednych polece� po UART do komputera w zale�no�ci od aktualnej strony oraz wci�ni�tego przycisku
void DEVCOMMAND_ExecuteCommand(int location, int page, bool ledStatus, int time) {

	//Je�eli wci�ni�to przycisk pod polem numer 1
	if (location == 1) {

		//w zale�no�ci od aktualnej strony wysy�a odpowiedni� komend� po UART wraz z wykonaniem przerwy pomi�dzy akcjami
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN1PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN1PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN1PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN1PAGE4, time);

		//nast�pnie wykonuje akcj� w��czenia i wy��czenia diody powiadomie� w czasie 200ms.
		//Je�eli dioda powiadomie� jest nie aktywna to odczeka 20ms
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	}

	//Je�eli wci�ni�to przycisk pod polem numer 2
	else if (location == 2) {

		//w zale�no�ci od aktualnej strony wysy�a odpowiedni� komend� po UART wraz z wykonaniem przerwy pomi�dzy akcjami
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN2PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN2PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN2PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN2PAGE4, time);

		//nast�pnie wykonuje akcj� w��czenia i wy��czenia diody powiadomie� w czasie 200ms.
		//Je�eli dioda powiadomie� jest nie aktywna to odczeka 20ms
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	}

	//Je�eli wci�ni�to przycisk pod polem numer 3
	else if (location == 3) {

		//w zale�no�ci od aktualnej strony wysy�a odpowiedni� komend� po UART wraz z wykonaniem przerwy pomi�dzy akcjami
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN3PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN3PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN3PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN3PAGE4, time);

		//nast�pnie wykonuje akcj� w��czenia i wy��czenia diody powiadomie� w czasie 200ms.
		//Je�eli dioda powiadomie� jest nie aktywna to odczeka 20ms
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	}

	//Je�eli wci�ni�to przycisk pod polem numer 4
	else if (location == 4) {

		//w zale�no�ci od aktualnej strony wysy�a odpowiedni� komend� po UART wraz z wykonaniem przerwy pomi�dzy akcjami
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN4PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN4PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN4PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN4PAGE4, time);

		//nast�pnie wykonuje akcj� w��czenia i wy��czenia diody powiadomie� w czasie 200ms.
		//Je�eli dioda powiadomie� jest nie aktywna to odczeka 20ms
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	}

	return;
}

//------------------------------------------------------------------------
//metoda wykonuj�ca w��czenie i wy��czenia diody powiadomie� w okre�lonym czasie lub odczekaniu takiej samej
//ilo�ci czasu w przypadku nieaktywnej diody powiadomie�. Ma na celu zasygnalizowa� wykrycie wci�ni�cia
//przycisku oraz wykonan� akcj�
void DEVCOMMAND_LedInfoBlink(int time, bool ledStatus) {
	if (ledStatus) {
		LedControl_ON(time);
		LedControl_OFF(time);
	} else
		DELAY_ms(time*2);
	return;
}
