/*头文件引用区*/
#include "debug.h"//必要系统头文件
#include "lcd.h"//屏幕头文件
#include "pic.h"//图片头文件
#include "timer.h"//定时器头文件
#include "key.h"//按键头文件
#include "uart.h"
#include "audio.h"
#include "string.h"

/*函数声明区*/
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//定时器3快速中断
u8  string_check(u8* string1,u8* string2,u8 len);//数组检测函数
u8 rfid_check();//卡片检测函数
void string_copy(u8* string1,u8* string2,u8 len);
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//串口3快速中断
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//串口2快速中断
void key_clear(); //键盘暂存区清空函数
void lock_proc();

/*变量声明区*/
unsigned long int uwtick;
u8 key_val,key_old,key_down,key_up;
u8 password[6]={5,5,5,5,2,0};         //开锁密码
u8 password_cmd[6]={5,2,0,9,5,1};
u8 key_temp[7]={10,10,10,10,10,10};   //按键暂存区
u8 key_index;                         //索引值
u8 key_index_old;
u16 time_5s_lock;                     //开门5s后自动关闭
u8 lock_flag = 1;                     //门开关标志位
u8 password_error_count;              //输入密码错误的次数
u16 time_15s_error = 15;              //输入密码错误3次，锁定15s
u16 time_1s = 0;
u8 show_flag;                         //显示汉字的模式
u8 show_flag_old;
u8 mode = 0;
//模式 0-密码 1-修改密码模式但无权限 2-修改密码但有权限 3-录入卡片没有管理员权限1 4-录入卡片有权限
u8 rfid_index;                        //刷卡模块
u8 rfid_temp[4];
u8 rfid[4][4];
u8 rfid_password_index;

/*按键处理函数*/
void key_proc()
{
  key_val=key_read();
  key_down=key_val&(key_val^key_old);
  key_up=~key_val&(key_val^key_old);
  key_old=key_val;

  if(password_error_count == 3)
  {
      return;
  }

  if(key_down)
  {
      audio_play(1);//滴的一声
  }

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
            if(mode == 0)
            {
              mode = 1;
              audio_play(6);//6修改开锁密码，请输入管理员密码
              password_error_count = 0;
              key_clear();
            }
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
             if(mode == 0)
             {
                 mode = 3;
                 audio_play(13);//13录入卡片，请输入管理员密码
                 key_clear();
             }
        break;
        case 13://清空
                key_clear();
        break;
        case 14:
        key_temp[key_index]=0;
        key_index++;
        break;
        case 15://回退
        if(key_index)
        {
            key_index--;
            key_temp[key_index] = 10;
        }
        break;
        case 16:
            switch(mode)
            {
             case 0://主页
                if( string_check(key_temp,password,6) )
                {
                    lock_flag = 0;//开锁
                    show_flag = 1;//显示汉字门已开启
                    audio_play(3);   //密码正确,欢迎回家
                    key_clear();
                    password_error_count = 0;
                }
                 else
                {
                    audio_play(4);
                    key_clear();
                    if(++password_error_count == 3)
                    {
                        show_flag = 2;
                        audio_play(5);
                    }
                }
              break;
              case 1://修改密码没解锁管理员权限
                  if( string_check(key_temp,password_cmd,6) )
                  {
                      mode = 2;//进入修改密码解锁管理员权限
                      audio_play(7);//密码正确
                      key_clear();
                      password_error_count = 0;
                  }
                  else
                  {
                      audio_play(9);
                      if(++password_error_count == 3)
                      {
                          show_flag = 2;
                          audio_play(10);
                      }
                  }
              break;
              case 2://修改密码解锁管理员权限
                    string_copy(key_temp, password, 6);
                    audio_play(8);
                    mode = 0;
                    key_clear();
              break;
              case 3:
                  if( string_check(key_temp,password_cmd,6) )
                  {
                      mode = 4;//录入卡片解锁管理员权限
                      audio_play(14);//14管理员密码验证成功，请将卡片平放在传感器上
                      key_clear();
                  }
                  else
                  {
                      audio_play(9);
                      if(++password_error_count == 3)
                      {
                          show_flag = 2;
                          audio_play(10);
                      }
                  }
              break;
              case 4:

              break;
            }
        break;
    }
}


