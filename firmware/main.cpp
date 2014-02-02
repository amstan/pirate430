#include <msp430.h>
#include "portmacros.h"
#include "bitop.h"
#include "USBSerial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pins.h"
#include "spi.c"

#define F_CPU 16000000

///Set multiplier based on the slow xtal
void core_frequency_set(unsigned long int frequency) {
	UCSCTL3 |= SELREF_2;
	UCSCTL4 |= SELA_2;
	UCSCTL0 = 0x0000;
	UCSCTL1 = DCORSEL_5;
	UCSCTL2 = FLLD_1 + (frequency/32768)-1;
}

void chip_init(void) {
	WDTCTL = WDTPW + WDTHOLD; //Stop WDT
	
	//FCPU setting
	P5SEL |= 0x0C; //Change pin modes to xtal
	core_frequency_set(F_CPU);
	
	set_bit(DIR(LED_R_PORT),LED_R);
	set_bit(DIR(LED_G_PORT),LED_G);
}

char getchar(void) {
	while(!USBSerial_available());
	return USBSerial_read();
}

int putchar(int c) {
	int ret=USBSerial_write(c);
	USBSerial_flush(); USBSerial_flush(); //workaround for https://github.com/energia/Energia/issues/320
	return ret;
}

int main(void) {
	chip_init();
	USBSerial_open();
	clear_bit(OUT(LED_G_PORT),LED_G);
	clear_bit(OUT(LED_R_PORT),LED_R);
	
	OUT(CS_PORT)=0x7f; //All except P6.7(connected as a CTS input)
	DIR(CS_PORT)=0x7f;
	spi_init(UCCKPL*1+UCCKPH*0); //Clock idle in a low state, data on rising edge
	
	while(1) {
		switch(getchar()) {
			case 'g': //green led control
				change_bit(OUT(LED_G_PORT),LED_G,getchar()!=0);
				break;
			
			case 'r': //red led control
				change_bit(OUT(LED_R_PORT),LED_R,getchar()!=0);
				break;
			
			case 'e': //echo test
				putchar(getchar());
				break;
				
			case 'c': //chip select
				OUT(CS_PORT)=getchar();
				break;
				
			case 'd': //send/recieve data
				set_bit(OUT(LED_R_PORT),LED_R);
				putchar(spi_send_recieve(getchar()));
				clear_bit(OUT(LED_R_PORT),LED_R);
				break;
				
			case 's': //sync character, ignore
			default:
				break;
		}
	}
}