#include "i2c_mlx90614.h"

void I2C_Initialize(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_I2C, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);

    GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN); // set default bitval
    GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN);
}

void I2C_W_SDA(uint8_t BitVal) {
    GPIO_WriteBit(I2C_SDA_PORT, I2C_SDA_PIN, (BitAction)BitVal);
    // if (BitVal) // F2: register operation
    //     I2C_SDA_PORT->BSRR = I2C_SDA_PIN;
    // else
    //     I2C_SDA_PORT->BRR = I2C_SDA_PIN;
    Delay_us(10);
}

void I2C_W_SCL(uint8_t BitVal) {
    GPIO_WriteBit(I2C_SCL_PORT, I2C_SCL_PIN, (BitAction)BitVal);
    // if (BitVal) // F2: register operation
    //     I2C_SCL_PORT->BSRR = I2C_SCL_PIN;
    // else
    //     I2C_SCL_PORT->BRR = I2C_SCL_PIN;
    Delay_us(10);
}

uint8_t I2C_R_SDA() {
    uint8_t BitVal = GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN);
    // uint32_t BitVal = I2C_SDA_PORT->IDR & I2C_SDA_PIN; // F2: register operation
    Delay_us(10);
    return BitVal;
}

void I2C_Start() {
    I2C_W_SDA(1);
    I2C_W_SCL(1);
    I2C_W_SDA(0); // start
    I2C_W_SCL(0); // ready for put data to SDA
}

void I2C_SendByte(uint8_t Data) {
    for (int i = 0; i < 8; ++i) {
        I2C_W_SDA(Data & (0x80 >> i)); // SCL=0,time for put data to SDA
        I2C_W_SCL(1);                  // data been taken by receiver
        I2C_W_SCL(0);                  // ready for next put data on SDA
    }
}

uint8_t I2C_ReceiveByte() {
    uint8_t Data = 0x00;
    I2C_W_SDA(1); // release SDA
    for (int i = 0; i < 8; ++i) {
        I2C_W_SCL(1); // ready for read data from SDA
        // Data |= (I2C_R_SDA() & (0x80 >> i)); // don't do like this. it is better to combine 0/1 which we can control rather than who transfer to us
        if (I2C_R_SDA()) { // don't write "I2C_R_SDA() == 1" when there is no need to get the specific value of something
            Data |= (0x80 >> i);
        }
        I2C_W_SCL(0); // ready for next put data on SDA
    }
    return Data;
}

void I2C_Stop() {
    I2C_W_SDA(0);
    I2C_W_SCL(1);
    I2C_W_SDA(1);
}
void I2C_SendAck(uint8_t AckBit) { // equal to send single time
    I2C_W_SDA(AckBit);
    I2C_W_SCL(1);
    I2C_W_SCL(0);
}

uint8_t I2C_ReceiveAck() { // equal to receive single time
    I2C_W_SDA(1);          // release SDA
    I2C_W_SCL(1);
    uint8_t AckBit = I2C_R_SDA();
    I2C_W_SCL(0);
    return AckBit;
}

//-----------------------------------------------------------------

void MLX90614_Init() {
    I2C_Initialize();
    //...其余电源,精度等方面设置
}

uint16_t MLX90614_ReadReg(uint8_t RegAddress) {
    I2C_Start();
    I2C_SendByte(MLX90614_ADDRESS); // slave address + write(0)
    I2C_ReceiveAck();
    I2C_SendByte(RegAddress);
    I2C_ReceiveAck();
    I2C_Start();
    I2C_SendByte(MLX90614_ADDRESS + 1); // slave address + read(1)
    I2C_ReceiveAck();
    uint8_t DataL = I2C_ReceiveByte();
    I2C_SendAck(0); // receive succeed
    uint8_t DataH = I2C_ReceiveByte();
    I2C_SendAck(0);
    uint8_t PECBit = I2C_ReceiveByte();
    I2C_SendAck(1); // 1 = no nack
    I2C_Stop();

    uint16_t Data = (DataH << 8) | DataL;
    return Data;
}

float MLX90614_GetData() {
    uint16_t Data = MLX90614_ReadReg(MLX90614_RAM_ACCESS | MLX90614_RAM_TOBJ1);
    float Temp = Data * 0.02 - 273.15;
    return Temp;
}
/**
 * IIC命令：开始，停止，发送字节，接收字节，发送应答，接收应答，读寄存器，写寄存器
 * 获取温度只用读寄存器
 * 发送与接收可以分为字节收发（数据与命令）与位收发（应答），更好的办法是将应答写入数据收发函数中，可以用于判断收发是否成功
 * 数据类型一定要考虑清楚，一般情况下使用8位，寄存器一般使用32位，整个数据使用16位
 * 如果只是为了判断是否为0，只用写if(xx)或if(xx!=0)，尽量不使用if(xx==1)因为0x10不为0，但是if(xx==1) 会判断为假，从而进入为0的结果执行
 * 发数据用data&(0x80>>i),表示逐个取出某位，接收数据先令dada=0x00,再将接到的位不为0的赋值Data |= (0x80 >> i)，而不是将接到的数据与data或与，数位不一样容易出错，用自己能把握的值更好
 */
