/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJE */
#include "../HEADERS/DEV_COMMAND.h"

/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJA METOD */

//------------------------------------------------------------------------
//metoda s³u¿¹ca do wysy³ania odpowiednych poleceñ po UART do komputera w zale¿noœci od aktualnej strony oraz wciœniêtego przycisku
void DEVCOMMAND_ExecuteCommand(int location, int page, bool ledStatus, int time) {

	//Je¿eli wciœniêto przycisk pod polem numer 1
	if (location == 1) {

		//w zale¿noœci od aktualnej strony wysy³a odpowiedni¹ komendê po UART wraz z wykonaniem przerwy pomiêdzy akcjami
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN1PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN1PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN1PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN1PAGE4, time);

		//nastêpnie wykonuje akcjê w³¹czenia i wy³¹czenia diody powiadomieñ w czasie 200ms.
		//Je¿eli dioda powiadomieñ jest nie aktywna to odczeka 20ms
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	}

	//Je¿eli wciœniêto przycisk pod polem numer 2
	else if (location == 2) {

		//w zale¿noœci od aktualnej strony wysy³a odpowiedni¹ komendê po UART wraz z wykonaniem przerwy pomiêdzy akcjami
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN2PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN2PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN2PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN2PAGE4, time);

		//nastêpnie wykonuje akcjê w³¹czenia i wy³¹czenia diody powiadomieñ w czasie 200ms.
		//Je¿eli dioda powiadomieñ jest nie aktywna to odczeka 20ms
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	}

	//Je¿eli wciœniêto przycisk pod polem numer 3
	else if (location == 3) {

		//w zale¿noœci od aktualnej strony wysy³a odpowiedni¹ komendê po UART wraz z wykonaniem przerwy pomiêdzy akcjami
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN3PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN3PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN3PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN3PAGE4, time);

		//nastêpnie wykonuje akcjê w³¹czenia i wy³¹czenia diody powiadomieñ w czasie 200ms.
		//Je¿eli dioda powiadomieñ jest nie aktywna to odczeka 20ms
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	}

	//Je¿eli wciœniêto przycisk pod polem numer 4
	else if (location == 4) {

		//w zale¿noœci od aktualnej strony wysy³a odpowiedni¹ komendê po UART wraz z wykonaniem przerwy pomiêdzy akcjami
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN4PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN4PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN4PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN4PAGE4, time);

		//nastêpnie wykonuje akcjê w³¹czenia i wy³¹czenia diody powiadomieñ w czasie 200ms.
		//Je¿eli dioda powiadomieñ jest nie aktywna to odczeka 20ms
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	}

	return;
}

//------------------------------------------------------------------------
//metoda wykonuj¹ca w³¹czenie i wy³¹czenia diody powiadomieñ w okreœlonym czasie lub odczekaniu takiej samej
//iloœci czasu w przypadku nieaktywnej diody powiadomieñ. Ma na celu zasygnalizowaæ wykrycie wciœniêcia
//przycisku oraz wykonan¹ akcjê
void DEVCOMMAND_LedInfoBlink(int time, bool ledStatus) {
	if (ledStatus) {
		LedControl_ON(time);
		LedControl_OFF(time);
	} else
		DELAY_ms(time*2);
	return;
}
