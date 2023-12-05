#include "led.h"
	    
//LED IO initialize
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8;			//PB9
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		
    GPIO_Initure.Pull=GPIO_PULLUP;         			
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	//high speed
	  HAL_GPIO_Init(GPIOB,&GPIO_Initure);     		//GPIOB.9
   
	

	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);	//PB10 set to 1 
		
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);	//PB9 set to 1 
	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);	//PB8 set to 1 

}

