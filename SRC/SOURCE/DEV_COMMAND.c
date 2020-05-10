/* CONNECT HEADER */
#include "../HEADERS/DEV_COMMAND.h"

/* BODY OF FUNCTIONS */
void DEVCOMMAND_ExecuteCommand(int location, int page, bool ledStatus, int time) {

	if (location == 1) {
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN1PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN1PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN1PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN1PAGE4, time);
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	} else if (location == 2) {
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN2PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN2PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN2PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN2PAGE4, time);
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	} else if (location == 3) {
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN3PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN3PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN3PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN3PAGE4, time);
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	} else if (location == 4) {
		if (page == 1)
			UARTCOMMUNICATION_send(COMMADN4PAGE1, time);
		else if (page == 2)
			UARTCOMMUNICATION_send(COMMADN4PAGE2, time);
		else if (page == 3)
			UARTCOMMUNICATION_send(COMMADN4PAGE3, time);
		else if (page == 4)
			UARTCOMMUNICATION_send(COMMADN4PAGE4, time);
		DEVCOMMAND_LedInfoBlink(100, ledStatus);
	}
	return;
}

void DEVCOMMAND_LedInfoBlink(int time, bool ledStatus) {
	if (ledStatus) {
		LedControl_ON(100);
		LedControl_OFF(100);
	} else
		DELAY_ms(200);
	return;
}
