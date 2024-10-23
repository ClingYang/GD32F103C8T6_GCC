#include "i2c.h"

void i2c_init(void){
	i2c_deinit(I2C0);
	
	//ʹ������ʱ��
	rcu_periph_clock_enable(RCU_I2C0);
	rcu_periph_clock_enable(RCU_GPIOB);
	
	//����gpio��
	gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
	
	//����i2c0
	i2c_clock_config(I2C0, I2C_SPEED, I2C_DTCY_2);  // i2cʱ������
	i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_OWN_ADDR);
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);
	
	//ʹ��i2c0
	i2c_enable(I2C0);
}