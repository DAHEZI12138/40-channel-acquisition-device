#include "ADS1299.h"
#include "spi.h"
#include "delay.h"
#include "malloc.h"
#include "stm32h7xx_hal_gpio.h"
#include "led.h"
#include "math.h"
#include "sys.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h" 
#include "usbd_cdc_interface.h"
#include "crc.h"

#define len 960

extern SPI_HandleTypeDef hspi1;
int i, j, k, stat;
int transport = 0;	
int test = 0;
u8 bytedata_1;
u8 bytedata_2;
u8 checksum = 0; 
u8 bytedata_2_receive[961] = {0}; 
u8 bytedata_2_tempreceive_1[27] = {0};
u8 bytedata_2_tempreceive_2[27] = {0};
u8 bytedata_2_tempreceive_3[27] = {0};
u8 bytedata_2_tempreceive_4[27] = {0};
u8 bytedata_2_tempreceive_5[27] = {0};
int state = 0;
unsigned char data_1[120] = {0};
unsigned char *data_2 = {0};
unsigned char *data_3 = {0};
int q;
uint8_t send[27] = {0};




//////////////////////////////////////////////////////////////
//initialize the chip
//////////////////////////////////////////////////////////////


// initial start pin
void START_Init()
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOH_CLK_ENABLE();					

	GPIO_Initure.Pin=START_Pin;			            //PH9
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		
	GPIO_Initure.Pull=GPIO_PULLDOWN;         	    
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  
	HAL_GPIO_Init(START_GPIO_Port,&GPIO_Initure);   
	HAL_GPIO_WritePin(START_GPIO_Port,START_Pin,GPIO_PIN_RESET);	//SET 0
}

// initial reset pin
void RESET_Init()
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOH_CLK_ENABLE();					

	GPIO_Initure.Pin=RESET_Pin;			            //PH10
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		
	GPIO_Initure.Pull=GPIO_PULLDOWN;         		
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	
	HAL_GPIO_Init(RESET_GPIO_Port,&GPIO_Initure);   
	HAL_GPIO_WritePin(RESET_GPIO_Port,RESET_Pin,GPIO_PIN_RESET);	//set 0
}

// initial chip select
void CS_Init()
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOA_CLK_ENABLE();					//start GPIOA clock
	

	GPIO_Initure.Pin=CS1_Pin|CS2_Pin|CS4_Pin;			  //PA6,5,4
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		    
	GPIO_Initure.Pull=GPIO_PULLDOWN;         		    
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	
	HAL_GPIO_Init(CS1_GPIO_Port,&GPIO_Initure);   
	
	__HAL_RCC_GPIOH_CLK_ENABLE();					//start GPIOH clock
	
	GPIO_Initure.Pin=CS3_Pin|CS5_Pin;			//PH6,8
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		
	GPIO_Initure.Pull=GPIO_PULLDOWN;         		
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	
	HAL_GPIO_Init(CS3_GPIO_Port,&GPIO_Initure);     
	
	HAL_GPIO_WritePin(CS1_GPIO_Port,CS1_Pin,GPIO_PIN_SET);	//PA6 set to 1 
	HAL_GPIO_WritePin(CS2_GPIO_Port,CS2_Pin,GPIO_PIN_SET);	//PA5 set to 1 
	HAL_GPIO_WritePin(CS3_GPIO_Port,CS3_Pin,GPIO_PIN_SET);	//PH6 set to 1 
	HAL_GPIO_WritePin(CS4_GPIO_Port,CS4_Pin,GPIO_PIN_SET);	//PA4 set to 1 
	HAL_GPIO_WritePin(CS5_GPIO_Port,CS5_Pin,GPIO_PIN_SET);	//PH8 set to 1 
}

