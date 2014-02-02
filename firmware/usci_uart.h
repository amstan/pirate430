#ifndef HW_SERIAL_H
#define HW_SERIAL_H

template<typename T_STORAGE>
struct UCA1 {
	T_STORAGE &_recv_buffer;
	
	inline void init(const unsigned long baud) {
		const unsigned long baud_rate_20_bits = (F_CPU + (baud >> 1))/baud; // Bit rate divisor
		
		UCA1CTL1 |= UCSWRST; // **Put state machine in reset**
		UCA1CTL1 |= UCSSEL_2; // use SMCLK for USCI clock
		UCA1BR0 = (baud_rate_20_bits >> 4) & 0xFF;      // Low byte of whole divisor
		UCA1BR1 = (baud_rate_20_bits >> 12) & 0xFF;     // High byte of whole divisor
		UCA1MCTL = ((baud_rate_20_bits << 4) & 0xF0) | UCOS16;
		UCA1CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
		UCA1IE |= UCRXIE; // Enable USCI_A0 RX interrupt
		
		__delay_cycles(F_CPU/100);
	}
	
	inline bool empty() {
		return _recv_buffer.empty();
	}
	
	inline int recv() {
		while(empty());
		return _recv_buffer.pop_front();
	}
	
	void xmit(unsigned char c) {
		while (!(UCA1IFG & UCTXIFG))
			; // USCI_A0 TX buffer ready?
		
		UCA1TXBUF = (unsigned char) c; // TX -> RXed character
	}
	
	void xmit(const char *s) {
		while (*s) {
			xmit(*s);
			++s;
		}
	}
};

void __attribute__((interrupt (USCI_A1_VECTOR))) USCI0RX_ISR() {
	uart_buffer.push_back(UCA1RXBUF);
}

#endif /* HW_SERIAL_H */
