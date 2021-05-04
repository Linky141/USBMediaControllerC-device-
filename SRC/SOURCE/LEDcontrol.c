/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJE */

#include "../HEADERS/LEDcontrol.h"
#include "../HEADERS/PWM.h"
#include "../../Libraries/Headers/delay.h"


/*#######################################################################*/
/*#######################################################################*/
/* IMPLEMENTACJA METOD */

//------------------------------------------------------------------------
//metoda odpowiada stopniowe w��czanie diody powiadomie� za pomoc� PWM.
void LedControl_ON(int time) {

	//p�tla wykonuje si� 100 razy (100 krok�w jasno�ci)
	//P�tla wykonuje si� od warto�ci 0 (dioda zgaszona) do 100 (maksymalnie roz�wiecona)
	for (int clk = 0; clk < 100; clk++) {

		//Nast�puje ustawienie jasno�ci z warto�ci� zegara p�tli
		PWM_SetLedBrightness(clk);

		//Nast�pnie zostaje odczekany czas podzielony przez ilo�� iteracji p�tli.
		//W wyniku tego gdy w metodzie ustawi si� odpowiedni� ilo�� ms to ca�a operacja
		//rozja�niania diody do maksymalnej jasno�ci b�dzie tyle trwa�a
		DELAY_ms(time / 100);
	}
}

//------------------------------------------------------------------------
//metoda odpowiada stopniowe wy��czanie diody powiadomie� za pomoc� PWM.
void LedControl_OFF(int time) {

	//p�tla wykonuje si� 100 razy (100 krok�w jasno�ci)
	//P�tla wykonuje si� od warto�ci 100 (maksymalnie roz�wiecona) do 0 (dioda zgaszona)
	for (int clk = 100; clk > 0; clk--) {

		//Nast�puje ustawienie jasno�ci z warto�ci� zegara p�tli
		PWM_SetLedBrightness(clk);

		//Nast�pnie zostaje odczekany czas podzielony przez ilo�� iteracji p�tli.
		//W wyniku tego gdy w metodzie ustawi si� odpowiedni� ilo�� ms to ca�a operacja
		//rozja�niania diody do maksymalnej jasno�ci b�dzie tyle trwa�a
		DELAY_ms(time / 100);
	}
}

//------------------------------------------------------------------------
//Metoda do ustawiania jasno�ci diody powiadomie� w przedziale od 0 do 100.
void PWM_SetLedBrightness(int val) {

	//gdy warto�� ustawiona b�dzie poza zakresem wtedy metoda si� nie wykona
	if (val < 0 || val > 100)
		return;

	//W przyadku obecnego PWM warto�ci� 0 jest stan wysoki a pwm dzia�a poprzez zbijanie do stanu niskiego.
	//Aby ustawienie by�o intuicyjne nale�y wykona� inwersje warto�ci w przedziale 0 - 100
	int tmp = 100 - val;

	//nast�pnie zostaje ustawione wype�nienie dla PWM z diod� powiadomie�
	PWM_SetDutyCycle(2, tmp);
}

//------------------------------------------------------------------------
//metoda odpowiadaj�ca za sterowanie w��czeniem diody powiadomie�.
//W przypadku gdy zmienna state b�dzie mia�a warto�� true to wykona operacje w��cznia diody przez okre�lon� w zmiennej time ilo�� ms
//gdy zmienna state b�dzie posiada�a warto�� false to metoda tylko odczeka okre�lon� ilo�� ms bez w��czania diody powiadomie�
void LedOn(int time, bool state) {
	if (state)
		LedControl_ON(time);
	else
		DELAY_ms(time);
}

//------------------------------------------------------------------------
//metoda odpowiadaj�ca za sterowanie wy��czeniem diody powiadomie�.
//W przypadku gdy zmienna state b�dzie mia�a warto�� true to wykona operacje wy��cznia diody przez okre�lon� w zmiennej time ilo�� ms
//gdy zmienna state b�dzie posiada�a warto�� false to metoda tylko odczeka okre�lon� ilo�� ms bez wy��czania diody powiadomie�
void LedOff(int time, bool state) {
	if (state)
		LedControl_OFF(time);
	else
		DELAY_ms(time);
}