//initial DRDY
void DRDY_Init()
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOD_CLK_ENABLE();					//start GPIOD clock

	GPIO_Initure.Pin=DRDY_Pin;			                //PD8
	GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;  		    
	GPIO_Initure.Pull=GPIO_PULLUP;         			    
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	
	HAL_GPIO_Init(DRDY_GPIO_Port,&GPIO_Initure);    
	
	HAL_NVIC_SetPriority(DRDY_EXTI_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DRDY_EXTI_IRQn);
}


void change_test(int test0)
{
	test = test0;
}

// interrupt callback function
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{ 		
	if(GPIO_Pin==GPIO_PIN_8) //PD8
	{	 
		ADS129x_ReceDATA(test);			 
	}
}


//REG setting
unsigned char* ADS1299_Config()
{
	HAL_GPIO_WritePin(RESET_GPIO_Port,RESET_Pin,GPIO_PIN_SET); // reset high
  delay_ms(3000);
	HAL_GPIO_WritePin(RESET_GPIO_Port,RESET_Pin,GPIO_PIN_RESET); // reset low
  HAL_Delay(10);  
	HAL_GPIO_WritePin(RESET_GPIO_Port,RESET_Pin,GPIO_PIN_SET); // reset high
	HAL_Delay(10); 
	HAL_GPIO_WritePin(RESET_GPIO_Port,RESET_Pin,GPIO_PIN_RESET); // reset low
  HAL_Delay(10); 
	HAL_GPIO_WritePin(RESET_GPIO_Port,RESET_Pin,GPIO_PIN_SET); // reset high
	HAL_Delay(10); 

	ADS1299_STOP(); // Stop Conversion for Configuration
	HAL_Delay(50); 
	ADS129x_SendCMD(_SDATAC); //stop continuous read
	HAL_Delay(10); 
	
	ADS129x_Reg_RW(_WREG|CONFIG1, 0xF5, 1); //output internal clk F5
	//HAL_Delay(10);
	delay_ms(500);	                         //this time must above 50ms, otherwise the last four chip will fail to initialize
	ADS129x_SendCMD(_SDATAC); 

	delay_ms(500);	
	ADS129x_Reg_RW(_WREG|CONFIG1, 0xD5, 2); //intput external clk D5
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG1, 0xD5, 3); //intput external clk D5
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG1, 0xD5, 4); //intput external clk D5
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG1, 0xD5, 5); //intput external clk D5
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|CONFIG2, 0XD4);   //start internal test signal
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG3, 0xEE, 1);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG3, 0XE0, 2);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG3, 0XE0, 3);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG3, 0XE0, 4);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG3, 0XE0, 5);
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|CONFIG4, 0X00);   //continuous read
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|BIAS_SENSN, 0x00);                                                                                                                                                                                            
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|BIAS_SENSP, 0xFF);
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|MISC1, 0X20); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|MISC2, 0X00); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|LOFF, 0X00); 
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSP, 0xFF, 1);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSP, 0XFF, 2);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSP, 0XFF, 3);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSP, 0XFF, 4);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSP, 0XFF, 5);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSN, 0xFF, 1);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSN, 0xFF, 2);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSN, 0xFF, 3);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSN, 0xFF, 4);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSN, 0xFF, 5);
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|LOFF_FLIP,0X00); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|LOFF_STATP,0X00); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|LOFF_STATN,0X00); 
	delay_ms(1);   
	ADS129x_Reg_W_all(_WREG|GPIO,0X00); 
	delay_ms(1);   
		
	ADS129x_Reg_W_all(_WREG|CH1SET, 0x65);  
	ADS129x_Reg_W_all(_WREG|CH2SET, 0x65);  
	ADS129x_Reg_W_all(_WREG|CH3SET, 0x65);
	ADS129x_Reg_W_all(_WREG|CH4SET, 0x65);
	ADS129x_Reg_W_all(_WREG|CH5SET, 0x65);
	ADS129x_Reg_W_all(_WREG|CH6SET, 0x65);
	ADS129x_Reg_W_all(_WREG|CH7SET, 0x65);
	ADS129x_Reg_W_all(_WREG|CH8SET, 0x65);

	data_2 = ADS129x_Reg_R_all();	
	delay_ms(3);
	
	ADS129x_SendCMD(_RDATAC);

	delay_ms(3);

	return data_2;
	}

