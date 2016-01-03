/*
	Caneca mistura tudo automatica
	Codigo para ATtiny13 e ATmega 328p
    Copyright (C) 2015  Ronaldo Rezende Junior (naldin.net at gmail)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
 */

// ATtiny13 trabalhando em 1.2Mhz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>

#define MOTOR_ON 5000     // Tempo que o motor permanece ligado em ms
#define SOMA_TEMPO 137    // Soma x ao tempo entre ligadas. Multiplo de ~1/5 segundo
#define TEMPO_DEBOUNCE 10 // Em ms
#define PORT_DDR DDRB
#define PORT_EST PORTB
#define PORT_LED PB2
#define PORT_MOTOR PB3
#define BOTAO PB1

// Variaveis
volatile int contLed = 1;    // Contador de tempo do led. Cada ciclo são 1/2 segundo
volatile int contMotor = 1;  // Contador de tempo do motor. Cada ciclo são 1/2 segundo
int led = 0;                 // Comparador de tempo do led
int motor = 0;               // Comparador de tempo do motor
int inicia = 0;              // Usado para no inicio do ciclo deixar o motor sempre ligado

ISR(TIM0_COMPA_vect) {       // Interrupção CTC
	contLed++;               // Somador
	contMotor++;             // Somador
}

void iniciaTimer(void){
TCCR0A |= (1 << WGM01);              // Modo CTC
TCCR0A |= (1 << COM0A0);             // Modo alternado
TCCR0B |= (1 << CS00) | (1 << CS02); // Prescaler 1024
TIMSK0 |= (1 << OCIE0A);             // Habilita interrupcao de saida por comparacao (CTC)
}

ISR(INT0_vect) {     // Interrupção de porta
// Usado somente para acordar do sleep
}

void iniciaPortas(void){
	PORT_DDR |= (1 << PORT_LED);    // Porta para Led
	PORT_DDR |= (1 << PORT_MOTOR);  // Porta para Motor
	PORTB |= (1 << BOTAO);          // Pull-up no Botao (INT0)
}

void iniciaInterrupt0(void) {
    GIMSK |= (1 << INT0);      // Habilita interrupção em INT0
    sei();                     // Habilita interrupções
}

void modoSleep(void){      // Desliga AVR
	PORT_EST |= (1 << PORT_LED);
	PORT_EST |= (1 << PORT_MOTOR);
	_delay_ms(2000);
	PORT_EST &= ~(1 << PORT_LED);
	PORT_EST &= ~(1 << PORT_MOTOR);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
}

void ligaMotor(void){      // Liga motor e led no tempo definido em MOTOR_ON
	PORT_EST |= (1 << PORT_LED);
	PORT_EST |= (1 << PORT_MOTOR);
	_delay_ms(MOTOR_ON);
    PORT_EST &= ~(1 << PORT_MOTOR);
}

void rodaTempos(void){     // Roda os tempos para led e motor usando contador dentro de interrupção CTC
	if (contLed == led){
		PORT_EST ^= (1 << PORT_LED);
		contLed = 0;
	}
	if (contMotor == motor){
		ligaMotor();
		contMotor = 0;
		contLed = 0;
	}
}

void ajustaTempos(void){    // Altera os tempos do led e motor pelo botao
	if (bit_is_clear(PINB,BOTAO)){
		_delay_ms(TEMPO_DEBOUNCE);       // Usado para Debounce
		if (bit_is_set(PINB,BOTAO)) {   // Usado para Debounce
			if (inicia == 0){           // Inicializa com led e motor ligados
				PORT_EST |= (1 << PORT_LED);
				PORT_EST |= (1 << PORT_MOTOR);
				inicia = 1;
			}
			else {                    // Soma o tempo a cada apertada do botao
				PORT_EST &= ~(1 << PORT_MOTOR);
				led = led + 2;
				motor = motor + SOMA_TEMPO;
				contLed = 0;
				contMotor = 0;
				if (led == 8){        // Tres tempos até entrar novamente no modo sleep
					led = 0;
					contLed = 0;
					motor = 0;
					contMotor = 0;
					inicia = 0;
					modoSleep();
				}
			}
		}
	}
}

int main (void){
	iniciaTimer();
	iniciaInterrupt0();
	iniciaPortas();
	modoSleep();
	
	while(1){
		OCR0A = 255;   // Ajusta timer para ciclo de ~1/2.2 segundos
		rodaTempos();
		ajustaTempos();	
	}
}
