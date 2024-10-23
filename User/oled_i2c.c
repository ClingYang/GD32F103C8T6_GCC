

#include "oled_i2c.h"
#include "oled_font.h"

/**
* 功能：
* 输入：
*     1）uint8_t i2c_addr, 
*     2）uint8_t i2c_data

**/
void i2c_write_byte(uint8_t i2c_addr, uint8_t i2c_data){
	while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
	i2c_start_on_bus(I2C0);
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));    // 进入主机模式
	i2c_master_addressing(I2C0, OLED_I2C_ADDR, I2C_TRANSMITTER);
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));   // 判断地址发送出去
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);         // 清除ADDSEND位
	while(SET != i2c_flag_get(I2C0, I2C_FLAG_TBE)); // 进入数据发送状态
	i2c_data_transmit(I2C0, i2c_addr);
	while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
	i2c_data_transmit (I2C0, i2c_data);
	while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
	i2c_stop_on_bus (I2C0);
	while(I2C_CTL0(I2C0)&0x0200);
}

/**
* 功能：
* 输入：oled_cmd：oled的控制命令
**/
void oled_write_command(uint8_t oled_cmd){
	i2c_write_byte(OLED_I2C_CMD_ADDR, oled_cmd); 
}

/**
* 功能：
* 输入：oled_data：要发送给oled的数据
**/
void oled_write_data(uint8_t oled_data){
	i2c_write_byte(OLED_I2C_DATA_ADDR, oled_data); 
}

/**
* 功能：
* 输入：无
**/
void oled_init(void){
	oled_write_command(0xAE);// 0xAE:关显示，0xAF:开显示
	
	oled_write_command(0x00);// 设置开始地址的低字节       
	oled_write_command(0x10);// 设置开始地址的高字节
	
	oled_write_command(0xd5);// 命令头，设置显示时钟分频比/振荡器频率
	oled_write_command(0x80);// 设置分割比率，设置时钟为100帧/秒
	
	oled_write_command(0xa8);// 命令头，设置多路复用率(1 to 64)
	oled_write_command(0x3f);// --1/64 duty
	
	oled_write_command(0xd3);// 命令头，设置显示偏移移位映射RAM计数器(0x00~0x3F)
	oled_write_command(0x00);// 不偏移
	
	oled_write_command(0x00);// 写入页位置（0xB0~7）
	oled_write_command(0x40);// 显示开始线
	
	oled_write_command(0x8d);// VCC电源
	oled_write_command(0x14);// --set(0x10) disable
	
	oled_write_command(0xa1);// 设置段重新映射
	oled_write_command(0xc8);// 设置Y轴扫描方向，0xc0上下反置 ，0xc8正常（上边为0行）
	
	oled_write_command(0xda);// 命令头，--set com pins hardware configuration
	oled_write_command(0x12);
	
	oled_write_command(0x81);// 对比度，指令：0x81，数据：0~255（255最高）
	oled_write_command(0xff);
	
	oled_write_command(0xd9);// 命令头，--set pre-charge period
	oled_write_command(0xf1);// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	
	oled_write_command(0xdb);// 命令头，--set vcomh
	oled_write_command(0x30);// Set VCOM Deselect Level
	
	oled_write_command(0x20);// 水平寻址设置
	oled_write_command(0x00);
	
	oled_write_command(0xa4);// 0xa4:正常显示，0xa5:整体点亮
	oled_write_command(0xa6);// 0xa6:正常显示，0xa7:反色显示
	
	oled_write_command(0xAF);// 0xAE:关显示，0xAF:开显示
}

// OLED显示单一色度
void oled_fill(uint8_t fill_data){
	uint8_t i=0, n;
	for(i = 0; i < 8; i++){
		oled_write_command (0xB0+i);    //设置页地址（0~7）
		oled_write_command (0x00);      //设置显示位置—列低地址
		oled_write_command (0x10);      //设置显示位置—列高地址  		
		for(n = 0; n < 132; n++){
			oled_write_data(fill_data);
		}
	}
}

// OLED 白屏显示
void oled_display_white(void){
	oled_fill(0xFF);
}

// OLED 清屏显示
void oled_clear_all(void){

	oled_fill(0x00);
}

// OLED打开
void oled_on(){
	oled_write_command(0xAF);  // 开启显示
	oled_write_command(0x8D);  // 设置电荷泵
	oled_write_command(0x14);  // 开启电荷泵
}

// OLED关闭
void oled_off(){
	oled_write_command(0xAF);  // 开启显示
	oled_write_command(0x8D);  // 设置电荷泵
	oled_write_command(0x10);  // 关闭电荷泵
}

// OLED设置显示位置
void oled_set_pos(uint8_t x, uint8_t y){
	oled_write_command(0xB0 + y);
	oled_write_command((x & 0xf0) >> 4 | 0x10 );
	oled_write_command((x & 0x0f) | 0x01 );
}

// 显示中文汉字
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

// 显示char字符
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size){
	uint8_t c_index = 0, i = 0;
	
	c_index = chr - ' ';  // 获取chr在字模数组中的序号
	if(char_size == 16){
		if(x > MAX_COLUMN - 8){ //如果超出了屏幕的显示宽度，则换到下一行重新开始
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
		if(x > MAX_COLUMN - 6){ //如果超出了屏幕的显示宽度，则换到下一行重新开始
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

// 显示字符串
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

// 在(x,y)处显示“error!”信息
void oled_show_error(uint8_t x, uint16_t y, uint8_t *err){
	oled_show_string(x, y, err, 16);
}
