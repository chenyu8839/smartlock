#include "debug.h"
#include "lcd.h"
#include "pic.h"


// 定义舵机的PWM周期和占空比
#define PWM_PERIOD 20000  // PWM周期为20ms (20000us)
#define PWM_MIN 500       // 最小脉宽为0.5ms (500us)
#define PWM_MAX 2500      // 最大脉宽为2.5ms (2500us)

// 初始化TIM2以生成PWM信号
void TIM2_PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // 使能TIM2时钟

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 配置TIM2的基本参数
    TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD - 1;  // 设置自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = 96 - 1;       // 设置预分频器，96MHz/96 = 1MHz (1us)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 配置TIM2的PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PWM_MIN;  // 初始占空比为最小值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // 配置TIM2通道1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);  // 使用TIM2的通道1
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  // 使能TIM2通道1的预装载寄存器


    TIM_ARRPreloadConfig(TIM2, ENABLE);               // 使能TIM2的自动重装寄存器预装载

    TIM_Cmd(TIM2, ENABLE);  // 使能TIM2
}

// 初始化GPIO以输出PWM信号
void GPIO_Init_PWM(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 使能GPIOA时钟

    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置PA0为复用推挽输出（TIM2通道1）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();

	TIM2_PWM_Init();
	GPIO_Init_PWM();
	TIM_SetCompare1(TIM2, 2000);

	LCD_Init();
	LCD_Fill(0,0,127,127,WHITE);

	LCD_ShowChinese(32, 70, "正", BLACK, WHITE, 16, 0);
	LCD_ShowChinese(32+16, 70, "在", BLACK, WHITE, 16, 0);
	LCD_ShowChinese(32+32, 70, "启", BLACK, WHITE, 16, 0);
	LCD_ShowChinese(32+48, 70, "动", BLACK, WHITE, 16, 0);
	LCD_ShowPicture(40,0,48,48,gImage_conan);
	//LCD_Fill(10,10,80,80,RED);
	uint8_t i = 0;
	for(i=0;i<128;i++)
	{
	    LCD_DrawLine(i,100,i,127,RED);
	    Delay_Ms(50);
	}

	LCD_ShowChinese(32, 70, "启", RED, WHITE, 16, 0);
    LCD_ShowChinese(32+16, 70, "动", RED, WHITE, 16, 0);
    LCD_ShowChinese(32+32, 70, "成", RED, WHITE, 16, 0);
    LCD_ShowChinese(32+48, 70, "功", RED, WHITE, 16, 0);

    Delay_Ms(1000);

    LCD_ShowPicture(0,0,128,128,gImage_1);

	while(1)
    {

	}
}

