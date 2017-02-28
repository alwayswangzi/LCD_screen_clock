#include "RTC_Timer.h"
#include "event.h"
extern unsigned char h,min;	//小时，分钟
extern unsigned char dot;	//闪烁的点
extern unsigned char on_clock;
unsigned char tick;
void RTC_Timer_Init(void)	//RTC配置函数
{
    NVIC_Configuration();	//中断配置函数

    ////*********************************************///
    ////系统复位后，对后备寄存器和RTC的访问被禁止，这是为了防止对后备区域(BKP)的意外写操作。
    ////执行以下操作将使能对后备寄存器和RTC的访问

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RCC_RTCCLKCmd(ENABLE);


    ////*********************************************///


    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)	//如果当前后备寄存器的值不一样说明要更改时钟了
    {
        RTC_Configuration();//RTC配置
        Time_Adjust(0);		//时间调整
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	//使后备寄存器的值与当前值保持一致，防止下次更改
    }
   else	//否则直接运行，不用配置
    {
	   RTC_WaitForSynchro();
	   RTC_ITConfig(RTC_IT_SEC, ENABLE);
       RTC_WaitForLastTask();
    }


}
void RTC_IRQHandler(void)	//中断服务函数，每一秒发生一次中断，送回当前时间数据
{
    unsigned long sec;
    sec=RTC_GetCounter();

    h=(sec%86400)/3600;
    if(h>12) h-=12;

    min=(sec%3600)/60;

    if(dot==0)
       dot=1;
    else
       dot=0;

//    if(min==0 && sec%60==0 &&)
//    	BEEP_ON;
//    else
//    	BEEP_OFF;

    if(on_clock)
    {
    	if(tick==1)
    	{
    		tick=0;
    		BEEP_ON;
    	}
    	else
    	{
    		tick=1;
    		BEEP_OFF;
    	}

    }
    RTC_ClearITPendingBit(RTC_IT_SEC);

}

void NVIC_Configuration(void)	//中断初始化函数
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void RTC_Configuration(void)	//RTC配置函数
{

	BKP_DeInit();//BKP寄存器全部设为缺省值

    RCC_LSEConfig(RCC_LSE_ON);

    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    RCC_RTCCLKCmd(ENABLE);

    RTC_WaitForSynchro();

    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    RTC_WaitForLastTask();

    RTC_EnterConfigMode();/// 允许配置

    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    RTC_WaitForLastTask();

    RTC_ExitConfigMode(); //退出配置模式

}


void Time_Adjust(unsigned long sec)
{
    RTC_WaitForLastTask();
    RTC_EnterConfigMode();/// 允许配置
    RTC_SetCounter(sec);
    RTC_WaitForLastTask();
    RTC_ExitConfigMode(); //退出配置模式
}