//initial ADS1299
unsigned char* ADS1299_Init()
	{	
	RESET_Init();
	delay_ms(50);
	START_Init();
	delay_ms(50);
	CS_Init();
	delay_ms(50);
	MX_SPI2_Init();								//initial SPI
	delay_ms(50);
	data_3 = ADS1299_Config();    //initial REG
  delay_ms(128);
  return data_3;
}



//////////////////////////////////////////////////////////////
//ADS1299 read and write
//////////////////////////////////////////////////////////////

// read all the chip
unsigned char* ADS129x_Reg_R_all()
{	
	for (q=0;q<24;q++) 
		{
			data_1[q] = ADS129x_Reg_RW(_RREG|q, 0, 1);
			data_1[q+24] = ADS129x_Reg_RW(_RREG|q, 0, 2);
			data_1[q+48] = ADS129x_Reg_RW(_RREG|q, 0, 3);
			data_1[q+72] = ADS129x_Reg_RW(_RREG|q, 0, 4);
			data_1[q+96] = ADS129x_Reg_RW(_RREG|q, 0, 5);
		}
	return data_1;
}		

// write all the chip
void ADS129x_Reg_W_all(unsigned char cmd, unsigned char data)
{	
	u8 i;
	for(i=1;i<6;i++)
	{	
		ADS129x_Reg_RW(cmd, data, i);
	}
}


// write or read ith ADS1299 REG
unsigned char ADS129x_Reg_RW(unsigned char cmd, unsigned char data, int i)
{     
	unsigned char	ch;

		ADS1299_CHANGE_CHANEL(i,0);	
		HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
		unsigned char temp = 0X00;
		HAL_SPI_Transmit(&hspi1, &temp, 1, 100);
	
	
		if( ( cmd & 0x20)==0x20 )   
		{         		
			temp = 0X00;
			HAL_SPI_Receive(&hspi1, &ch, 0x01, 100);
			ADS1299_CHANGE_CHANEL(i,1);
			return ch;    
		}
		else
		{    
			HAL_SPI_Transmit(&hspi1, &data, 1, 100);
			delay_us(5); 
			ADS1299_CHANGE_CHANEL(i,1);		
			return 0;
		} 
}
	

//select chip
void ADS1299_CHANGE_CHANEL(u8 n,u8 sw)
{
	if (sw == 0)
	{
		switch(n)
		{
			case 1:
				CS1_low();
				break;
			case 2:
				CS2_low();
				break;
			case 3:
				CS3_low();
				break;
			case 4:
				CS4_low();
				break;
			case 5:
				CS5_low();
				break;
			default:
				break;
		}
	}
	else
	{
		switch(n)
		{
			case 1:
				CS1_high();
				break;
			case 2:
				CS2_high();
				break;
			case 3:
				CS3_high();
				break;
			case 4:
				CS4_high();
				break;
			case 5:
				CS5_high();
				break;
			default:
				break;
		}
	}
}



//////////////////////////////////////////////////////////////
// data transmission
//////////////////////////////////////////////////////////////

// send CMD
 void ADS129x_SendCMD(unsigned char cmd)
{
	u8 i;
	for(i=1;i<6;i++)
	{
		ADS1299_CHANGE_CHANEL(i,0);
		HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
		delay_us(10);
		ADS1299_CHANGE_CHANEL(i,1);			
	}
}


