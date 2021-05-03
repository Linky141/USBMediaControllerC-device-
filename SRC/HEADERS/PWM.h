#ifndef _PWM_H_
#define _PWM_H_

/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJE */
#include <avr/io.h>
#include "../../Libraries/Headers/stdutils.h"

/*#######################################################################*/
/*#######################################################################*/
/* DEFINICJE */

//definicje pinów u¿ywanych przy PWM
#define PWM_A (1<<PD5)
#define PWM_B (1<<PD4)

/*#######################################################################*/
/*#######################################################################*/
/* DEKLARACJE METOD */
void PWM_Init();
void PWM_SetDutyCycle(uint8_t channel, uint8_t dutyCycle);
void PWM_Start();

#endif
