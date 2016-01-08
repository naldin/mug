
    #README
    	
	License
	=======
	
	Caneca mistura tudo automatica
	Automatic self stirring mug
	Codigo para ATtiny13 e ATmega328p
	Code for ATtiny13 and ATmega328p
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
    
    Description
    ===========
    
    I won a mug that has a motor with little propeller for mix the drink, this 
    motor turn on through a buttom. It works well, but push the buttom every 
    time to mix the hot chocolate was so boring. Then I thought use a 
    microcontroller to control the motor. Using an small ATtiny13A SMD, a 
    trasistor, resistor led and capacitor I made an automatic system for this mug. 
    Do you can see how it works below. The ATtiny used is a 13A, it is important 
    because the working voltage is between 1.8V and 5.5V, as the mug use two AAA 
    battery and the ATtiny without "A" works between 2.7V and 5.5V, with the drop 
    voltage the microcontroller may have problem to work. In the power down mode, the 
    ATtiny13A drain only 0.5uA, what guarantee the life of battery for a long time when 
    it is in power mode. The circuit is simple, do you can use a NPN transistor like BC548 
    or a FET like 2N7000. The capacitors are importante because the motor noise will cause 
    malfunction to microcontroller. I made two codes, one for ATtiny 13A and another for 
    ATmega 328p, but because the small dimension, the ATtiny is the best option.
    
    For change the motor time, modify the values in:
    #define SOMA_TEMPO 137    // Add x to time between turn on
    For led time, modify the value 2 in "led = led + 2;" inside of 
    "ajustaTempos()" function. Each unit sum 0.45 seconds as setted in OCR0A. 
    As the led is setted for turn on and off every entry to the "rodaTempos()", 
    the led will blink every 0.45 second.
    
    Components
    ==========
    - ATtiny 13 or ATmega 328p
    - BC548 transistor or other
    - 1000 ohms Resistor
    - 10uF Capacitor for circuit
    - 100nF Capacitor for noise motor filter
    
    Compiler and burn instructions
    ==============================
    
    Need AVR Toolchain (avrdude and avr-gcc)
    To compile: make inside folder
    To compile and burn: make flash
    If you want only burn the HEX file:
    avrdude -c usbasp -p t13 -U flash:w:mugTiny13.hex:i
    Where usbasp is your ISP programmer and t13 is ATtiny13A
    
    Use Instructions
    ================
    
    - First click, motor always on.
    - Second click, motor turn on every 30 seconds for 5 seconds.
    - Third click, motor turn on every 1 minute for 5 seconds.
    - Fourth click, motor turn on every 1 minute and 30 seconds for 5 seconds.
    - Last click, motor turn on 2 seconds and turn off the system.
    
    Video of operation
    ==================
        
    https://www.youtube.com/watch?v=ScmhuLAGeVc
    
    Blog
    ====
    
    http://xyzturbo.blogspot.com.br/2016/01/caneca-mistura-tudo-automatica.html
