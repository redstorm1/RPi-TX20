#include "TX20.h"

const char TX20_Directions[16][4] = {{"N"}, {"NNE"}, {"NE"}, {"ENE"}, {"E"}, {"ESE"}, {"SE"}, {"SSE"}, {"S"}, {"SSW"}, {"SW"}, {"WSW"}, {"W"}, {"WNW"}, {"NW"}, {"NNW"}};

void RPi_TX20_InitPins( void ) 
{
	// Set the DATA pin to input
	TX20_DATA_SET_INPUT;
    
}

unsigned char RPi_TX20_GetReading(int *iDir, int *iSpeed )
{
    // Local Variables
	int a = 0;
	int x = 0;
	int datagram[41];
	int start =0;
	int startframe = 0;
	int winddir = 0;
	int winddir2 = 0;
	int windspeed = 0;
	int windspeed2 = 0;
	int checksum = 0;
	unsigned char chk;
    int bitcount=0;
    unsigned int isValidData = 0;
    unsigned int pin;
    
    //Pull DTR low to signal TX20 to send data
    TX20_DTR_SET_OUTPUT_LOW;
    
    // Input should be low right now
    if ( TX20_DATA_GET_BIT != 0 ) 
    {
        printf("Error 1 : Invalid pin state\n");
        //return FALSE;
    }

	//printf("Waiting for pin to go high\n");
	while(isValidData < 1)
	{
		uint8_t value = TX20_DATA_GET_BIT; // Read some data
		//printf("read from pin 15: %d\n", value);
		if (value == 1 && start == 0){
			start = 1;
			a = 0;
		    startframe=winddir=checksum=winddir2=0;
			windspeed=0;windspeed2=0;
			//printf("start of data found\n");    
		}
		if(start == 1){
			for (bitcount=41; bitcount>0; bitcount--) {
				pin = (TX20_DATA_GET_BIT);
    
				if (bitcount > 41-5){
				  // start, inverted
				  startframe = (startframe<<1)|(pin^1);
				} else
				if (bitcount > 41-5-4){
				  // wind dir, inverted
				  winddir = winddir>>1 | ((pin^1)<<3);
				} else
				if (bitcount > 41-5-4-12){
				  // windspeed, inverted
				  windspeed = windspeed>>1 | ((pin^1)<<11);
				} else
				if (bitcount > 41-5-4-12-4){
				  // checksum, inverted
				  checksum = checksum>>1 | ((pin^1)<<3);
				} else 
				if (bitcount > 41-5-4-12-4-4){
				  // wind dir
				  winddir2 = winddir2>>1 | (pin<<3);
				} else {
				  // windspeed
				  windspeed2 = windspeed2>>1 | (pin<<11);
				}
				datagram[a] = pin;
				a++;	
				TX20_DoDelay;
				//printf("a: %d",a);
			}
		}
		//delayMicroseconds(1220);
		TX20_DoDelay;
		//check if we got a valid datagram
		if(a == 41){
			//Calculate Checksum
			chk= ( winddir + (windspeed&0xf) + ((windspeed>>4)&0xf) + ((windspeed>>8)&0xf) );chk&=0xf;
			
			//Decide on the result to return
			if (startframe==4 && winddir==winddir2 && windspeed==windspeed2 && checksum==chk){      
				isValidData=1;
				//printf("is valid set\n");
			} else {
				isValidData=0;
			}			
		}
	}
    
    //return values
    *iDir = winddir;
	*iSpeed = windspeed;
    return isValidData;
}
