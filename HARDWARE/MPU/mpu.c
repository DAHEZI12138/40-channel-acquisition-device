#include "mpu.h"
#include "led.h"
#include "delay.h"


u8 MPU_Set_Protection(u32 baseaddr,u32 size,u32 rnum,u32 ap)
{
	MPU_Region_InitTypeDef MPU_Initure;
	
	HAL_MPU_Disable();								        

	MPU_Initure.Enable=MPU_REGION_ENABLE;			        
	MPU_Initure.Number=rnum;			                    
	MPU_Initure.BaseAddress=baseaddr;	                    
	MPU_Initure.Size=size;				                    
	MPU_Initure.SubRegionDisable=0X00;                      
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;                
	MPU_Initure.AccessPermission=(u8)ap;		            
	MPU_Initure.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;	
	MPU_Initure.IsShareable=MPU_ACCESS_NOT_SHAREABLE;       
	MPU_Initure.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;         
	MPU_Initure.IsBufferable=MPU_ACCESS_BUFFERABLE;         
	HAL_MPU_ConfigRegion(&MPU_Initure);                    
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);			       
    return 0;
}

u8 MPU_Set_SRAMProtection(u32 baseaddr,u32 size,u32 rnum,u32 ap)
{
	MPU_Region_InitTypeDef MPU_Initure;
	
	HAL_MPU_Disable();								        

	MPU_Initure.Enable=MPU_REGION_ENABLE;			        
	MPU_Initure.Number=rnum;			                    
	MPU_Initure.BaseAddress=baseaddr;	                   
	MPU_Initure.Size=size;				                    
	MPU_Initure.SubRegionDisable=0X00;                     
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;                
	MPU_Initure.AccessPermission=(u8)ap;		            
	MPU_Initure.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;	
	MPU_Initure.IsShareable=MPU_ACCESS_SHAREABLE;       
	MPU_Initure.IsCacheable=MPU_ACCESS_CACHEABLE;      
	MPU_Initure.IsBufferable=MPU_ACCESS_NOT_BUFFERABLE;         
	HAL_MPU_ConfigRegion(&MPU_Initure);                     
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);			       
    return 0;
}


void MPU_Memory_Protection(void)
{
	MPU_Set_Protection(0x60000000,MPU_REGION_SIZE_64MB,MPU_REGION_NUMBER0,MPU_REGION_FULL_ACCESS);	
	MPU_Set_SRAMProtection(0x24000000,MPU_REGION_SIZE_512KB,MPU_REGION_NUMBER1,MPU_REGION_FULL_ACCESS);	
    MPU_Set_Protection(0XC0000000,MPU_REGION_SIZE_32MB,MPU_REGION_NUMBER2,MPU_REGION_FULL_ACCESS);	
	MPU_Set_Protection(0X80000000,MPU_REGION_SIZE_256MB,MPU_REGION_NUMBER3,MPU_REGION_FULL_ACCESS);	
}
