

#include "oled_i2c.h"
#include "oled_font.h"

/**
* ���ܣ�
* ���룺
*     1��uint8_t i2c_addr, 
*     2��uint8_t i2c_data

**/
void i2c_write_byte(uint8_t i2c_addr, uint8_t i2c_data){
	while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
	i2c_start_on_bus(I2C0);
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));    // ��������ģʽ
	i2c_master_addressing(I2C0, OLED_I2C_ADDR, I2C_TRANSMITTER);
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));   // �жϵ�ַ���ͳ�ȥ
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);         // ���ADDSENDλ
	while(SET != i2c_flag_get(I2C0, I2C_FLAG_TBE)); // �������ݷ���״̬
	i2c_data_transmit(I2C0, i2c_addr);
	while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
	i2c_data_transmit (I2C0, i2c_data);
	while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
	i2c_stop_on_bus (I2C0);
	while(I2C_CTL0(I2C0)&0x0200);
}

/**
* ���ܣ�
* ���룺oled_cmd��oled�Ŀ�������
**/
void oled_write_command(uint8_t oled_cmd){
	i2c_write_byte(OLED_I2C_CMD_ADDR, oled_cmd); 
}

/**
* ���ܣ�
* ���룺oled_data��Ҫ���͸�oled������
**/
void oled_write_data(uint8_t oled_data){
	i2c_write_byte(OLED_I2C_DATA_ADDR, oled_data); 
}

/**
* ���ܣ�
* ���룺��
**/
void oled_init(void){
	oled_write_command(0xAE);// 0xAE:����ʾ��0xAF:����ʾ
	
	oled_write_command(0x00);// ���ÿ�ʼ��ַ�ĵ��ֽ�       
	oled_write_command(0x10);// ���ÿ�ʼ��ַ�ĸ��ֽ�
	
	oled_write_command(0xd5);// ����ͷ��������ʾʱ�ӷ�Ƶ��/����Ƶ��
	oled_write_command(0x80);// ���÷ָ���ʣ�����ʱ��Ϊ100֡/��
	
	oled_write_command(0xa8);// ����ͷ�����ö�·������(1 to 64)
	oled_write_command(0x3f);// --1/64 duty
	
	oled_write_command(0xd3);// ����ͷ��������ʾƫ����λӳ��RAM������(0x00~0x3F)
	oled_write_command(0x00);// ��ƫ��
	
	oled_write_command(0x00);// д��ҳλ�ã�0xB0~7��
	oled_write_command(0x40);// ��ʾ��ʼ��
	
	oled_write_command(0x8d);// VCC��Դ
	oled_write_command(0x14);// --set(0x10) disable
	
	oled_write_command(0xa1);// ���ö�����ӳ��
	oled_write_command(0xc8);// ����Y��ɨ�跽��0xc0���·��� ��0xc8�������ϱ�Ϊ0�У�
	
	oled_write_command(0xda);// ����ͷ��--set com pins hardware configuration
	oled_write_command(0x12);
	
	oled_write_command(0x81);// �Աȶȣ�ָ�0x81�����ݣ�0~255��255��ߣ�
	oled_write_command(0xff);
	
	oled_write_command(0xd9);// ����ͷ��--set pre-charge period
	oled_write_command(0xf1);// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	
	oled_write_command(0xdb);// ����ͷ��--set vcomh
	oled_write_command(0x30);// Set VCOM Deselect Level
	
	oled_write_command(0x20);// ˮƽѰַ����
	oled_write_command(0x00);
	
	oled_write_command(0xa4);// 0xa4:������ʾ��0xa5:�������
	oled_write_command(0xa6);// 0xa6:������ʾ��0xa7:��ɫ��ʾ
	
	oled_write_command(0xAF);// 0xAE:����ʾ��0xAF:����ʾ
}

// OLED��ʾ��һɫ��
void oled_fill(uint8_t fill_data){
	uint8_t i=0, n;
	for(i = 0; i < 8; i++){
		oled_write_command (0xB0+i);    //����ҳ��ַ��0~7��
		oled_write_command (0x00);      //������ʾλ�á��е͵�ַ
		oled_write_command (0x10);      //������ʾλ�á��иߵ�ַ  		
		for(n = 0; n < 132; n++){
			oled_write_data(fill_data);
		}
	}
}

// OLED ������ʾ
void oled_display_white(void){
	oled_fill(0xFF);
}

// OLED ������ʾ
void oled_clear_all(void){

	oled_fill(0x00);
}

// OLED��
void oled_on(){
	oled_write_command(0xAF);  // ������ʾ
	oled_write_command(0x8D);  // ���õ�ɱ�
	oled_write_command(0x14);  // ������ɱ�
}

// OLED�ر�
void oled_off(){
	oled_write_command(0xAF);  // ������ʾ
	oled_write_command(0x8D);  // ���õ�ɱ�
	oled_write_command(0x10);  // �رյ�ɱ�
}

// OLED������ʾλ��
void oled_set_pos(uint8_t x, uint8_t y){
	oled_write_command(0xB0 + y);
	oled_write_command((x & 0xf0) >> 4 | 0x10 );
	oled_write_command((x & 0x0f) | 0x01 );
}

// ��ʾ���ĺ���
void oled_show_chinese(uint8_t x, uint8_t y, uint8_t index){
	uint8_t i;
	oled_set_pos(x, y);
	for(i = 0; i < 16; i++){
		oled_write_data(Hzk[2*index][i]);
	}
	oled_set_pos(x, y+1);
	for(i = 0; i < 16; i++){
		oled_write_data(Hzk[2*index + 1][i]);
	}
}

// ��ʾchar�ַ�
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size){
	uint8_t c_index = 0, i = 0;
	
	c_index = chr - ' ';  // ��ȡchr����ģ�����е����
	if(char_size == 16){
		if(x > MAX_COLUMN - 8){ //�����������Ļ����ʾ��ȣ��򻻵���һ�����¿�ʼ
			x = 0;
			y += 2;
		}
		oled_set_pos(x, y);
		for(i = 0; i < 8; i++){
			oled_write_data(F8X16[c_index*16 + i]);
		}
		oled_set_pos(x, y+1);
		for(i = 0; i < 8; i++){
			oled_write_data(F8X16[c_index*16 + i + 8]);
		}
	}else if(char_size == 8){
		if(x > MAX_COLUMN - 6){ //�����������Ļ����ʾ��ȣ��򻻵���һ�����¿�ʼ
			x = 0;
			y += 1;
		}
		oled_set_pos(x, y);
		for(i = 0; i < 6; i++){
			oled_write_data(F6x8[c_index][i]);
		}
	}else{
		oled_show_error(x, y, (uint8_t *)"wrong char_size setted.");
	}
}

// ��ʾ�ַ���
void oled_show_string(uint8_t x, uint8_t y, uint8_t *str, uint8_t char_size){
	uint8_t i = 0;
	while(str[i] != '\0'){
		oled_show_char(x, y, str[i], char_size);
		if(char_size == 16){
			x += 8;
			if(x > MAX_COLUMN - 8){
				x = 0;
				y += 2;
			}
		}else if(char_size == 8){
			x += 6;
			if(x > MAX_COLUMN - 8){
				x = 0;
				y += 1;
			}
		}
		i++;
	}
}

// ��(x,y)����ʾ��error!����Ϣ
void oled_show_error(uint8_t x, uint16_t y, uint8_t *err){
	oled_show_string(x, y, err, 16);
}
