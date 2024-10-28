#include "gd32f10x.h"
#include "systick.h"
#include "i2c.h"
#include "oled_i2c.h"
#include "usart.h"
int main()
{

    systick_config();
    nvic_config();

    dma_config();

    usart_config();
    /* enable   USART_INT_IDLE */
    usart_interrupt_enable(USART2, USART_INT_IDLE);
    printf("\n\rPlease send data less than 256 bytes:\n\r");
    i2c_init();
    oled_init();
    oled_clear_all();
    oled_show_string(0, 0, (uint8_t *)"USART2", 16);

    while (1)
    {
        if (1 == receive_flag)
        {
            for (tx_count = 0; tx_count < rx_count; tx_count++)
            {
                while (RESET == usart_flag_get(USART2, USART_FLAG_TBE))
                    ;
                usart_data_transmit(USART2, rxbuffer[tx_count]);
            }
            receive_flag = 0;
        }
    }
}
