/* CONNECT HEADER */
#include "../HEADERS/UART_COMMUNICATION.h"

/* BODY OF FUNCTIONS */
void UARTCOMMUNICATION_send(const char *str, int time) {
		UART_Printf(str);
		DELAY_ms(time);
}
