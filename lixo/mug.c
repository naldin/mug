// ATMega328p trabalhando em 1Mhz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <avr/power.h>
#include <avr/sleep.h>

ISR(INT0_vect) {         /* Run every time there is a change on button */
	PORTB |= (1 << PB1);

/*	

  if (bit_is_set(PIND, PD2)) {
    PORTB |= (1 << PB1);
  }
  else {
    PORTB &= ~(1 << PB1);
  }
   */
 
}


void initInterrupt0(void) {
  EIMSK |= (1 << INT0);                                 /* enable INT0 */
  EICRA |= (1 << ISC00);                /* trigger when button changes */
  sei();                          /* set (global) interrupt enable bit */
}


int main (void){
	long i=0;
	int a =0;
	DDRB |= (1 << PB0);
	DDRB |= (1 << PB1);
	PORTD |= (1 << PD2);
	PORTB &= ~(1 << PB1);
	initInterrupt0();
	
	while(1){
		
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
	}
		
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
