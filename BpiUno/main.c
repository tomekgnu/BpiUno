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
#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include "a2d.h"
#include "spi.h"
#include "main.h"



extern u08 spiTransferComplete;
extern u08 inputValues[6];

int main(int argc, char** argv) {

      
		
    a2dInit();
	a2dSetPrescaler(ADC_PRESCALE_DIV32);
	a2dSetReference(ADC_REFERENCE_AVCC);
	a2dStartConvert();
	setupPins();
	spiInit();
    SPDR = 0xFF;
	
    while(1){
        
		inputValues[ADC0] = a2dConvert8bit(ADC_CH_ADC0);  
		inputValues[ADC1] = a2dConvert8bit(ADC_CH_ADC1);
		inputValues[JOYX] = a2dConvert8bit(ADC_CH_ADC2);
		inputValues[JOYY] = a2dConvert8bit(ADC_CH_ADC3);
		readButtons(inputValues);
		
    }
	
    return (EXIT_SUCCESS);
}

