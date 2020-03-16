#include <avr/io.h>
#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include "main.h"


void setupPins(){
	// buttons
	DDRD ^= ((1 << PIND2) | (1 << PIND3) | (1 << PIND4) | (1 << PIND5));	// inputs
	PORTD |= ((1 << PIND2) | (1 << PIND3) | (1 << PIND4) | (1 << PIND5));	// pull-up	
}

void readButtons(u08 *ptr){
	ptr[BUT0] = 0;
	ptr[BUT0] |= ~PIND & ((1 << PIND5) | (1 << PIND4) | (1 << PIND3) | (1 << PIND2));
	ptr[BUT0] >>= 2;	
}
