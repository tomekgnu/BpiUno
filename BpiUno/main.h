/*
 * main.h
 *
 * Created: 14.03.2020 17:10:24
 *  Author: Tomek
 */ 


#ifndef MAIN_H_
#define MAIN_H_

// input type enum
typedef enum {POT,JOY,BUT,NONE} inputType;
	
// how many bytes each input uses
#define ADC_BYTES	2
#define JOY_BYTES	2
#define BUT_BYTES	2

// indexes in array with ADC/JOY/BUT values
#define ADC0	0
#define ADC1	1
#define JOYX	2
#define JOYY	3
#define BUT0	4
#define BUT1	5

void setupPins();
void readButtons(u08 *ptr);

#endif /* MAIN_H_ */