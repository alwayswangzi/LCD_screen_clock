#include "event.h"
#include "RTC_Timer.h"
extern unsigned char h,min;		//小时，分钟
extern unsigned char item;//0：关闭菜单显示	1：菜单显示
extern unsigned char light;
extern unsigned char color;
extern unsigned char bright_step;
extern unsigned char item_step;//1：亮度二级菜单	2：时间二级菜单	3：日期二级菜单
extern unsigned char item_page;//0：亮度一级菜单	1：时间一级菜单	2：日期一级菜单
extern unsigned char bright_step;
extern unsigned char single_value;
extern unsigned char press_count;
extern unsigned char set_time_n1;
extern unsigned char set_time_n2;
extern unsigned char set_time_n3;
extern unsigned char set_time_n4;
void Scanning(void)
{
	switch(Remote_Scan())
	{
	    case KEY_ALIENTEK:	item=~item;	BEEP_ON;delay_ms(200);BEEP_OFF;	break;
	    case KEY_PLAY:
	    {
	    	press_count++;
	    	if(press_count>2)
	    	{
	    		color=~color;
	    		BKP_WriteBackupRegister(BKP_DR3,color);
	    		press_count=0;
	    	}
	    	BEEP_ON;
	    	delay_ms(100);
	    	BEEP_OFF;
	    	break;
	    }
	    default: ;

	}
}


void Item_Display(void)
{
		switch(Remote_Scan())
		{
			case KEY_PLAY:
				item_step=item_page;
				item=0;
			//	BEEP_ON;
				delay_ms(80);
				BEEP_OFF;
				delay_ms(300);
				break;
			case KEY_1:
				item_page=0;
			//	BEEP_ON;
				delay_ms(80);
				BEEP_OFF;
				break;
			case KEY_2:
				item_page=1;
			//	BEEP_ON;
				delay_ms(80);
				BEEP_OFF;
				break;
			case KEY_3:
				item_page=2;
			//	BEEP_ON;
				delay_ms(80);
				BEEP_OFF;
				break;
			case KEY_DELETE:
				item=0;
			//	BEEP_ON;
				delay_ms(80);
				BEEP_OFF;
				break;
			default:;
		}
	Display_Item();
}

void Function_Display(void)
{

	if(item_step==ITEM_BRILIGHT)
	{
		switch(Remote_Scan())
		{
			case KEY_RIGHT:
			{
				press_count++;
				if(press_count>2)
				{
					--light;
					press_count=0;
				}
				if(light<1)
					light=31;
				BEEP_ON;
				delay_ms(50);
				BEEP_OFF;
				break;

			}
			case KEY_LEFT:
			{
				press_count++;
				if(press_count>2)
				{
					++light;
					press_count=0;
				}
				if(light>31)
					light=1;
				BEEP_ON;
				delay_ms(50);
				BEEP_OFF;
				break;
			}
			case KEY_PLAY:
			{
				press_count++;
				if(press_count>3)
				{
					++light;
					press_count=0;
					item=0;
				}
				BEEP_ON;
				delay_ms(100);
				BEEP_OFF;
				BKP_WriteBackupRegister(BKP_DR2,light);
				break;
			}

			default:Display_Light();

		}

	}

	else if(item_step==ITEM_TIME_SET)
	{
		if(bright_step>4)
			bright_step=0;

		switch(Remote_Scan())
		{
			case KEY_0:
				single_value=0; press_count++; if(press_count>2){bright_step++;press_count=0;}BEEP_ON;delay_ms(100);BEEP_OFF;break;
			case KEY_1:
				single_value=1; press_count++; if(press_count>2){bright_step++;press_count=0;}BEEP_ON;delay_ms(100);BEEP_OFF;break;
			case KEY_2:
				single_value=2; press_count++; if(press_count>2){bright_step++;press_count=0;}BEEP_ON;delay_ms(100);BEEP_OFF;break;
			case KEY_3:
				single_value=3; press_count++; if(press_count>2){bright_step++;press_count=0;}BEEP_ON;delay_ms(100);BEEP_OFF;break;
			case KEY_4:
				single_value=4; press_count++; if(press_count>2){bright_step++;press_count=0;}BEEP_ON;delay_ms(100);BEEP_OFF;break;
			case KEY_5:
				single_value=5; press_count++; if(press_count>2){bright_step++;press_count=0;}BEEP_ON;delay_ms(100);BEEP_OFF;break;
			case KEY_6:
				single_value=6; press_count++; if(press_count>2){bright_step++;press_count=0;}BEEP_ON;delay_ms(100);BEEP_OFF;break;
			case KEY_7:
				single_value=7; press_count++; if(press_count>2){bright_step++;press_count=0;}BEEP_ON;delay_ms(100);BEEP_OFF;break;
			case KEY_8:
				single_value=8; press_count++; if(press_count>2){bright_step++;press_count=0;}BEEP_ON;delay_ms(100);BEEP_OFF;break;
			case KEY_9:
				single_value=9; press_count++; if(press_count>2){bright_step++;press_count=0;}BEEP_ON;delay_ms(100);BEEP_OFF;break;

			case KEY_PLAY:
			{
				press_count++;
				if(press_count>3)
				{
					Time_Adjust((set_time_n1*10+set_time_n2)*3600+(set_time_n3*10+set_time_n4)*60);
					item=0;
					press_count=0;
				}
				BEEP_ON;
				delay_ms(100);
				BEEP_OFF;
				break;
			}
			case KEY_DELETE:
			{
				press_count++;
				if(press_count>2)
				{
					Display_Set_Time(0,single_value);
					press_count=0;
				}
				BEEP_ON;
				delay_ms(100);
				BEEP_OFF;
				break;
			}
			default:	Display_Set_Time(bright_step,single_value);
		}
	}

	else if(item_step==ITEM_DATE)
	{

	}


	else
	{
		item=0;
	}
}
void Mode_FFT(void)
{


}
