void spi_init(unsigned int clocksettings) {
	//giveup control of the other pins to the spi hardware
	set_bits(SEL(SPI_PORT),(1<<SCK)|(1<<MOSI)|(1<<MISO));
	
	// Put state machine in reset
	UCB0CTL1 = UCSWRST;
	
	// 3-pin, 8-bit SPI master, MSB
	UCB0CTL0 = UCMST+UCSYNC+UCMSB+clocksettings;
	
	// SMCLK
	UCB0CTL1 = UCSSEL_2;
	
	//No prescaler
	UCB0BR0 = 10;
	UCB0BR1 = 0;
	
	//Start SPI hardware
	clear_bits(UCB0CTL1,UCSWRST);
}

unsigned char spi_send_recieve(unsigned char send) {
	UCB0TXBUF=send;
	
	///wait until it recieves
	while (!(UCB0IFG&UCRXIFG));
	
	return UCB0RXBUF;
}