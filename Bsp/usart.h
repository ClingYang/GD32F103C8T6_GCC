#ifndef __USART_H
#define __USART_H
#include "gd32f10x.h"
#include <stdio.h>


#define USART2_RDATA_ADDRESS      ((uint32_t)&USART_DATA(USART2))
extern uint8_t rxbuffer[256];
extern uint8_t rx_count;
extern uint8_t tx_count;
extern __IO uint8_t receive_flag;



void dma_config(void);
void usart_config(void);
void nvic_config(void);

void usart_config(void);

#endif /* __USART_H */
