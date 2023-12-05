
#ifndef __ADS1299__
#define __ADS1299__

#include "sys.h"
#include "crc.h"

typedef struct {    //addr   default value
uint8_t ID;         //0x00   0x3D(ads1299-6) 0x3E(ads1299)   
uint8_t CONFIG1;    //0x01   0x96
uint8_t CONFIG2;    //0x02   0xC0
uint8_t CONFIG3;    //0x03   0x60
uint8_t LOFF;       //0x04   0x00
uint8_t CH1SET;     //0x05   0x61
uint8_t CH2SET;     //0x06   0x61
uint8_t CH3SET;     //0x07   0x61
uint8_t CH4SET;     //0x08   0x61
uint8_t CH5SET;     //0x09   0x61
uint8_t CH6SET;     //0x0A   0x61
uint8_t CH7SET;     //0x0B   0x61
uint8_t CH8SET;     //0x0C   0x61
uint8_t BIAS_SENSP; //0x0D   0x00
uint8_t BIAS_SENSN; //0x0E   0x00
uint8_t LOFF_SENSP; //0x0F   0x00
uint8_t LOFF_SENSN; //0x10   0x00
uint8_t LOFF_FLIP;  //0x11   0x00
uint8_t LOFF_STATP; //0x12   0x00
uint8_t LOFF_STATN; //0x13   0x00
uint8_t GPIO;       //0x14   0x0F
uint8_t MISC1;      //0x15   0x00
uint8_t MISC2;      //0x16   0x00
uint8_t CONFIG4;    //0x17   0x00
} REG_MAP;          //  size 24
//#pragma pack(0)  

#define START_Pin GPIO_PIN_9
#define START_GPIO_Port GPIOH
#define RESET_Pin GPIO_PIN_10
#define RESET_GPIO_Port GPIOH
#define CS1_Pin GPIO_PIN_6
#define CS1_GPIO_Port GPIOA
#define CS2_Pin GPIO_PIN_5
#define CS2_GPIO_Port GPIOA
#define CS3_Pin GPIO_PIN_6
#define CS3_GPIO_Port GPIOH
#define CS4_Pin GPIO_PIN_4
#define CS4_GPIO_Port GPIOA
#define CS5_Pin GPIO_PIN_8
#define CS5_GPIO_Port GPIOH


#define DRDY_Pin GPIO_PIN_8
#define DRDY_GPIO_Port GPIOD
#define DRDY_EXTI_IRQn EXTI9_5_IRQn



 
// SPI Command Definitions (Datasheet, 35)
#define _WAKEUP  0x02 // Wake-up from standby mode
#define _STANDBY 0x04 // Enter Standby mode
#define _RESET   0x06 // Reset the device registers to default
#define _START   0x08 // Start and restart (synchronize) conversions
#define _STOP    0x0A // Stop conversion
#define _RDATAC  0x10 // Enable Read Data Continuous mode (default mode at power-up)
#define _SDATAC  0x11 // Stop Read Data Continuous mode
#define _RDATA   0x12 // Read data by command; supports multiple read back
#define _RREG    0x20 // Read Register
#define _WREG    0x40 // Write to Register 
// Register Addresses
#define ID         0x00
#define CONFIG1    0x01
#define CONFIG2    0x02
#define CONFIG3    0x03
#define LOFF       0x04
#define CH1SET     0x05
#define CH2SET     0x06
#define CH3SET     0x07
#define CH4SET     0x08
#define CH5SET     0x09
#define CH6SET     0x0A
#define CH7SET     0x0B
#define CH8SET     0x0C
#define BIAS_SENSP 0x0D
#define BIAS_SENSN 0x0E
#define LOFF_SENSP 0x0F
#define LOFF_SENSN 0x10
#define LOFF_FLIP  0x11
#define LOFF_STATP 0x12
#define LOFF_STATN 0x13
#define GPIO       0x14
#define MISC1      0x15
#define MISC2      0x16
#define CONFIG4    0x17 
 
 
 
 

#define ADS1299_STOP()   do{HAL_GPIO_WritePin(START_GPIO_Port,START_Pin,GPIO_PIN_RESET);}while(0);
#define ADS1299_START()    do{HAL_GPIO_WritePin(START_GPIO_Port,START_Pin,GPIO_PIN_SET);}while(0); 
 
#define CS1_high()   do{HAL_GPIO_WritePin(CS1_GPIO_Port,CS1_Pin,GPIO_PIN_SET);}while(0);
#define CS1_low()    do{HAL_GPIO_WritePin(CS1_GPIO_Port,CS1_Pin,GPIO_PIN_RESET);}while(0);
#define CS2_high()   do{HAL_GPIO_WritePin(CS2_GPIO_Port,CS2_Pin,GPIO_PIN_SET);}while(0);
#define CS2_low()    do{HAL_GPIO_WritePin(CS2_GPIO_Port,CS2_Pin,GPIO_PIN_RESET);}while(0);
#define CS3_high()   do{HAL_GPIO_WritePin(CS3_GPIO_Port,CS3_Pin,GPIO_PIN_SET);}while(0);
#define CS3_low()    do{HAL_GPIO_WritePin(CS3_GPIO_Port,CS3_Pin,GPIO_PIN_RESET);}while(0);
#define CS4_high()   do{HAL_GPIO_WritePin(CS4_GPIO_Port,CS4_Pin,GPIO_PIN_SET);}while(0);
#define CS4_low()    do{HAL_GPIO_WritePin(CS4_GPIO_Port,CS4_Pin,GPIO_PIN_RESET);}while(0);
#define CS5_high()   do{HAL_GPIO_WritePin(CS5_GPIO_Port,CS5_Pin,GPIO_PIN_SET);}while(0);
#define CS5_low()    do{HAL_GPIO_WritePin(CS5_GPIO_Port,CS5_Pin,GPIO_PIN_RESET);}while(0);

extern SPI_HandleTypeDef hspi1; // spi1 Handletypedef

void START_Init(void);
void RESET_Init(void);
void CS_Init(void);
void DRDY_Init(void);


unsigned char* ADS1299_Init(void);           	//  init ads1299 
unsigned char* ADS1299_Config(void);           	//  init ads1299 
unsigned char* ADS1299_exit_measure(void);
unsigned char* ADS1299_Polar_vol(void);
unsigned char* ADS1299_Polar_vol_exit(void);
unsigned char* ADS1299_noise_test(void);
unsigned char* common_mode_measure(void);
unsigned char* ADS1299_Common_mode_measure(void);
unsigned char* ADS1299_Differential_mode_measure(void);
unsigned char* ADS129x_Reg_R_all(void);
void ADS129x_Reg_W_all(unsigned char cmd, unsigned char data);
unsigned char ADS129x_Reg_RW(unsigned char cmd,unsigned char data, int i);
void ADS129x_SendCMD(unsigned char cmd);
void ADS129x_ReceDATA(int test);
void ADS1299_CHANGE_CHANEL(u8 n,u8 sw);
void change_test(int test0);

#define MWSPT_NSEC 3

#endif
