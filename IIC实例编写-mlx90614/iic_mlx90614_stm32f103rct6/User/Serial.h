#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdio.h> //用于对printf函数进行重定向，printf默认输出到屏幕，想要输出给调试助手需要这句
#include <stdarg.h> //用于封装sprintf函数，涉及可变参数
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t* Array,uint16_t Lenth);
void Serial_SendString(char* String);
uint32_t Serial_Pow(uint32_t x, uint32_t n);
void Serial_SendNumber(uint32_t Number,uint8_t Lenth);
void Serial_Sprintf(char* format, ...);


#endif

