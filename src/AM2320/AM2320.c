#include <stm32f10x_gpio.h>
#include <stm32f10x_i2c.h>
#include <stm32f10x_rcc.h>
#include "i2cm.h"
#include <utils/delay.h>
#include "AM2320.h"

float am2320_t;
float am2320_h;
		
unsigned int CRC16(uint8_t *ptr, uint8_t length) 
{ 
      unsigned int crc = 0xFFFF; 
      uint8_t s = 0x00; 

      while(length--) {
        crc ^= *ptr++; 
        for(s = 0; s < 8; s++) {
          if((crc & 0x01) != 0) {
            crc >>= 1; 
            crc ^= 0xA001; 
          } else crc >>= 1; 
        } 
      } 
      return crc; 
} 

void AM2320_init(void)
{
	 i2cm_init(I2C1,  100000);
}


#define temiout 500
int AM2320_read(void)
{
	uint8_t buf[8];
	int8_t err = 5;
		
	while (!i2cm_Start(I2C1, AM2320_address, 0, temiout) && err ) err--;	
	if (err)
  {	
	  delay_ms(2); //>0.8ms					
	  i2cm_Stop(I2C1, temiout);
    delay_ms(1); //>0.8ms	
	}		
	else return 1;	
	
	err = i2cm_Start(I2C1, AM2320_address, 0, temiout);	
	if (!err)
  {	
	  buf[0]= (0x03); // запрос
	  buf[1]=	(0x00); // с 0-го адреса
	  buf[2]=	(0x04); // 4 байта	
	  i2cm_WriteBuff(I2C1, buf, 3, temiout);
	  i2cm_Stop(I2C1, temiout);		
	  delay_ms(2); //>1.5ms		
	}		
	else return 2;
	
	err = i2cm_Start(I2C1, AM2320_address, 1, temiout);
	// запрос 4 байт (температуры и влажности)
	if (!err)
  {
	  // считываем результаты запроса
		delay_us(50);
	  i2cm_ReadBuffAndStop(I2C1, buf, 8, temiout);
	}
	else return 3;
	
	// CRC check
	unsigned int Rcrc = buf[7] << 8;
	Rcrc += buf[6];
	if (Rcrc == CRC16(buf, 6)) {
		unsigned int temperature = ((buf[4] & 0x7F) << 8) + buf[5];
		am2320_t = temperature / 10.0;
		am2320_t = ((buf[4] & 0x80) >> 7) == 1 ? am2320_t * (-1) : am2320_t;

		unsigned int humidity = (buf[2] << 8) + buf[3];
		am2320_h = humidity / 10.0;
		return 0;
	}
        return 4;
}
