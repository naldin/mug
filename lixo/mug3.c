// ATMega328p trabalhando em 1Mhz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>

#define MOTOR_ON 300      // Tempo que o motor permanece ligado em ms
#define TEMPO_INICIAL 120 // Tempo entre ligadas inical do motor. Multiplo de 1/4 segundo
#define SOMA_TEMPO 120    // Soma x ao tempo entre ligadas. Multiplo de 1/4 segundo
#define TEMPO_DEBOUCE 10  // Em ms
#define PORT_DDR DDRB
#define PORT_DIR PORTB
#define PORT_LED
#define PORT_MOTOR

// Variaveis
volatile int contLed = 1;    // Contador de tempo do led. Cada ciclo são 1/2 segundos
volatile int contMotor = 1;  // Contador de tempo do motor. Cada ciclo são 1/2 segundos
int led = 0;                 // Comparador de tempo do led
int motor = 0;               // Comparador de tempo do motor.
int inicia = 0;              // Usado para no inicio do ciclo o motor ficar deixar o motor sempre ligado
                             // Funcao ajustaTempos

ISR(TIMER0_COMPA_vect) {     // Interrupção CTC
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
	DDRB |= (1 << PB0);    // Porta para Led
	DDRB |= (1 << PB1);    // Porta para Motor
	PORTD |= (1 << PD2);   // Pull-up em INT0
}

void iniciaInterrupt0(void) {
  EIMSK |= (1 << INT0);      // Habilita interrupção INT0
  EICRA |= (1 << ISC00);     // Interrupção por mudança de estado em INT0
  sei();                     // Habilita interrupções
}

void modoSleep(void){      // Desliga AVR
	PORTB |= (1 << PB0);
	_delay_ms(2000);
	PORTB &= ~(1 << PB0);
	PORTB &= ~(1 << PB1);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
}

void ligaMotor(void){      // Liga motor e led no tempo desejado em MOTOR_ON
	PORTB |= (1 << PB0);
	PORTB |= (1 << PB1);
	_delay_ms(MOTOR_ON);
    PORTB &= ~(1 << PB1);
}

void rodaTempos(void){     // Roda os tempos para led e motor usando contador dentro de interrupção CTC
	if (contLed == led){
		PORTB ^= (1 << PB0);
		contLed = 0;
	}
	if (contMotor == motor){
		ligaMotor();
		contMotor = 0;
		contLed = 0;
	}
}

void ajustaTempos(void){    // Altera os tempos do led e motor
	if (bit_is_clear(PIND,PD2)){
		_delay_ms(TEMPO_DEBOUCE);     // Usado para Debounce
		if (bit_is_set(PIND,PD2)) {   // Usado para Debounce
			if (inicia == 0){         // Inicializa com led e motor ligados
				PORTB |= (1 << PB0);
				PORTB |= (1 << PB1);
				inicia = 1;
			}
			else {                    // Soma o tempo a cada apertada do botão
				PORTB &= ~(1 << PB1);
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
		OCR0A = 243;   // Ajusta timer para ciclo de ~1/2 segundo
		rodaTempos();
		ajustaTempos();	
	}
}
