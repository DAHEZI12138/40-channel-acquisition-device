#ifndef _LED_H
#define _LED_H
#include "sys.h"
	
#define LED0(n)		  (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET))
#define LED0_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9)) 
#define LED1(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET))
#define LED1_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8)) //Output level inversion
#define LED2(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET))
#define LED2_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10)) 
void LED_Init(void); 

#endif
