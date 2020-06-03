#include <avr/io.h>
#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include "main.h"


void setupPins(){
	// buttons: PORTD - 3 buttons and joystick press, PORTC - single button on Screw Shield
	DDRD &= ~((1 << PIND2) | (1 << PIND3) | (1 << PIND4) | (1 << PIND5));	// inputs
	PORTD |= ((1 << PIND2) | (1 << PIND3) | (1 << PIND4) | (1 << PIND5));	// pull-up
	DDRC &= ~(1 << PINC5);
	PORTC |= ((1 <<PINC5));
		
	DDRD |= ((1 << PIND6));		// D6 output: to bananapi PH2 pin as external INT
	PORTD |= ((1 << PIND6));	// initially high
}

void readButtons(u08 *ptr){
	ptr[BUT0] = 0;
	ptr[BUT0] |= ~PIND & ((1 << PIND5) | (1 << PIND4) | (1 << PIND3) | (1 << PIND2));
	ptr[BUT1] |= ~PINC & (1 << PINC5);
	//ptr[BUT0] >>= 2;	
}
