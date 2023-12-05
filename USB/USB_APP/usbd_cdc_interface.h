#ifndef __USBD_CDC_IF_H
#define __USBD_CDC_IF_H
#include "usbd_cdc.h"

#define USB_USART_REC_LEN	 	200     //The maximum number of bytes in the receive buffer of a USB serial port

#define CDC_POLLING_INTERVAL    1       //Polling interval¬×max 65ms, min 1ms

extern u8  USB_USART_RX_BUF[USB_USART_REC_LEN]; //Receive buffers, max USB_USART_REC_LEN
extern u16 USB_USART_RX_STA;   					//Receive state	
extern USBD_CDC_ItfTypeDef  USBD_CDC_fops;

void VCP_DataTx(uint8_t *data,uint32_t Len);
uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len);
void usb_printf(char* fmt,...); 

#endif 

