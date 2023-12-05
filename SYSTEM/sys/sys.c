#include "sys.h"
//enable L1-Cache
void Cache_Enable(void)
{
    SCB_EnableICache();
    SCB_EnableDCache();  
	SCB->CACR|=1<<2;   	
}


void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{  
	HAL_StatusTypeDef ret=HAL_OK;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
    //RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
    
	MODIFY_REG(PWR->CR3,PWR_CR3_SCUEN, 0);
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) {}
  
	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState=RCC_HSE_ON;
    RCC_OscInitStruct.HSI48State=RCC_HSI48_ON;
	RCC_OscInitStruct.HSIState=RCC_HSI_OFF;
	RCC_OscInitStruct.CSIState=RCC_CSI_OFF;
	RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE;

	RCC_OscInitStruct.PLL.PLLN=plln;
	RCC_OscInitStruct.PLL.PLLM=pllm;
	RCC_OscInitStruct.PLL.PLLP=pllp;
	RCC_OscInitStruct.PLL.PLLQ=pllq;

	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
	ret=HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret!=HAL_OK) while(1);
  
	RCC_ClkInitStruct.ClockType=(RCC_CLOCKTYPE_SYSCLK|\
                                RCC_CLOCKTYPE_HCLK |\
                                RCC_CLOCKTYPE_D1PCLK1 |\
                                RCC_CLOCKTYPE_PCLK1 |\
                                RCC_CLOCKTYPE_PCLK2 |\
                                RCC_CLOCKTYPE_D3PCLK1);

	RCC_ClkInitStruct.SYSCLKSource= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider=RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider=RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider=RCC_APB1_DIV2; 
	RCC_ClkInitStruct.APB2CLKDivider=RCC_APB2_DIV2; 
	RCC_ClkInitStruct.APB3CLKDivider=RCC_APB3_DIV2;  
	RCC_ClkInitStruct.APB4CLKDivider=RCC_APB4_DIV4; 
	ret=HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_2);
	if(ret!=HAL_OK) while(1);
  
	__HAL_RCC_CSI_ENABLE() ;
	__HAL_RCC_SYSCFG_CLK_ENABLE() ;  
	HAL_EnableCompensationCell();
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

u8 Get_ICahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>17)&0X01;
    return sta;
}

u8 Get_DCahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>16)&0X01;
    return sta;
}

#if defined(__clang__) 
void __attribute__((noinline)) WFI_SET(void)
{
    __asm__("wfi");
}
  
void __attribute__((noinline)) INTX_DISABLE(void)
{
    __asm__("cpsid i \t\n"
            "bx lr");
}

void __attribute__((noinline)) INTX_ENABLE(void)
{
    __asm__("cpsie i \t\n"
            "bx lr");
}

void __attribute__((noinline)) MSR_MSP(u32 addr) 
{
    __asm__("msr msp, r0 \t\n"
            "bx r14");
}
#elif defined (__CC_ARM)     
__asm void WFI_SET(void)
{
	WFI;		  
}

__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}

__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 		
	BX r14
}
#endif
