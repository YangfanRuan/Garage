#include "Delay.h"
#include "Serial.h"
#include "i2c_mlx90614.h"
#include "stm32f10x.h"

int main(void) {
    // 来到这里的时候，系统的时钟已经被配置成72M。
    Serial_Init();
    MLX90614_Init();

    float Temperature = 0; // 温度数据变量（浮点型）
    while (1) {
        Temperature = MLX90614_GetData();
        Temperature = printf("Temper = %.2f\n", Temperature);
        Delay_ms(1000);
    }
}
