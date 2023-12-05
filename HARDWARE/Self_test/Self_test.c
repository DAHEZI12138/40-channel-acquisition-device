#include "led.h"
#include "delay.h"
#include "Self_test.h"
#include <stdio.h>

//////////////////////////////
//Check the work state of ADS1299
//Use the led to instrcut the chips that didn't initialise properly
//////////////////////////////

int positions[] = {1, 25, 49, 73, 97};
int numPositions = sizeof(positions) / sizeof(positions[0]);


void checkPositions(unsigned char *data4)
{
	 for (int i = 0; i < numPositions; i++) 
	{
		if (i == 0 & data4[positions[i]] != 0xF5) 
			{
			for(int z=1;z<4;z++)
				{
					LED1(0);
					LED0(0);
					delay_ms(500);
					LED1(1);
					LED0(1);
					delay_ms(500);
				} 
				delay_ms(2500);
       }		
		if (i == 1 & data4[positions[i]] != 0xD5) 
			{				
			for(int z=1;z<4;z++)
				{
					for(int j=1;j<3;j++)
					{
						LED1(0);
						LED0(0);
						delay_ms(500);
						LED1(1);
						LED0(1);
						delay_ms(500);
					}
					delay_ms(2500);
				}
			}				
		if (i == 2 & data4[positions[i]] != 0xD5) 
			{				
			for(int z=1;z<4;z++)
				{
					for(int j=1;j<4;j++)
					{
						LED1(0);
						LED0(0);
						delay_ms(500);
						LED1(1);
						LED0(1);
						delay_ms(500);
					}
					delay_ms(2500);
				}	
			}
		if (i == 3 & data4[positions[i]] != 0xD5) 
			{				
			for(int z=1;z<4;z++)
				{
					for(int j=1;j<5;j++)
					{
						LED1(0);
						LED0(0);
						delay_ms(500);
						LED1(1);
						LED0(1);
						delay_ms(500);
					}
					delay_ms(2500);
				}	
			}
		if (i == 4 & data4[positions[i]] != 0xD5) 
			{				
			for(int z=1;z<4;z++)
				{
					for(int j=1;j<6;j++)
					{
						LED1(0);
						LED0(0);
						delay_ms(500);
						LED1(1);
						LED0(1);
						delay_ms(500);
					}
					delay_ms(2500);
				}	
			}
    }
	 LED0(0);	
	}