// output data
void ADS129x_ReceDATA(int test)
{	
			
		CS1_low();
		HAL_SPI_TransmitReceive(&hspi1, send, bytedata_2_tempreceive_1, 27,100);
		CS1_high();
			
		CS2_low();
		HAL_SPI_TransmitReceive(&hspi1, send, bytedata_2_tempreceive_2, 27,100);
		CS2_high();
			
		CS3_low();
		HAL_SPI_TransmitReceive(&hspi1, send, bytedata_2_tempreceive_3, 27,100);
		CS3_high();
			
		CS4_low();
		HAL_SPI_TransmitReceive(&hspi1, send, bytedata_2_tempreceive_4, 27,100);
		CS4_high();
			
		CS5_low();
		HAL_SPI_TransmitReceive(&hspi1, send, bytedata_2_tempreceive_5, 27,100);
		CS5_high();
		
		for (j=3;j<27;j++)
		{		
				bytedata_2_receive[transport] = bytedata_2_tempreceive_1[j];
				transport++;
		}
		for (j=3;j<27;j++)
		{		
				bytedata_2_receive[transport] = bytedata_2_tempreceive_2[j];
				transport++;
		}
		for (j=3;j<27;j++)
		{		
				bytedata_2_receive[transport] = bytedata_2_tempreceive_3[j];
				transport++;
		}
		for (j=3;j<27;j++)
		{		
				bytedata_2_receive[transport] = bytedata_2_tempreceive_4[j];
				transport++;
		}
		for (j=3;j<27;j++)
		{		
				bytedata_2_receive[transport] = bytedata_2_tempreceive_5[j];
				transport++;
		}
						
		if (transport == len)
		{
			checksum = 0;
			checksum = calcCRC(bytedata_2_receive, len);
			bytedata_2_receive[transport] = checksum;
			VCP_DataTx(bytedata_2_receive, 961);
			LED0_Toggle;
			transport = 0;					
		}		
		if (test == 1)
		{	
			state = state + 1;
			if (state == 500)
			{
			
				LED2_Toggle;
				state = 0;
			
			}
		}			
	return;
}                                                                                                                          




//////////////////////////////////////////////////////////////
// test mode
//////////////////////////////////////////////////////////////

// exit test mode
unsigned char* ADS1299_exit_measure()
{
	// enter common mode
	ADS129x_SendCMD(_SDATAC); //stop continuous reading
	delay_ms(10);

	ADS129x_Reg_W_all(_WREG|CONFIG2, 0XD4);   
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG3, 0xEE, 1);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG3, 0XE0, 2);   
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG3, 0XE0, 2);   
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG3, 0XE0, 2);   
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|CONFIG3, 0XE0, 2);   
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|CONFIG4, 0X00);   
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSP, 0xFF, 1);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSP, 0XFF, 2);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSP, 0XFF, 3);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSP, 0XFF, 4);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSP, 0XFF, 5);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSN, 0xFF, 1);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSN, 0xFF, 2);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSN, 0xFF, 3);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSN, 0xFF, 4);
	delay_ms(1);
	ADS129x_Reg_RW(_WREG|BIAS_SENSN, 0xFF, 5);
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|MISC1,0X20); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|MISC2,0X00); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|LOFF,0X00); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|LOFF_SENSP,0X00); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|LOFF_SENSN,0X00); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|LOFF_FLIP,0X00); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|LOFF_STATP,0X00); 
	delay_ms(1);
	ADS129x_Reg_W_all(_WREG|LOFF_STATN,0X00); 
	delay_ms(1);   	
	ADS129x_Reg_W_all(_WREG|CH1SET, 0x60);  
	ADS129x_Reg_W_all(_WREG|CH2SET, 0x60);  
	ADS129x_Reg_W_all(_WREG|CH3SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH4SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH5SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH6SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH7SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH8SET, 0x60);

	data_2 = ADS129x_Reg_R_all();	
	delay_ms(3);
	ADS129x_SendCMD(_RDATAC);
	delay_ms(3);
	return data_2;
	
}	


