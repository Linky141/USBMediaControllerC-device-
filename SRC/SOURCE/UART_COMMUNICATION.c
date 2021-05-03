/*#######################################################################*/
/*#######################################################################*/
/* NAGŁÓWEK */
#include "../HEADERS/UART_COMMUNICATION.h"

/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJA METOD */

//metoda wysyła ciąg znaków po UART a następnie czeka okerśloną ilość czasu
void UARTCOMMUNICATION_send(const char *str, int time) {
		UART_Printf(str);
		DELAY_ms(time);
}
