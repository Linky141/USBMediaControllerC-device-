/*
 * ConnectionWithChecksum.h
 *
 *  Created on: 8 lut 2019
 *      Author: Tomasz Bielas
 */

#ifndef CONNECTIONWITHCHECKSUM_H_
#define CONNECTIONWITHCHECKSUM_H_

/* IMPLEMENTATIONS */
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../../Libraries/Headers/lcd.h"
#include "../HEADERS/LEDcontrol.h"
#include "../../Libraries/Headers/delay.h"
#include "../../Libraries/Headers/uart.h"



/* FUNCTIONS */
void UARTCOMMUNICATION_send(const char *str, int time);

#endif /* CONNECTIONWITHCHECKSUM_H_ */
