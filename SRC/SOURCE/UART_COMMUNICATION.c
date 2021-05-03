/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJE */
#include "../HEADERS/UART_COMMUNICATION.h"

/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJA METOD */

//------------------------------------------------------------------------
//metoda wysyła ciąg znaków po UART a następnie czeka określoną ilość czasu
void UARTCOMMUNICATION_send(const char *str, int time) {
	UART_Printf(str);
	DELAY_ms(time);
}

//------------------------------------------------------------------------
//metoda pobiera po uart ciąg znaków
void UARTCOMMUNICATION_recive(const char *str) {
	UART_RxString(str);
}
