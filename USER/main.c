#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h" 
#include "usbd_cdc_interface.h"
#include "ads1299.h"
#include "spi.h"
#include "Self_test.h"
#include "mpu.h"


USBD_HandleTypeDef USBD_Device;
extern PCD_HandleTypeDef hpcd;
extern vu8 bDeviceState;			// USB connection situation

unsigned char *data_4 = {0};

unsigned char data_5[] = "Decrypted\r\n";  // communication with host
unsigned char data_6[] = "DB_enter\r\n";
unsigned char data_7[] = "DB_exit\r\n";
unsigned char data_8[] = "EPVM_OK\r\n";
unsigned char data_9[] = "XPVM_OK\r\n";

unsigned char code_Passnum[9]={'D','e','t','e','c','t'};
unsigned char exit_measure[15]={'E','x','i','t',' ','m','e','a','s','u','r','e'};
unsigned char enter_polar_measure[7]={'E','P','V','M'}; //Enter polar vol measure
unsigned char exit_polar_measure[7]={'X','P','V','M'}; //Exit polar vol measure	
unsigned char noise_test[13] = {'N','o','i','s','e',' ','t','e','s','t'};	 //Enter noise test
unsigned char Common_mode_measure[22] = {'C','o','m','m','o','n',' ','m','o','d','e',' ','m','e','a','s','u','r','e'};	//Common mode measure	
unsigned char Differential_mode_measure[28] = {'D','i','f','f','e','r','e','n','t','i','a','l',' ','m','o','d','e',' ','m','e','a','s','u','r','e'};	// Differential mode measure
unsigned char Square_wave_self_test[9] = {'D','B','_','e','n','a'};	// Square wave
unsigned char Quit_Square_wave_self_test[9] = {'D','B','_','d','i','s'};	// Quit Square wave

	
int main(void)
{
	   
	Cache_Enable();                			//open L1-Cache
	MPU_Memory_Protection();        		//protect related storage area
	HAL_Init();				        					//initialise hal library
	Stm32_Clock_Init(160,5,2,4);  		  //set clock
	delay_init(400);										//initialise delay
	LED_Init();													//initialise led
	
	
	delay_ms(500);
	delay_ms(500);
	

	USBD_Init(&USBD_Device,&VCP_Desc,0);
	USBD_RegisterClass(&USBD_Device,USBD_CDC_CLASS);
	USBD_CDC_RegisterInterface(&USBD_Device,&USBD_CDC_fops);
	USBD_Start(&USBD_Device);
	HAL_PWREx_EnableUSBVoltageDetector();
	
	delay_ms(10);
	data_4 = ADS1299_Init();            //initialise ADS1299
	delay_ms(100);
		
	checkPositions(data_4);             //check the work state of ADS1299
	
 	while(1)
	{			
		if(USB_USART_RX_STA&0x8000)       //if receive the commend, stop the acquisition process
		{
		  ADS1299_STOP();	
			LED1(1);											  //extinguish working status led
			USB_USART_RX_STA=0;
			
			if(strcmp((char*)USB_USART_RX_BUF,(char*)code_Passnum)==0) //validate the password
			{	
				VCP_DataTx(data_5, 11);
				memset(USB_USART_RX_BUF, 0, sizeof USB_USART_RX_BUF);
				DRDY_Init();
			}
			
			if(USB_USART_RX_BUF[0] == 0x30) // suspend the acquisition process
			{
				
			}
			if(USB_USART_RX_BUF[0] == 0x31) // start the acquisition process
			{
				LED1(0);
				ADS1299_START();
				
			}
			if(USB_USART_RX_BUF[0] == 0x32) // return register information
			{
				VCP_DataTx(data_4, 120);
			}	
								
			if (strcmp((char*)USB_USART_RX_BUF,(char*)exit_measure)==0) //exit the performence measure
				
			{
				memset(USB_USART_RX_BUF, 0, sizeof USB_USART_RX_BUF);	
				delay_ms(10);
				data_4 = ADS1299_exit_measure();
				delay_ms(10);
			}
						
			if (strcmp((char*)USB_USART_RX_BUF,(char*)enter_polar_measure)==0) // PGA gain set to 12
			{
				VCP_DataTx(data_8, 9);
				memset(USB_USART_RX_BUF, 0, sizeof USB_USART_RX_BUF);
				delay_ms(10);
				data_4 = ADS1299_Polar_vol();
				delay_ms(10);

			}
			if (strcmp((char*)USB_USART_RX_BUF,(char*)exit_polar_measure)==0) 
			{
				VCP_DataTx(data_9, 9);
				memset(USB_USART_RX_BUF, 0, sizeof USB_USART_RX_BUF);
				delay_ms(10);
				data_4 = ADS1299_Polar_vol_exit();			
				delay_ms(10);

			}
					
			if (strcmp((char*)USB_USART_RX_BUF,(char*)noise_test)==0)  // start noise test
			{
				memset(USB_USART_RX_BUF, 0, sizeof USB_USART_RX_BUF);
				delay_ms(10);
				data_4 = ADS1299_noise_test();
				delay_ms(10);			
			}
			
								
			if (strcmp((char*)USB_USART_RX_BUF,(char*)Common_mode_measure)==0) // Common mode
			{
				memset(USB_USART_RX_BUF, 0, sizeof USB_USART_RX_BUF);
				delay_ms(10);
				data_4 = ADS1299_Common_mode_measure();
				delay_ms(10);

			}
			if (strcmp((char*)USB_USART_RX_BUF,(char*)Differential_mode_measure)==0) // differential mode
			{
				memset(USB_USART_RX_BUF, 0, sizeof USB_USART_RX_BUF);
				delay_ms(10);
				data_4 = ADS1299_Differential_mode_measure();
				delay_ms(10);

			}
			if(strcmp((char*)USB_USART_RX_BUF,(char*)Square_wave_self_test)==0) // generate square wave
			{
				VCP_DataTx(data_6, 10);
				memset(USB_USART_RX_BUF, 0, sizeof USB_USART_RX_BUF);
				delay_ms(10);
				change_test(1);
				delay_ms(10);
				
			}
			if(strcmp((char*)USB_USART_RX_BUF,(char*)Quit_Square_wave_self_test)==0)
			{
				VCP_DataTx(data_7, 10);
				memset(USB_USART_RX_BUF, 0, sizeof USB_USART_RX_BUF);
				delay_ms(10);
				change_test(0);
				delay_ms(10);
				
			}
		}
	
			
	}
}
