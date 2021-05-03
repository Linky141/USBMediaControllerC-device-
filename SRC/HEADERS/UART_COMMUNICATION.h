#ifndef CONNECTIONWITHCHECKSUM_H_
#define CONNECTIONWITHCHECKSUM_H_

/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJE */
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../../Libraries/Headers/lcd.h"
#include "../HEADERS/LEDcontrol.h"
#include "../../Libraries/Headers/delay.h"
#include "../../Libraries/Headers/uart.h"

/*#######################################################################*/
/*#######################################################################*/
/* DEKLARACJE METOD */
void UARTCOMMUNICATION_send(const char *str, int time);
void UARTCOMMUNICATION_recive(const char *str);

#endif /* CONNECTIONWITHCHECKSUM_H_ */
