// ATMega328p trabalhando em 1Mhz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <avr/power.h>
#include <avr/sleep.h>

volatile int contLed = 0;
volatile int contMotor = 0;
volatile int motor = 1;
volatile int led = 1;

ISR(TIMER0_COMPA_vect) {     // Interrupção CTC
	contLed++;
 
}

void iniciaTimer(void){
TCCR0A |= (1 << WGM01);              // Modo CTC
TCCR0A |= (1 << COM0A0);             // Modo alternado
TCCR0B |= (1 << CS00) | (1 << CS02); // Prescaler 1024
TIMSK0 |= (1 << OCIE0A);                // habilita interrupcao de saida por comparacao
DDRD |= (1 << PD6);
}

ISR(INT0_vect) {     // Interrupção de porta
	if (PIND == (1 << PD2)){
		_delay_us(100);             // Debounce
		if (PIND == (1 << PD2)){    // Debounce
			led = led + 3;
			motor = motor + 3;
			if (motor == 10){
				motor = 1;
				contMotor = 0;
			}
			if (led == 10){
				led = 1;
				contLed = 0;
			}
		}
	}
}

void iniciaInterrupt0(void) {
  EIMSK |= (1 << INT0);                                 /* enable INT0 */
  EICRA |= (1 << ISC00);                /* trigger when button changes */
  sei();                          /* set (global) interrupt enable bit */
}

int main (void){
	iniciaTimer();
	DDRB |= (1 << PB0);    // Led
	DDRB |= (1 << PB1);    // Motor
	DDRD &= ~(1 << PD6);
	PORTD |= (1 << PD2);
//	PORTB &= ~(1 << PB1);
	iniciaInterrupt0();
	
	while(1){
		OCR0A = 243;
		if (contLed == led){
			PORTB ^= (1 << PB0);
			contLed = 0;
		}
		if (contMotor == motor){
			PORTB ^= (1 << PB1);
			contMotor = 0;
		}
		/*
		if (i==50000){  // 100000 = um por segundo
			PORTB ^= (1 << PB0);
			i=0;
			a++;
		}
		 
		i++;
		
		if (a == 10){
			PORTB &= ~(1 << PB1);
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
			a=0;
	}*/
	
	/*	
		if (PIND == (1 << PD2)){
			PORTB |= (1 << PB1);
		}
		else if (PIND != (1 << PD2)){
			PORTB &= ~(1 << PB1);
		}
	*/
}
}
