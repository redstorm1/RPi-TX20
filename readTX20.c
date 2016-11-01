/*
 The main program
 compile with gcc -o readTX20 -l rt readTX20.c TX20.c -l bcm2835
 */

#include "bcm2835.h"
#include <stdio.h>
#include <time.h>
#include "TX20.h"




void printWindSpeedAndDirection(void)
{
	
	// Local variables
	int WindDirection = 0;
	int WindSpeed = 0;
    float kmph = 0.36;
	
		
	// Set up the TX23 Pins
	RPi_TX20_InitPins();

	//Read from the TX20
	if (RPi_TX20_GetReading(&WindDirection,&WindSpeed)==1)
	{
        printf("Wind Direction: %s (%.1f)\n", TX20_Directions[WindDirection],((double)WindDirection)*22.5);
        printf("Wind Speed: %.1fkmph\n", WindSpeed*kmph);
//		printf("(%d , %d )", WindDirection, WindSpeed);
//		printf("%s,Wind Direction,%0.1f\n",thetime,((double)WindDirection)*22.5);
//		printf("%s,Wind Speed,%d\n",thetime,WindSpeed);

	}
	else
	{
		exit(1);
	}
}



int main (int argc, char *argv[])
{
	//Check for debugging flags
	int i;
	int debugMode = 0;
	for (i = 1; i < argc ; i++)
	{
		if ((strcmp(argv[i],"--debug") == 0) || (strcmp(argv[i],"-d") == 0))
			debugMode = 1;
		else if ((strcmp(argv[i],"--help") == 0) || (strcmp(argv[i],"-?") == 0))
		{
			printf("Usage: readTX20 [OPTION]\nRead data from a La Crosse TX20 Anemometer.\n");
			printf("\t-v, --verbose\t\tGive detailed error messages\n");
			printf("\t-d, --debug\t\tShow times and pin state changes only\n");
			printf("\t-?, --help\t\tShow usage information\n\n");
			exit(0);
		}
	}
	
	//Initialise the Raspberry Pi GPIO
	if(!bcm2835_init())
		exit(1);

		printWindSpeedAndDirection();
        
        bcm2835_gpio_write(TX20DTR, HIGH);
        bcm2835_close();

	return 0;
}
