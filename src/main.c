#include "LED_Display.h"	//LED点阵屏显示
#include "RTC_Timer.h"		//RTC时钟
#include "delay.h"			//延时函数
#include "Remote.h"			//红外遥控
#include "event.h"			//任务处理函数
#include "timer.h"			//定时器
unsigned char line;  		// 行
unsigned long Data_R[16];	//当前屏幕数据-红色
unsigned long Data_G[16];	//当前屏幕数据-蓝色
unsigned char h,min;		//小时，分钟
unsigned char dot;			//时钟闪烁的点
unsigned char item;			//菜单命令	0：显示时间	1：显示菜单
unsigned char light;		//亮度级别
unsigned char color;		//字体颜色
unsigned char item_step;	//菜单序号
unsigned char item_page;	//菜单页码
unsigned char bright_step;	//亮度步长
unsigned char single_value; //单个数字值
unsigned char press_count;	//按键计数
unsigned char set_time_n1;	//设置时间	小时 十位
unsigned char set_time_n2;	//设置时间	小时 个位
unsigned char set_time_n3;	//设置时间	分钟 十位
unsigned char set_time_n4;	//设置时间	分钟 个位
unsigned char on_clock;
int main(void)
{
//	RTC_Timer_Init();		//RTC时钟初始化
	delay_init();			//延时函数初始化
	LED_Screen_Init();		//LED点阵屏显示初始化
	Tim3_Init();			//定时器3初始化
	Remote_Init();			//红外遥控初始化
	light=BKP_ReadBackupRegister(BKP_DR2); //从后备寄存器2中读取之前存储的亮度值
	color=BKP_ReadBackupRegister(BKP_DR3);
    while(1)
    {

    	Scanning();				//键盘扫描	判断是否正常显示还是菜单显示
    	while(item)
    	{
    		while(item)			//第一级菜单
    			Item_Display();
    		delay_ms(100);
    		item=1;
    		while(item)		//第二级菜单
    			Function_Display();
    	}

    	Display_Time();			//正常显示

    }
}




