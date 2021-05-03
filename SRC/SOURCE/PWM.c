/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJE */

#include "../HEADERS/PWM.h"

/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJA METOD */

//------------------------------------------------------------------------
//inicjalizacja PWM
void PWM_Init() {

	//ustawienie obu pinów jako out(PD5 i PD4)
	DDRD |= (PWM_A | PWM_B);
}

//------------------------------------------------------------------------

void PWM_SetDutyCycle(uint8_t channel, uint8_t dutysCycle) {

	//wyliczanie wype³nienia (do metody dochodzi maksymalna wartoœæ 100. 8 bitowe wype³nienie posiada 255 kroków wiêc
	//wartoœæ zostaje przeskalowana
	dutysCycle *= 2.55;

	//sprawdzanie jaki kana³ PWM ma zostaæ zmieniony
	switch (channel) {

	//w przypadku 1 kana³u ustawia wype³nienie dla OCR1A na wartoœæ wyliczon¹ wy¿ej
	case 1:
		OCR1A = dutysCycle;
		break;

	//w przypadku 1 kana³u ustawia wype³nienie dla OCR1A na wartoœæ wyliczon¹ wy¿ej
	case 2:
		OCR1B = dutysCycle;
		break;
	}
}

//------------------------------------------------------------------------
//uruchamianie PWM
void PWM_Start() {

	//ustaienie w rejestrze TCCR1A bitu WGM10(0) na wartoœæ 1
	//oraz ustaienie w rejestrze TCCR1B bitu WGM12(3) na wartoœæ 1
	//w celu w³¹czenia trybu Fast PWM 8bit
	TCCR1A |= (1 << WGM10);
	TCCR1B |= (1 << WGM12);

	//Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM.
	//Na starcie bêdie stan wysoki a podczas przerwania wyjœcie ustawiane jest na stan niski
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);

	//Preksaler = 64,  fpwm = 976,5Hz
	TCCR1B |= (1 << CS10) | (1 << CS11);

	//ustawienie wype³nienia dla 1 PWM na 0
	OCR1A = 0;

	//ustawienie wype³nienia dla 2 PWM na 0
	OCR1B = 0;
}


