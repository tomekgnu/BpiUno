/*
 * BpiUno.c
 *
 * Created: 02.03.2020 22:16:37
 * Author : Tomek
 */ 

/* 
 * File:   newmain.c
 * Author: Tomek
 *
 * Created on 29 lutego 2020, 18:23
 */
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include "a2d.h"
#include "spi.h"
#include "main.h"

static void sendInterruptSignal();

u08 inputValues[6];
u08 inputOldValues[6] = {0,0,0,0,0,0};

int main(int argc, char** argv) {

    a2dInit();
	a2dSetPrescaler(ADC_PRESCALE_DIV32);
	a2dSetReference(ADC_REFERENCE_AVCC);
	a2dStartConvert();
	setupPins();
	spiInit();
	
    SPDR = 0xFF;
	
   inputOldValues[ADC0] = a2dConvert8bit(ADC_CH_ADC0);
   inputOldValues[ADC1] = a2dConvert8bit(ADC_CH_ADC1);
   inputOldValues[JOYX] = a2dConvert8bit(ADC_CH_ADC2);
   inputOldValues[JOYY] = a2dConvert8bit(ADC_CH_ADC3);
   readButtons(inputOldValues);
   
    while(1){
        u08 index;
		inputValues[ADC0] = a2dConvert8bit(ADC_CH_ADC0);  
		inputValues[ADC1] = a2dConvert8bit(ADC_CH_ADC1);
		inputValues[JOYX] = a2dConvert8bit(ADC_CH_ADC2);
		inputValues[JOYY] = a2dConvert8bit(ADC_CH_ADC3);
		readButtons(inputValues);
		
		for(index = 0; index < 4; index++){
			if(inputValues[index] != inputOldValues[index]){
				inputOldValues[index] = inputValues[index];
				sendInterruptSignal();
			}
			if(inputValues[BUT0] > 0){
				//inputOldValues[BUT0] = inputValues[BUT0];
				sendInterruptSignal();
				inputValues[BUT0] = 0;
			}
			
			
			if(inputValues[BUT1] > 0){
				//inputOldValues[BUT1] = inputValues[BUT1];
				sendInterruptSignal();
				inputValues[BUT1] = 0;
			}
		}
		
    }
	
    return (EXIT_SUCCESS);
}

static void sendInterruptSignal(){
	PORTD &= ~(1 << PIND6);	
	_delay_ms(250);
	PORTD |= ((1 << PIND6));
}