// polar voltage test
unsigned char* ADS1299_Polar_vol()
{

	ADS129x_SendCMD(_SDATAC); 
	delay_ms(10);
	
	ADS129x_Reg_W_all(_WREG|CH1SET, 0x50);  
	ADS129x_Reg_W_all(_WREG|CH2SET, 0x50); 
	ADS129x_Reg_W_all(_WREG|CH3SET, 0x50);
	ADS129x_Reg_W_all(_WREG|CH4SET, 0x50);
	ADS129x_Reg_W_all(_WREG|CH5SET, 0x50);
	ADS129x_Reg_W_all(_WREG|CH6SET, 0x50);
	ADS129x_Reg_W_all(_WREG|CH7SET, 0x50);
	ADS129x_Reg_W_all(_WREG|CH8SET, 0x50);

	data_2 = ADS129x_Reg_R_all();	
	delay_ms(3);
	ADS129x_SendCMD(_RDATAC);
	delay_ms(3);
	return data_2;
	
}

unsigned char* ADS1299_Polar_vol_exit()
{

	ADS129x_SendCMD(_SDATAC);
	delay_ms(10);

	ADS129x_Reg_W_all(_WREG|CH1SET, 0x60);  
	ADS129x_Reg_W_all(_WREG|CH2SET, 0x60);  
	ADS129x_Reg_W_all(_WREG|CH3SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH4SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH5SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH6SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH7SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH8SET, 0x60);

	data_2 = ADS129x_Reg_R_all();	
	delay_ms(3);
	ADS129x_SendCMD(_RDATAC);
	delay_ms(3);

	return data_2;
	
}


// noise test
unsigned char* ADS1299_noise_test()
{

	ADS129x_SendCMD(_SDATAC);
	delay_ms(10);

	ADS129x_Reg_W_all(_WREG|CH1SET, 0x61); 
	ADS129x_Reg_W_all(_WREG|CH2SET, 0x61);  
	ADS129x_Reg_W_all(_WREG|CH3SET, 0x61);
	ADS129x_Reg_W_all(_WREG|CH4SET, 0x61);
	ADS129x_Reg_W_all(_WREG|CH5SET, 0x61);
	ADS129x_Reg_W_all(_WREG|CH6SET, 0x61);
	ADS129x_Reg_W_all(_WREG|CH7SET, 0x61);
	ADS129x_Reg_W_all(_WREG|CH8SET, 0x61);

	data_2 = ADS129x_Reg_R_all();	
	delay_ms(3);
	ADS129x_SendCMD(_RDATAC);
	delay_ms(3);
	return data_2;
	
}


// Common mode
unsigned char* ADS1299_Common_mode_measure()
{
	ADS129x_SendCMD(_SDATAC); 
	delay_ms(10);

	ADS129x_Reg_W_all(_WREG|CH1SET, 0x68);
	ADS129x_Reg_W_all(_WREG|CH2SET, 0x68);
	ADS129x_Reg_W_all(_WREG|CH3SET, 0x68);
	ADS129x_Reg_W_all(_WREG|CH4SET, 0x68);
	ADS129x_Reg_W_all(_WREG|CH5SET, 0x68);
	ADS129x_Reg_W_all(_WREG|CH6SET, 0x68);
	ADS129x_Reg_W_all(_WREG|CH7SET, 0x68);
	ADS129x_Reg_W_all(_WREG|CH8SET, 0x68);
	
	data_2 = ADS129x_Reg_R_all();	
	delay_ms(3);
	ADS129x_SendCMD(_RDATAC);
	delay_ms(3);
	return data_2;
}

// differential mode
unsigned char* ADS1299_Differential_mode_measure()
{
	ADS129x_SendCMD(_SDATAC);
	delay_ms(10);

	ADS129x_Reg_W_all(_WREG|CH1SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH2SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH3SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH4SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH5SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH6SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH7SET, 0x60);
	ADS129x_Reg_W_all(_WREG|CH8SET, 0x60);
	
	data_2 = ADS129x_Reg_R_all();	
	delay_ms(3);
	ADS129x_SendCMD(_RDATAC);
	delay_ms(3);
	return data_2;
}