/*屏幕处理函数*/
void lcd_proc()
{
    /*黄色密码输入区*/
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

    if(show_flag != show_flag_old)
    {
        LCD_Fill(0,0,128,32,WHITE);//填充白色背景
        show_flag_old = show_flag;
    }
    switch(show_flag)
    {
    case 0:
        lcd_show_chinese(0,0,"门已上锁，请输入密码",RED,WHITE,16,0);
    break;
    case 1:
        lcd_show_chinese(0,0,"门已开启，欢迎回家",RED,WHITE,16,0);
    break;
    case 2:
        LCD_ShowIntNum(50, 16,time_15s_error,2, RED,WHITE,16);
    break;
    }
}



/*系统计时定时器2中断服务函数*/
void TIM3_IRQHandler(void)//每1毫秒
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)//检查指定的中断标志位是否被置位，不等于0 → 说明确实是更新中断触发的
    {
      uwtick++;

      if(lock_flag == 0)
      {
         if(++time_5s_lock >= 5000)
         {
             time_5s_lock = 0;
             lock_flag = 1;//上锁
             show_flag = 0;//显示汉字门已上锁
             audio_play(2);//门已上锁，请刷卡或输入密码解锁
         }
      }

      if(password_error_count == 3)//错误三次
      {
          if(++time_1s == 1000)//1s倒计时
          {
            time_1s = 0;
            if(--time_15s_error == 0)
            {
                password_error_count = 0;
                time_15s_error = 15;
                show_flag = 0;
            }
          }
      }

    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//如果不清除，硬件会认为这个中断还没被处理，会反复触发，程序就卡死在中断里了喵！

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
        {lock_proc,30,0},//门锁处理函数，30毫秒执行一次，0秒开始执行
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
    USART_Printf_Init(115200);
    //printf("hello\r\n");
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
        Delay_Ms(10);
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

    audio_init();
    audio_play(2);

    Usart2_Init();
    while(1)
    {
        scheduler_run();

    }
}

/*门锁处理函数*/
void lock_proc()
{
   lock(lock_flag);
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

void string_copy(u8* string1,u8* string2,u8 len)
{
    u8 i;
    for(i=0;i<len;i++)
    {
        string2[i] = string1[i];
    }
}

u8 rfid_check()
{
    u8 i;
    for (i = 0; i < rfid_password_index; ++i)
    {
        if(string_check(rfid_temp,rfid[i],4))
            return 1;
    }

    return 0;
}

void key_clear() //键盘暂存区清空函数
{
 memset(key_temp,10,6);
 key_index = 0;
}

/*系统计时定时器2中断服务函数*/
void USART2_IRQHandler(void)
{
    u8 temp;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        temp=USART_ReceiveData(USART2);
        switch(rfid_index)
        {
            case 0:
               if(temp == 0x04)rfid_index++;
            break;
            case 1:
               if(temp == 0x0c)rfid_index++;
               else rfid_index = 0;
            break;
            case 2:
               if(temp == 0x02)rfid_index++;
               else rfid_index = 0;
            break;
            case 3:
               if(temp == 0x30)rfid_index++;
               else rfid_index = 0;
            break;
            case 4:
               if(temp == 0x00)rfid_index++;
               else rfid_index = 0;
            break;
            case 5:
               if(temp == 0x04)rfid_index++;
               else rfid_index = 0;
            break;
            case 6:
               if(temp == 0x00)rfid_index++;
               else rfid_index = 0;
            break;
            case 7:
                rfid_temp[0] = temp;
                rfid_index++;
            break;
            case 8:
                rfid_temp[1] = temp;
                rfid_index++;
            break;
            case 9:
                rfid_temp[2] = temp;
                rfid_index++;
            break;
            case 10:
                rfid_temp[3] = temp;
                rfid_index = 0;
                if(mode == 0)
                {
                    if(rfid_check())
                    {
                        audio_play(11);//11刷卡成功，门已开启，欢迎回家
                        lock_flag = 0;//开门
                    }
                    else
                    {
                        audio_play(12);//12刷卡失败，请重试
                        lock_flag = 1;//关门
                    }
                }
                if(mode == 4)
                {
                    string_copy(rfid_temp, rfid[rfid_password_index], 4);
                    audio_play(15);//15卡片添加成功
                    mode = 0;
                    rfid_password_index++;
                }
            break;
        }
    }
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

/*系统计时定时器3中断服务函数*/
void USART3_IRQHandler(void)
{
    u8 temp;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        temp=USART_ReceiveData(USART3);
    }
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}

