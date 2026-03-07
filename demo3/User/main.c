/*头文件引用区*/
#include "debug.h"
#include "lcd.h"
#include "pic.h"
#include "timer.h"
#include "key.h"

/*函数声明区*/
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//定时器3快速中断
u8  string_check(u8* string1,u8* string2,u8 len);


/*变量声明区*/
unsigned long int uwtick;
u8 num;
u8 key_val,key_old,key_down,key_up;
u8 password[6]={5,5,5,5,2,0};
u8 password_cmd[6]={2,7,7,5,1,6};
u8 key_temp[7]={0};
u8 key_index;
u8 key_index_old;

/*按键处理函数*/
void key_proc()
{
  key_val=key_read();
  key_down=key_val&(key_val^key_old);
  key_up=~key_val&(key_val^key_old);
  key_old=key_val;

  switch(key_down)
    {
        case 1:
        key_temp[key_index]=1;
        key_index++;
        break;
        case 2:
        key_temp[key_index]=2;
        key_index++;
        break;
        case 3:
        key_temp[key_index]=3;
        key_index++;
        break;
        case 4:

        break;
        case 5:
        key_temp[key_index]=4;
        key_index++;
        break;
        case 6:
        key_temp[key_index]=5;
        key_index++;
        break;
        case 7:
        key_temp[key_index]=6;
        key_index++;
        break;
        case 8:

        break;
        case 9:
        key_temp[key_index]=7;
        key_index++;
        break;
        case 10:
        key_temp[key_index]=8;
        key_index++;
        break;
        case 11:
        key_temp[key_index]=9;
        key_index++;
        break;
        case 12:

        break;
        case 13:
           key_index=0;
           lock(1);
        break;
        case 14:
        key_temp[key_index]=0;
        key_index++;
        break;
        case 15:

        break;
        case 16:
            if( string_check(key_temp,password,6) )lock(0);
            else
            {
                lock(1);
                key_index = 0;
            }
                break;


    }
}


/*屏幕处理函数*/
void lcd_proc()
{
    if(key_index!=key_index_old)
        {
            u8 i=key_index;
             LCD_Fill(16,45,112,66,YELLOW);//填充黄色背景
             if(key_index==7)key_index=6;
             while(i--)
             {
                 if(i<6)
                 //LCD_ShowChar(20+16*i,45,key_temp[i]+48,RED,YELLOW,16,0);
                 LCD_ShowChar(20+16*i,45,'*',RED,YELLOW,16,0);
             }
             key_index_old=key_index;
        }

}

/*系统计时定时器3中断服务函数*/
void TIM3_IRQHandler(void)//每1毫秒
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)
        {
         uwtick++;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
/*任务调度器相关*/
typedef struct
{
    void (*task_func)(void);//任务函数
    unsigned long int rate_ms;  //任务执行周期
    unsigned long int last_run; //任务上次运行的时间
}task_t;

task_t scheduler_task[]={
        {lcd_proc,100,0},//屏幕处理函数，100毫秒执行一次，0秒开始执行
        {key_proc,10,0},//按键处理函数，10毫秒执行一次，0秒开始执行
};
unsigned char task_num;
void scheduler_init()
{
    task_num=sizeof(scheduler_task)/sizeof(task_t);
}
void scheduler_run()
{
    unsigned char i;
    for(i=0;i<task_num;i++)
    {
        unsigned long int now_time=uwtick;
        if(now_time> (scheduler_task[i].last_run+scheduler_task[i].rate_ms) )
        {
            scheduler_task[i].last_run=now_time;
            scheduler_task[i].task_func();
        }
    }
}

/*主函数*/
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级分组
    SystemCoreClockUpdate();//时钟配置
    Delay_Init();//延时函数初始化
    TIM2_PWM_Init();//舵机初始化（TIM2输出PWM）
    lock(1);//初始默认上锁

    LCD_Init();//屏幕初始化
    LCD_Fill(0,0,127,127,WHITE);//整个屏幕填充白色
    lcd_show_chinese(32,50,"正在启动",RED,WHITE,16,0);//正在启动
    unsigned char i=0;//循环次数
    while(i<128)//循环划线实现进度条的效果
    {
        LCD_DrawLine(i,100,i,128,RED);
        Delay_Ms(70);
        i++;
    }
    lcd_show_chinese(32,50,"启动成功",RED,WHITE,16,0);//开机成功
    Delay_Ms(500);
    LCD_ShowPicture(0,0,128,128,gImage_conan);//显示logo
    Delay_Ms(1500);
    LCD_ShowPicture(0,0,128,128,gImage_lock);//显示主页
    lcd_show_chinese(0,0,"门已上锁，请输入密码",RED,WHITE,16,0);//开机成功
    LCD_Fill(16,45,112,66,YELLOW);//填充黄色背景

    key_init();//矩阵按键初始化
    TIM3_Init(999,96-1);//系统计时定时器初始化
    scheduler_init();
    while(1)
    {
        scheduler_run();

    }
}

u8  string_check(u8* string1,u8* string2,u8 len)
{
    while(len--)
    {
        if(string1[len] == string2[len]);
        else return 0;
    }
    return 1;
}



