#include "usart.h"

uint8_t rxbuffer[256];
uint8_t rx_count = 0;
uint8_t tx_count = 0;
__IO uint8_t receive_flag = 0;
void dma_config(void)
{
    dma_parameter_struct dma_init_struct;

    rcu_periph_clock_enable(RCU_DMA0);

    /* deinitialize DMA channel2 (USART2 rx) */
    dma_deinit(DMA0, DMA_CH2);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)rxbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = 256;
    dma_init_struct.periph_addr = USART2_RDATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH2, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH2);
    /* enable DMA channel4 */
    dma_channel_enable(DMA0, DMA_CH2);
}
void usart_config(void)
{
    /* enable USART clock */
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_USART2);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    /* USART configure */
    usart_deinit(USART2);
    usart_baudrate_set(USART2, 115200U);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_dma_receive_config(USART2, USART_RECEIVE_DMA_ENABLE);
    usart_dma_transmit_config(USART2, USART_TRANSMIT_DMA_ENABLE);
    usart_enable(USART2);
}
void nvic_config(void)
{
    nvic_irq_enable(USART2_IRQn, 0, 0);
}


int _write(int fd, char *pBuffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        usart_data_transmit(USART2, (uint8_t)pBuffer[i]);
        while (RESET == usart_flag_get(USART2, USART_FLAG_TBE))
            ;
    }
    return size;
}

// /* retarget the C library printf function to the USART */
// int fputc(int ch, FILE *f)
// {
//     usart_data_transmit(USART0, (uint8_t)ch);
//     while (RESET == usart_flag_get(USART0, USART_FLAG_TBE))
//         ;
//     return ch;
// }

