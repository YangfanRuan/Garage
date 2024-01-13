#include "Serial.h"

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//只有串口1是APB2，串口2是APB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//PA9,PA10是串口收发引脚，时钟也需要开启

	USART_InitTypeDef USART_InitStructure;//配置串口模式
	USART_InitStructure.USART_BaudRate = 115200;//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx;//串口模式，若既要发送，又要接受，可以用“|”
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验位，无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位，1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长，本次没有校验位，选8位即可
	USART_Init(USART1, &USART_InitStructure);
	
	GPIO_InitTypeDef GPIO_InitStructure;//配置端口模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//发送引脚PA9，输出模式，选推挽复用
	GPIO_InitStructure.GPIO_Speed=  GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_Cmd(USART1, ENABLE);//使能串口，比普通IO使能时钟多一步

	//接收引脚PA10，输入模式（不分普通输入、复用输入，一根线只能有一个输出，但可以有多个输入）
	//所以输入脚，GPIO和外设可以同时使用，输入脚一般配浮空输入或上拉输入
	//串口波形空闲状态是高电平，所以不使用下拉输入
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);//等待数据发送完毕，没发送完flag为空，一只等待在这里，发送完flag为1，进入下一句
	//标志位不需要手动清零，下一次再次发送会自动清零
}

void Serial_SendArray(uint8_t* Array, uint16_t Lenth)
{
	for( uint16_t i = 0; i < Lenth; ++i)
		Serial_SendByte(Array[i]);
}

void Serial_SendString(char* String)//字符串使用uint8_t也可以，字符串自带结束标志位，不需要长度
{	
	for( uint8_t i = 0; String[i] !='\0'; ++i)//'\0'表示结尾标志符，直接使用0也可以
		Serial_SendByte(String[i]);
}

uint32_t Serial_Pow(uint32_t x, uint32_t n) //次方函数，用在Serial_SendNumber函数中
{
	uint32_t Result = 1;
	while(n--)
		Result *= x;
	return Result;
}
void Serial_SendNumber(uint32_t Number,uint8_t Lenth)//发送十进制型的数字，思路：逐个拆出每个位上的数字进行输出
{
	for( uint8_t i = 0; i < Lenth; ++i)
		Serial_SendByte(Number / Serial_Pow(10, Lenth - i - 1) % 10 + '0');
	// + '0'表示以字符的形式显示，换成+ 0x30亦可
	// 从高位到低位输出，i是从低到高，因此使用次方的时候是Lenth - i - 1
	//取某一位：Number/10^x%10,x表示该位所在数位后面有几个0，/10^x表示把右边去掉，%10表示把左边去掉
}

//int fputc(int ch, FILE *f)//将fputc重定向到串口,fputc是printf函数的底层，这个函数本来在stdio.h中包含，这里是重写，因此不需要在Serial.h文件中声明
//{
//	Serial_SendByte(ch);
//	return ch;
//}
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}


void Serial_Sprintf(char* format, ...)//format用于接收格式化字符串，...用于接收可变参数列表
{
	char String[100];//自己定义的存放内容的数组长度
	va_list arg;//定义一个参数列表变量，va_list是一个类型名
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

