
    #README
    	
	Licença
	=======
	
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
    
    Descrição
    =========
    
    Pouco tempo atrás ganhei uma caneca mistura tudo, ela possui um motor com uma 
    pequena hélice no fundo, acionado com um botão no cabo. Funciona bem, mas ficar apertando 
    o botão toda hora para misturar o chocolate quente, estava ficando tedioso. Foi então que 
    pensei em usar um micro controlador para comandar o motor automaticamente. Usando um pequeno 
    ATtiny13 SMD que estava parado aqui, um transistor, resistor, led e capacitores de placas 
    usadas que tenho para retirar componentes, fiz um sistema automático. O funcionamento segue 
    abaixo nas instruções de uso. O ATtiny usado é um 13A, este A é importante porque sua tensão 
    de trabalho é de 1,8 a 5,5V, como a caneca usa duas pilhas AAA, o ATtiny sem o final A funciona 
    de 2,7V a 5,5V e com a queda de tensão das pilhas pode-ser ter problemas.
    No modo power down, o ATtiny13 consome apenas 0,5uA de corrente, o que garantirá a vida das 
    pilhas AAA por vários anos, caso a caneca fique desligada (Power Down Mode) não sendo usada.
    
    O circuito é simples, para o transistor pode ser usado qualquer um de uso geral, ou até mesmo um FET. 
    Os capacitores devem ser respeitados devido ao ruído que o motor gera, o que sem eles pode provocar 
    mau funcionamento do micro controlador.
    
    O código além do ATtiny13, fiz também para o ATmega 328p, por ser um micro controlador muito usado, 
    mas devido as dimensões reduzidas do ATtiny13, entendo ser uma melhor opção. Para se alterar os 
    tempos do motor, basta alterar os valores definidos em:
    
    #define SOMA_TEMPO 137    // Soma x ao tempo entre ligadas. Multiplo de ~1/5 segundo
    
    Já para o led, basta alterar o valor do 2 em “led = led + 2” dentro da função "ajustaTempos()", cada 
    unidade soma 0,45 segundo conforme setado em OCR0A. Portanto no 
    primeiro ciclo temporizado teremos 2 que multiplicado por 0,45 será 0,9 / 2 = 0,45 segundo que 
    o led ficará ligado e 0,45 desligado.
    
    Material necessário
    ===================
    
    - ATtiny 13 ou ATmega 328p
    - Transistor de uso geral ex BC548
    - Resistor de 1000k ohms
    - Capacitor 10uF
    - Capacitor de 100nF
    
    Instruções para compilar e gravar
    =================================
    
    Necessário possuir AVR Toolchain instalado (avrdude e avr-gcc).
    Para compilar use make dentro do diretorio dos arquivos.
    Para compilar e já gravar no AVR use make flash.
    Caso deseje apenas gravar o arquivo HEX no AVR, use:
    avrdude -c usbasp -p t13 -U flash:w:mugTiny13.hex:i
    Lembrando de setar a interface correta no lugar de usbasp, caso não seja esta.
    
    Instruções de uso
    =================
    
    - No primeiro clique do botão, o motor e led ligam permanecendo ligados.
    - No próximo clique o led pisca aproximadamente a cada 0,67 segundo e o motor irá ligar a cada 30seg por 5seg.
    - No próximo clique o led pisca aproximadamente a cada 1,1 segundos e o motor irá ligar a cada 1min por 5seg.
    - No próximo clique o led pisca aproximadamente a cada 1,5 segundos e o motor irá ligar a cada 1min e 30seg por 5seg.
    - No próximo clique o motor e led ligam por 2seg e coloca o micro controlador em modo power down (desligado).
    - Mais um clique e o ciclo recomeça.
    
    Video
    =====
    
    youtube
    
    Blog
    ====
    
    xyz
