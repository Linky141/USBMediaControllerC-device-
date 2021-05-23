#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <util/delay.h>
#include <stdbool.h>
#include "../HEADERS/LEDcontrol.h"
#include "../HEADERS/UART_COMMUNICATION.h"
#include "../../Libraries/Headers/uart.h"

#define COMMADN1PAGE1 "aa11\n"
#define COMMADN2PAGE1 "aa22\n"
#define COMMADN3PAGE1 "aa33\n"
#define COMMADN4PAGE1 "aa44\n"

#define COMMADN1PAGE2 "bb11\n"
#define COMMADN2PAGE2 "bb22\n"
#define COMMADN3PAGE2 "bb33\n"
#define COMMADN4PAGE2 "bb44\n"

#define COMMADN1PAGE3 "cc11\n"
#define COMMADN2PAGE3 "cc22\n"
#define COMMADN3PAGE3 "cc33\n"
#define COMMADN4PAGE3 "cc44\n"

#define COMMADN1PAGE4 "dd11\n"
#define COMMADN2PAGE4 "dd22\n"
#define COMMADN3PAGE4 "dd33\n"
#define COMMADN4PAGE4 "dd44\n"

void DEVCOMMAND_ExecuteCommand(int location, int page, bool ledStatus, int time);
void DEVCOMMAND_LedInfoBlink(int time, bool ledStatus);

#endif /* COMMANDS_H_ */
