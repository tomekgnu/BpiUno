/*! \file spi.c \brief SPI interface driver. */
//*****************************************************************************
//
// File Name	: 'spi.c'
// Title		: SPI interface driver
// Author		: Pascal Stang - Copyright (C) 2000-2002
// Created		: 11/22/2000
// Revised		: 06/06/2002
// Version		: 0.6
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "spi.h"
#include "main.h"

// Define the SPI_USEINT key if you want SPI bus operation to be
// interrupt-driven.  The primary reason for not using SPI in
// interrupt-driven mode is if the SPI send/transfer commands
// will be used from within some other interrupt service routine
// or if interrupts might be globally turned off due to of other
// aspects of your program
//
// Comment-out or uncomment this line as necessary

#define SPI_USEINT
// global variables
volatile u08 spiTransferComplete = FALSE;
u08 inputValues[6] = {2,3,1,6,7,4};
u08 inputBytes[3] = {ADC_BYTES,JOY_BYTES,BUT_BYTES};
u08 inputIndex = 0;
inputType currInp = NONE;

SIGNAL(SPI_STC_vect)
{
	u08 byte = SPDR;
	static u08 byteCount = 0;
	
	switch(byte){
		case 1: if(currInp != POT){
					currInp = POT;
					inputIndex = ADC0;
					byteCount = 0;
				}
				break;
		case 2: if(currInp != JOY){
					currInp = JOY;
					inputIndex = JOYX;
					byteCount = 0;
				}
				break;
		case 3: if(currInp != BUT){
					currInp = BUT;
					inputIndex = BUT0;
					byteCount = 0;
				}
				break;
		default: break;
						
	}
	if(currInp == POT || currInp == JOY || currInp == BUT){
		if(byteCount < inputBytes[currInp]){
			SPDR = inputValues[inputIndex];
			inputIndex++;
			byteCount++;
		}
		else{
			SPDR = 0xFF;
			currInp = NONE;
		}
	}
	else
		SPDR = 0xFF;
	
	spiTransferComplete = TRUE;
	
}


// init SPI in slave mode
void spiInit()
{

    // setup SPI I/O pins
    sbi(PORTB, 5);  // set SCK hi
    cbi(DDRB, 5);   // set SCK as input
    sbi(DDRB, 4);   // set MISO as output
    cbi(DDRB, 3);   // set MOSI as input
    cbi(DDRB, 2);   // SS must be input for slave mode to work
	
	// setup SPI interface :
	cbi(SPCR, SPR0);
	sbi(SPCR, SPR1);
	// select clock phase positive-going in middle of data
	cbi(SPCR, CPOL);
	cbi(SPCR,CPHA);
	// Data order MSB first
	cbi(SPCR,DORD);
	// enable SPI with interrupt
	sbi(SPCR, SPE);
	sbi(SPCR, SPIE);	
	// some other possible configs
	//outp((1<<MSTR)|(1<<SPE)|(1<<SPR0), SPCR );
	//outp((1<<CPHA)|(1<<CPOL)|(1<<MSTR)|(1<<SPE)|(1<<SPR0)|(1<<SPR1), SPCR );
	//outp((1<<CPHA)|(1<<MSTR)|(1<<SPE)|(1<<SPR0), SPCR );
	
	// clear status
	inb(SPSR);
	spiTransferComplete = TRUE;
}
/*
void spiSetBitrate(u08 spr)
{
	outb(SPCR, (inb(SPCR) & ((1<<SPR0)|(1<<SPR1))) | (spr&((1<<SPR0)|(1<<SPR1)))));
}
*/
void spiSendByte(u08 data)
{
	// send a byte over SPI and ignore reply
	#ifdef SPI_USEINT
		while(!spiTransferComplete);
		spiTransferComplete = FALSE;
	#else
		while(!(inb(SPSR) & (1<<SPIF)));
	#endif

	outb(SPDR, data);
}

u08 spiTransferByte(u08 data)
{
	#ifdef SPI_USEINT
	// send the given data
	spiTransferComplete = FALSE;
	outb(SPDR, data);
	// wait for transfer to complete
	while(!spiTransferComplete);
	#else
	// send the given data
	outb(SPDR, data);
	// wait for transfer to complete
	while(!(inb(SPSR) & (1<<SPIF)));
	#endif
	// return the received data
	return inb(SPDR);
}

u16 spiTransferWord(u16 data)
{
	u16 rxData = 0;

	// send MS byte of given data
	rxData = (spiTransferByte((data>>8) & 0x00FF))<<8;
	// send LS byte of given data
	rxData |= (spiTransferByte(data & 0x00FF));

	// return the received data
	return rxData;
}
