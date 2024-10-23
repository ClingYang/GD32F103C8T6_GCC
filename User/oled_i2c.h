#ifndef __OLED_H
#define __OLED_H

#include "gd32f10x.h"
#include "i2c.h"

#define OLED_I2C_ADDR      0x78
#define OLED_I2C_CMD_ADDR  0x00
#define OLED_I2C_DATA_ADDR 0x40

#define MAX_COLUMN 132

void i2c_write_byte(uint8_t i2c_addr, uint8_t i2c_data);

void oled_write_command(uint8_t oled_cmd); 
void oled_write_data(uint8_t oled_data); 

void oled_init(void);

void oled_display_white(void);							// OLED ������ʾ
void oled_clear_all(void);								// OLED ������ʾ
void oled_fill(uint8_t fill_data);                      // ��һɫ�����OLED��

void oled_set_pos(uint8_t x, uint8_t y);                // ��������
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size);    // ��ʾ�ַ�
void oled_show_string(uint8_t x, uint8_t y, uint8_t *str, uint8_t char_size); // ��ʾ�ַ���

void oled_show_chinese(uint8_t x, uint8_t y, uint8_t index);                  // ��ʾ����

void oled_show_error(uint8_t x, uint16_t y, uint8_t *err);                    // ��ʾ������ʾ
	
#endif
