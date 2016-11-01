#ifndef RPI_TX20_H_
#define	RPI_TX20_H_

#include <bcm2835.h> 
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

// Defines
#define	TRUE	1
#define	FALSE	0

// Define the Raspberry Pi GPIO Pins for the TX20
#define TX20DATA RPI_GPIO_P1_11 // Input on RPi pin GPIO 11 physical
#define TX20DTR RPI_GPIO_P1_12 // Input on RPi pin GPIO 12 physical

#define	TX20_DATA_SET_INPUT	        bcm2835_gpio_fsel(TX20DATA, BCM2835_GPIO_FSEL_INPT)// Set RPI pin P1-11 to be an input
#define TX20_DTR_SET_OUTPUT_LOW 	bcm2835_gpio_write(TX20DTR, LOW);\
                                    bcm2835_gpio_fsel(TX20DTR, BCM2835_GPIO_FSEL_OUTP)// Set RPI pin P1-12 to be an Output
#define TX20_DATA_GET_BIT		    bcm2835_gpio_lev(TX20DATA)
#define TX20_DoDelay delayMicroseconds(1220)

extern const char TX20_Directions[16][4];

/* Public Functions ----------------------------------------------------------- */
void RPi_TX20_InitPins( void );
unsigned char RPi_TX20_GetReading(int *iDir, int *iSpeed );

#endif
