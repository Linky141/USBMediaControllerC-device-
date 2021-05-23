#include "../HEADERS/UART_COMMUNICATION.h"

void UARTCOMMUNICATION_send(const char *str, int time) {
	UART_Printf(str);
	DELAY_ms(time);
}

void UARTCOMMUNICATION_recive(const char *str) {
	UART_RxString(str);
}
