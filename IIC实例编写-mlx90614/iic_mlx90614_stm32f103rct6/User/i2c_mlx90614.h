#ifndef I2C_MLX90614_H
#define I2C_MLX90614_H

#include "Delay.h"
#include "Serial.h"
#include "stm32f10x_it.h"

#define RCC_APB2Periph_I2C RCC_APB2Periph_GPIOB
#define I2C_SDA_PORT GPIOB
#define I2C_SDA_PIN GPIO_Pin_9
#define I2C_SCL_PORT GPIOB
#define I2C_SCL_PIN GPIO_Pin_8

void I2C_Initialize(void);
void I2C_W_SDA(uint8_t BitVal);
void I2C_W_SCL(uint8_t BitVal);
uint8_t I2C_R_SDA(void);
void I2C_Start(void);
void I2C_SendByte(uint8_t Data);
uint8_t I2C_ReceiveByte(void);
void I2C_Stop(void);
void I2C_SendAck(uint8_t AckBit);
uint8_t I2C_ReceiveAck(void);

//------------------------------------------------

#define MLX90614_ADDRESS 0x00
#define MLX90614_RAM_ACCESS 0x00
#define MLX90614_EEPROM_ACCESS 0x20
#define MLX90614_RAM_TOBJ1 0x07

void MLX90614_Init(void);
uint16_t MLX90614_ReadReg(uint8_t RegAddress);
float MLX90614_GetData(void);

#endif // I2C_MLX90614_H
