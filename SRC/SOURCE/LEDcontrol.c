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
//metoda odpowiada stopniowe w³¹czanie diody powiadomieñ za pomoc¹ PWM.
void LedControl_ON(int time) {

	//pêtla wykonuje siê 100 razy (100 kroków jasnoœci)
	//Pêtla wykonuje siê od wartoœci 0 (dioda zgaszona) do 100 (maksymalnie rozœwiecona)
	for (int clk = 0; clk < 100; clk++) {

		//Nastêpuje ustawienie jasnoœci z wartoœci¹ zegara pêtli
		PWM_SetLedBrightness(clk);

		//Nastêpnie zostaje odczekany czas podzielony przez iloœæ iteracji pêtli.
		//W wyniku tego gdy w metodzie ustawi siê odpowiedni¹ iloœæ ms to ca³a operacja
		//rozjaœniania diody do maksymalnej jasnoœci bêdzie tyle trwa³a
		DELAY_ms(time / 100);
	}
}

//------------------------------------------------------------------------
//metoda odpowiada stopniowe wy³¹czanie diody powiadomieñ za pomoc¹ PWM.
void LedControl_OFF(int time) {

	//pêtla wykonuje siê 100 razy (100 kroków jasnoœci)
	//Pêtla wykonuje siê od wartoœci 100 (maksymalnie rozœwiecona) do 0 (dioda zgaszona)
	for (int clk = 100; clk > 0; clk--) {

		//Nastêpuje ustawienie jasnoœci z wartoœci¹ zegara pêtli
		PWM_SetLedBrightness(clk);

		//Nastêpnie zostaje odczekany czas podzielony przez iloœæ iteracji pêtli.
		//W wyniku tego gdy w metodzie ustawi siê odpowiedni¹ iloœæ ms to ca³a operacja
		//rozjaœniania diody do maksymalnej jasnoœci bêdzie tyle trwa³a
		DELAY_ms(time / 100);
	}
}

//------------------------------------------------------------------------
//Metoda do ustawiania jasnoœci diody powiadomieñ w przedziale od 0 do 100.
void PWM_SetLedBrightness(int val) {

	//gdy wartoœæ ustawiona bêdzie poza zakresem wtedy metoda siê nie wykona
	if (val < 0 || val > 100)
		return;

	//W przyadku obecnego PWM wartoœci¹ 0 jest stan wysoki a pwm dzia³a poprzez zbijanie do stanu niskiego.
	//Aby ustawienie by³o intuicyjne nale¿y wykonaæ inwersje wartoœci w przedziale 0 - 100
	int tmp = 100 - val;

	//nastêpnie zostaje ustawione wype³nienie dla PWM z diod¹ powiadomieñ
	PWM_SetDutyCycle(2, tmp);
}

//------------------------------------------------------------------------
//metoda odpowiadaj¹ca za sterowanie w³¹czeniem diody powiadomieñ.
//W przypadku gdy zmienna state bêdzie mia³a wartoœæ true to wykona operacje w³¹cznia diody przez okreœlon¹ w zmiennej time iloœæ ms
//gdy zmienna state bêdzie posiada³a wartoœæ false to metoda tylko odczeka okreœlon¹ iloœæ ms bez w³¹czania diody powiadomieñ
void LedOn(int time, bool state) {
	if (state)
		LedControl_ON(time);
	else
		DELAY_ms(time);
}

//------------------------------------------------------------------------
//metoda odpowiadaj¹ca za sterowanie wy³¹czeniem diody powiadomieñ.
//W przypadku gdy zmienna state bêdzie mia³a wartoœæ true to wykona operacje wy³¹cznia diody przez okreœlon¹ w zmiennej time iloœæ ms
//gdy zmienna state bêdzie posiada³a wartoœæ false to metoda tylko odczeka okreœlon¹ iloœæ ms bez wy³¹czania diody powiadomieñ
void LedOff(int time, bool state) {
	if (state)
		LedControl_OFF(time);
	else
		DELAY_ms(time);
}
