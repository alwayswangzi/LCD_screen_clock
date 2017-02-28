#include "LED_Display.h"

#define STD_LIGHT 3
extern unsigned char Move,Speed,line;  //移位	速度	 行
extern unsigned long Data_R[16];		//当前屏幕数据-红色
extern unsigned long Data_G[16];		//当前屏幕数据-蓝色
extern unsigned char h,min;			//小时，分钟
extern unsigned char dot;				//时钟闪烁的点
extern unsigned char light;
extern unsigned char color;
extern unsigned char item_page;
//extern unsigned char alarm_switch;
//extern unsigned int temp;		//温度
//extern unsigned char alarm_hour;
//extern unsigned char alarm_min;
//extern unsigned char set_alarm_n1;
//extern unsigned char set_alarm_n2;
//extern unsigned char set_alarm_n3;
//extern unsigned char set_alarm_n4;
extern unsigned char set_time_n1;
extern unsigned char set_time_n2;
extern unsigned char set_time_n3;
extern unsigned char set_time_n4;

const unsigned char NUM[10][16];//数字字模8*16
const unsigned char NUM_SMALL[11][8];
const unsigned long WEEK[7];

void delay(unsigned char x)
{
	unsigned char i,j;
	for(i=0;i<x;i++)
	for(j=0;j<72;j++);


}
void LED_Screen_Init(void)			//点阵屏初始化
{
	GPIO_InitTypeDef GPIO_Stru;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);

	GPIO_Stru.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Stru.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_Stru.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOC,&GPIO_Stru);

	GPIO_Stru.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_Stru);
}

void scan(unsigned char Value)	//不影响其他端口的扫描
{
	switch(Value)
    {
		case  0: scan0;break;
		case  1: scan1;break;
		case  2: scan2;break;
		case  3: scan3;break;
        case  4: scan4;break;
		case  5: scan5;break;
		case  6: scan6;break;
		case  7: scan7;break;
        case  8: scan8;break;
		case  9: scan9;break;
		case 10:scan10;break;
		case 11:scan11;break;
        case 12:scan12;break;
		case 13:scan13;break;
		case 14:scan14;break;
	    case 15:scan15;break;
	    default:break;
      }
}

void rxd_data(unsigned char l)		     //串行发送数据
{
  unsigned char k;
  unsigned long temp_r=Data_R[l];	//每一行暂存的数据32位-红色
  unsigned long temp_g=Data_G[l];	//每一行暂存的数据32位-蓝色
  for(k=0;k<32;k++)		//一行32位 发送给595
  {
	  SHCP_0;
	  if(temp_r&0x00000001)
		  R_0;				//低电平点亮LED
	  else
		  R_1;
	  if(temp_g&0x00000001)
		  G_0;
	  else
		  G_1;

	temp_r=temp_r>>1;  	//字节移位 准备送下一位
	temp_g=temp_g>>1;
	SHCP_1;				//上升沿移位595
  }

}

void load_data_num(unsigned int num)
{
	unsigned char n1,n2,n3,n4;	//代表每一个数字
	unsigned char i;			//代表当前行（共16行）

	n1=num/1000;			//数字分离
	n2=num%1000/100;
	n3=num%100/10;
	n4=num%10;

	for(i=0;i<16;i++)
	{
		Data_R[i]=0;		//每一行的数据（32位）由4个该行的数字字模（8位）组成
		Data_G[i]=0;
		Data_G[i]=NUM[n1][i]|(NUM[n2][i]<<7)|(NUM[n3][i]<<17)|(NUM[n4][i]<<24);
		Data_G[13]|=0x18000;
		Data_G[14]|=0x18000;
	}

}


void Display_Time(void)	//显示时间（24小时制）
{
	unsigned char n1,n2,n3,n4;	//代表每一个数字
	unsigned char i;			//代表当前行（共16行）

	n1=h/10;			//数字分离
	n2=h%10;
	n3=min/10;
	n4=min%10;

	for(i=0;i<16;i++)
	{
		Data_R[i]=0;		//每一行的数据（32位）由4个该行的数字字模（8位）组成
		Data_G[i]=0;
		if(color)
		{
			if(n1)
				Data_G[i]=NUM[n1][i]<<1|(NUM[n2][i]<<7)|(NUM[n3][i]<<17)|(NUM[n4][i]<<24);
			else
				Data_G[i]=			 (NUM[n2][i]<<7)|(NUM[n3][i]<<17)|(NUM[n4][i]<<24);

		}
		else
		{
			if(n1)
				Data_R[i]=NUM[n1][i]<<1|(NUM[n2][i]<<7)|(NUM[n3][i]<<17)|(NUM[n4][i]<<24);
			else
				Data_R[i]=			 (NUM[n2][i]<<7)|(NUM[n3][i]<<17)|(NUM[n4][i]<<24);
		}


	}

	if(dot)			//时间中间点每秒闪烁一次
	{

		if(color)
		{
			Data_G[5]|=0x18000;
			Data_G[6]|=0x18000;
			Data_G[10]|=0x18000;
			Data_G[11]|=0x18000;
		}
		else
		{
			Data_R[5]|=0x18000;
			Data_R[6]|=0x18000;
			Data_R[10]|=0x18000;
			Data_R[11]|=0x18000;
		}

	}

	for(line=0;line<16;line++)
	{
	    rxd_data(line); //装载移动数据
	    scan(line); //输出行扫描

	    STCP_1;//锁存
	    STCP_0;
	    OE_0;
	    delay(STD_LIGHT);	 //延时，等待一段时间，让这列显示的内容在人眼内产生“视觉暂留”现象。
	    OE_1;
	    delay(light);	 //亮度控制在70%  以免过大电流
	 }
}

void Display_Date(void)
{


}
/*void Display_Temp(void)
{
	load_data_num(temp);
	for(line=0;line<16;line++)
	{
		rxd_data(line); //装载移动数据
		scan(line); //输出行扫描

		STCP_1;//锁存
		STCP_0;
		OE_0;
		delay(STD_LIGHT);	 //延时，等待一段时间，让这列显示的内容在人眼内产生“视觉暂留”现象。
		OE_1;
		delay(light);	 //亮度控制在70%  以免过大电流
	}

}*/

void Display_Item(void)
{
	if(item_page==0)
	{
		if(color)
		{
			Data_G[0]= 0;
			Data_G[1]= 0;
			Data_G[2]= 0;
			Data_G[3]= 0;
			Data_G[4]= 0;
			Data_G[5]= 0;
			Data_G[6]= 0;
			Data_G[8]= 0;
			Data_G[9]= 0;
			Data_G[10]=0;
			Data_G[11]=0;
			Data_G[12]=0;
			Data_G[13]=0;
			Data_G[14]=0;
			Data_G[15]=0;
		}
		else
		{
			Data_R[0]= 1;
			Data_R[1]= 0;
			Data_R[2]= 0;
			Data_R[3]= 0;
			Data_R[4]= 0;
			Data_R[5]= 0;
			Data_R[6]= 0;
			Data_R[7]= 0;
			Data_R[8]= 0;
			Data_R[9]= 0;
		    Data_R[10]=0;
			Data_R[11]=0;
			Data_R[12]=0;
			Data_R[13]=0;
			Data_R[14]=0;
			Data_R[15]=0;
		}

	}
	else if(item_page==1)
	{
		if(color)
		{
			Data_G[0]= 2;
			Data_G[1]= 0;
			Data_G[2]= 0;
			Data_G[3]= 0;
			Data_G[4]= 0;
			Data_G[5]= 0;
			Data_G[6]= 0;
			Data_G[8]= 0;
			Data_G[9]= 0;
			Data_G[10]=0;
			Data_G[11]=0;
			Data_G[12]=0;
			Data_G[13]=0;
			Data_G[14]=0;
			Data_G[15]=0;
		}
		else
		{
			Data_R[0]= 0;
			Data_R[1]= 0;
			Data_R[2]= 0;
			Data_R[3]= 0;
			Data_R[4]= 0;
			Data_R[5]= 0;
			Data_R[6]= 0;
			Data_R[7]= 0;
			Data_R[8]= 0;
			Data_R[9]= 0;
		    Data_R[10]=0;
			Data_R[11]=0;
			Data_R[12]=0;
			Data_R[13]=0;
			Data_R[14]=0;
			Data_R[15]=0;
		}
	}
	else
	{
		if(color)
		{
			Data_G[0]= 0;
			Data_G[1]= 0;
			Data_G[2]= 0;
			Data_G[3]= 0;
			Data_G[4]= 0;
			Data_G[5]= 0;
			Data_G[6]= 0;
			Data_G[8]= 0;
			Data_G[9]= 0;
			Data_G[10]=0;
			Data_G[11]=0;
			Data_G[12]=0;
			Data_G[13]=0;
			Data_G[14]=0;
			Data_G[15]=0;
		}
		else
		{
			Data_R[0]= 4;
			Data_R[1]= 0;
			Data_R[2]= 0;
			Data_R[3]= 0;
			Data_R[4]= 0;
			Data_R[5]= 0;
			Data_R[6]= 0;
			Data_R[7]= 0;
			Data_R[8]= 0;
			Data_R[9]= 0;
		    Data_R[10]=0;
			Data_R[11]=0;
			Data_R[12]=0;
			Data_R[13]=0;
			Data_R[14]=0;
			Data_R[15]=0;
		}

	}

	for(line=0;line<16;line++)
	{
		rxd_data(line); //装载移动数据
		scan(line); //输出行扫描

		STCP_1;//锁存
		STCP_0;
		OE_0;
		delay(STD_LIGHT);	 //延时，等待一段时间，让这列显示的内容在人眼内产生“视觉暂留”现象。
		OE_1;
		delay(light);	 //亮度控制在70%  以免过大电流
	}


}

void Display_Light(void)
{
	unsigned char n1,n2;
	n1=(33-light)/10;
	n2=(33-light)%10;
	if(color)
	{
		Data_G[0]= 0x00000000|(NUM_SMALL[n1][0]<<22)|(NUM_SMALL[n2][0]<<27);
		Data_G[1]= 0x00021024|(NUM_SMALL[n1][1]<<22)|(NUM_SMALL[n2][1]<<27);
		Data_G[2]= 0x00021004|(NUM_SMALL[n1][2]<<22)|(NUM_SMALL[n2][2]<<27);
		Data_G[3]= 0x00077724|(NUM_SMALL[n1][3]<<22)|(NUM_SMALL[n2][3]<<27);
		Data_G[4]= 0x00025524|(NUM_SMALL[n1][4]<<22)|(NUM_SMALL[n2][4]<<27);
		Data_G[5]= 0x00025724|(NUM_SMALL[n1][5]<<22)|(NUM_SMALL[n2][5]<<27);
		Data_G[6]= 0x00025424|(NUM_SMALL[n1][6]<<22)|(NUM_SMALL[n2][6]<<27);
		Data_G[7]= 0x00000700|(NUM_SMALL[n1][7]<<22)|(NUM_SMALL[n2][7]<<27);

		Data_G[8]= 0;
		Data_G[9]= 0;
		Data_G[10]=3<<(31-light);
		Data_G[11]=0xffffffff;
		Data_G[12]=3<<(31-light);
		Data_G[13]=0;
		Data_G[14]=0;
		Data_G[15]=0;
	}
	else
	{
		Data_R[0]= 0x00000000|(NUM_SMALL[n1][0]<<22)|(NUM_SMALL[n2][0]<<27);
		Data_R[1]= 0x00021024|(NUM_SMALL[n1][1]<<22)|(NUM_SMALL[n2][1]<<27);
		Data_R[2]= 0x00021004|(NUM_SMALL[n1][2]<<22)|(NUM_SMALL[n2][2]<<27);
		Data_R[3]= 0x00077724|(NUM_SMALL[n1][3]<<22)|(NUM_SMALL[n2][3]<<27);
		Data_R[4]= 0x00025524|(NUM_SMALL[n1][4]<<22)|(NUM_SMALL[n2][4]<<27);
		Data_R[5]= 0x00025724|(NUM_SMALL[n1][5]<<22)|(NUM_SMALL[n2][5]<<27);
		Data_R[6]= 0x00025424|(NUM_SMALL[n1][6]<<22)|(NUM_SMALL[n2][6]<<27);
		Data_R[7]= 0x00000700|(NUM_SMALL[n1][7]<<22)|(NUM_SMALL[n2][7]<<27);

		Data_R[8]= 0;
		Data_R[9]= 0;
		Data_R[10]=3<<(31-light);
		Data_R[11]=0xffffffff;
		Data_R[12]=3<<(31-light);
		Data_R[13]=0;
		Data_R[14]=0;
		Data_R[15]=0;
	}
	for(line=0;line<16;line++)
	{
		rxd_data(line); //装载移动数据
		scan(line); //输出行扫描

		STCP_1;//锁存
		STCP_0;
		OE_0;
		delay(STD_LIGHT);	 //延时，等待一段时间，让这列显示的内容在人眼内产生“视觉暂留”现象。
		OE_1;
		delay(light);	 //亮度控制在70%  以免过大电流
	}
}


/*void Display_Alarm(unsigned char step,unsigned char value)
{
	if(color)
	{
		Data_G[0]=0x00000000;
		Data_G[1]=0x000172e0;
		Data_G[2]=0x00f14280;
		Data_G[3]=0x00d772e0;
		Data_G[4]=0x00d152a0;
		Data_G[5]=0x009152a0;
		Data_G[6]=0x009172e0;
		Data_G[7]=0x00000000;

		if(step==0)
			{
				Data_G[8]= 0;
				Data_G[9]= 0x00006000;
				Data_G[10]=0x00006000;
				Data_G[11]=0x00e70770;
				Data_G[12]=0x00006000;
				Data_G[13]=0x00006000;
				Data_G[14]=0;
				Data_G[15]=0;
			}
			else if(step==1)
			{
				Data_G[8]|= NUM_SMALL[value][0]<<3;
				Data_G[9]|= NUM_SMALL[value][1]<<3;
				Data_G[10]|=NUM_SMALL[value][2]<<3;
				Data_G[11]&=0xffffff8f;	Data_G[11]|=NUM_SMALL[value][3]<<3;
				Data_G[12]|=NUM_SMALL[value][4]<<3;
				Data_G[13]|=NUM_SMALL[value][5]<<3;
				Data_G[14]|=NUM_SMALL[value][6]<<3;
				Data_G[15]|=NUM_SMALL[value][7]<<3;

				set_alarm_n1=value;
			}
			else if(step==2)
			{
				Data_G[8]|= NUM_SMALL[value][0]<<8;
				Data_G[9]|= NUM_SMALL[value][1]<<8;
				Data_G[10]|=NUM_SMALL[value][2]<<8;
				Data_G[11]&=0xfffff8ff;	Data_G[11]|=NUM_SMALL[value][3]<<8;
				Data_G[12]|=NUM_SMALL[value][4]<<8;
				Data_G[13]|=NUM_SMALL[value][5]<<8;
				Data_G[14]|=NUM_SMALL[value][6]<<8;
				Data_G[15]|=NUM_SMALL[value][7]<<8;

				set_alarm_n2=value;
			}
			else if(step==3)
			{
				Data_G[8]|= NUM_SMALL[value][0]<<16;
				Data_G[9]|= NUM_SMALL[value][1]<<16;
				Data_G[10]|=NUM_SMALL[value][2]<<16;
				Data_G[11]&=0xfff8ffff;Data_G[11]|=NUM_SMALL[value][3]<<16;
				Data_G[12]|=NUM_SMALL[value][4]<<16;
				Data_G[13]|=NUM_SMALL[value][5]<<16;
				Data_G[14]|=NUM_SMALL[value][6]<<16;
				Data_G[15]|=NUM_SMALL[value][7]<<16;

				set_alarm_n3=value;
			}
			else if(step==4)
			{
				Data_G[8]|= NUM_SMALL[value][0]<<21;
				Data_G[9]|= NUM_SMALL[value][1]<<21;
				Data_G[10]|=NUM_SMALL[value][2]<<21;
				Data_G[11]&=0xff1fffff;Data_G[11]|=NUM_SMALL[value][3]<<21;
				Data_G[12]|=NUM_SMALL[value][4]<<21;
				Data_G[13]|=NUM_SMALL[value][5]<<21;
				Data_G[14]|=NUM_SMALL[value][6]<<21;
				Data_G[15]|=NUM_SMALL[value][7]<<21;

				set_alarm_n2=value;
			}

	}
	else
	{
		Data_R[0]=0x00000000;
		Data_R[1]=0x000172e0;
		Data_R[2]=0x00f14280;
		Data_R[3]=0x00d772e0;
		Data_R[4]=0x00d152a0;
		Data_R[5]=0x009152a0;
		Data_R[6]=0x009172e0;
		Data_R[7]=0x00000000;

		if(step==0)
			{

				Data_R[8]= 0;
				Data_R[9]= 0x00006000;
				Data_R[10]=0x00006000;
				Data_R[11]=0x00e70770;
				Data_R[12]=0x00006000;
				Data_R[13]=0x00006000;
				Data_R[14]=0;
				Data_R[15]=0;


			}
			else if(step==1)
			{
				Data_R[8]|= NUM_SMALL[value][0]<<3;
				Data_R[9]|= NUM_SMALL[value][1]<<3;
				Data_R[10]|=NUM_SMALL[value][2]<<3;
				Data_R[11]&=0xffffff8f;	Data_R[11]|=NUM_SMALL[value][3]<<3;
				Data_R[12]|=NUM_SMALL[value][4]<<3;
				Data_R[13]|=NUM_SMALL[value][5]<<3;
				Data_R[14]|=NUM_SMALL[value][6]<<3;
				Data_R[15]|=NUM_SMALL[value][7]<<3;

				set_alarm_n1=value;
			}
			else if(step==2)
			{
				Data_R[8]|= NUM_SMALL[value][0]<<8;
				Data_R[9]|= NUM_SMALL[value][1]<<8;
				Data_R[10]|=NUM_SMALL[value][2]<<8;
				Data_R[11]&=0xfffff8ff;	Data_R[11]|=NUM_SMALL[value][3]<<8;
				Data_R[12]|=NUM_SMALL[value][4]<<8;
				Data_R[13]|=NUM_SMALL[value][5]<<8;
				Data_R[14]|=NUM_SMALL[value][6]<<8;
				Data_R[15]|=NUM_SMALL[value][7]<<8;

				set_alarm_n2=value;
			}
			else if(step==3)
			{
				Data_R[8]|= NUM_SMALL[value][0]<<16;
				Data_R[9]|= NUM_SMALL[value][1]<<16;
				Data_R[10]|=NUM_SMALL[value][2]<<16;
				Data_R[11]&=0xfff8ffff;Data_R[11]|=NUM_SMALL[value][3]<<16;
				Data_R[12]|=NUM_SMALL[value][4]<<16;
				Data_R[13]|=NUM_SMALL[value][5]<<16;
				Data_R[14]|=NUM_SMALL[value][6]<<16;
				Data_R[15]|=NUM_SMALL[value][7]<<16;

				set_alarm_n3=value;
			}
			else if(step==4)
			{
				Data_R[8]|= NUM_SMALL[value][0]<<21;
				Data_R[9]|= NUM_SMALL[value][1]<<21;
				Data_R[10]|=NUM_SMALL[value][2]<<21;
				Data_R[11]&=0xff1fffff;Data_R[11]|=NUM_SMALL[value][3]<<21;
				Data_R[12]|=NUM_SMALL[value][4]<<21;
				Data_R[13]|=NUM_SMALL[value][5]<<21;
				Data_R[14]|=NUM_SMALL[value][6]<<21;
				Data_R[15]|=NUM_SMALL[value][7]<<21;

				set_alarm_n4=value;
			}

	}

	for(line=0;line<16;line++)
	{
		rxd_data(line); //装载移动数据
		scan(line); //输出行扫描

		STCP_1;//锁存
		STCP_0;
		OE_0;
		delay(STD_LIGHT);	 //延时，等待一段时间，让这列显示的内容在人眼内产生“视觉暂留”现象。
		OE_1;
		delay(light);	 //亮度控制在70%  以免过大电流
	}

}
*/
/*void Display_Alarm_Confirm(void)
{
	if(color)
	{
		Data_G[0]=0;
		Data_G[1]=0xe024c00e;
		Data_G[2]=0x8f204ee2;
		Data_G[3]=0x4d64eaa2;
		Data_G[4]=0x4d244aa2;
		Data_G[5]=0x09244aa2;
		Data_G[6]=0x49244aee;
		Data_G[7]=0;

		Data_G[8]=0;
		Data_G[9]=0x42000018;
		Data_G[10]=0x24000024;
		Data_G[11]=0x18000042;
		Data_G[12]=0x18000042;
		Data_G[13]=0x24000024;
		Data_G[14]=0x42000018;
		Data_G[15]=0;
	}
	else
	{
		Data_R[0]=0;
		Data_R[1]=0xe024c00e;
		Data_R[2]=0x8f204ee2;
		Data_R[3]=0x4d64eaa2;
		Data_R[4]=0x4d244aa2;
		Data_R[5]=0x09244aa2;
		Data_R[6]=0x49244aee;
		Data_R[7]=0;

		Data_R[8]=0;
		Data_R[9]=0x42000018;
		Data_R[10]=0x24000024;
		Data_R[11]=0x18000042;
		Data_R[12]=0x18000042;
		Data_R[13]=0x24000024;
		Data_R[14]=0x42000018;
		Data_R[15]=0;
	}

	for(line=0;line<16;line++)
		{
			rxd_data(line); //装载移动数据
			scan(line); //输出行扫描

			STCP_1;//锁存
			STCP_0;
			OE_0;
			delay(STD_LIGHT);	 //延时，等待一段时间，让这列显示的内容在人眼内产生“视觉暂留”现象。
			OE_1;
			delay(light);	 //亮度控制在70%  以免过大电流
		}
}
*/
void Display_Set_Time(unsigned char step,unsigned char value)
{
	if(color)
	{
		Data_G[0]= 0x00000000;
		Data_G[1]= 0x00002200;
		Data_G[2]= 0x00ef0700;
		Data_G[3]= 0x00ad2200;
		Data_G[4]= 0x00ed2200;
		Data_G[5]= 0x00292200;
		Data_G[6]= 0x00e92600;
		Data_G[7]= 0x00000000;

		if(step==0)
		{

				Data_G[8]= 0;
				Data_G[9]= 0x00006000;
				Data_G[10]=0x00006000;
				Data_G[11]=0x00e70770;
				Data_G[12]=0x00006000;
				Data_G[13]=0x00006000;
				Data_G[14]=0;
				Data_G[15]=0;

		}
		else if(step==1)
		{
				set_time_n1=value;

				Data_G[8]|= NUM_SMALL[value][0]<<3;
				Data_G[9]|= NUM_SMALL[value][1]<<3;
				Data_G[10]|=NUM_SMALL[value][2]<<3;
				Data_G[11]&=0xffffff8f;	Data_G[11]|=NUM_SMALL[value][3]<<3;
				Data_G[12]|=NUM_SMALL[value][4]<<3;
				Data_G[13]|=NUM_SMALL[value][5]<<3;
				Data_G[14]|=NUM_SMALL[value][6]<<3;
				Data_G[15]|=NUM_SMALL[value][7]<<3;

		}
		else if(step==2)
		{
				set_time_n2=value;

				Data_G[8]|= NUM_SMALL[value][0]<<8;
				Data_G[9]|= NUM_SMALL[value][1]<<8;
				Data_G[10]|=NUM_SMALL[value][2]<<8;
				Data_G[11]&=0xfffff8ff;	Data_G[11]|=NUM_SMALL[value][3]<<8;
				Data_G[12]|=NUM_SMALL[value][4]<<8;
				Data_G[13]|=NUM_SMALL[value][5]<<8;
				Data_G[14]|=NUM_SMALL[value][6]<<8;
				Data_G[15]|=NUM_SMALL[value][7]<<8;


		}
		else if(step==3)
		{
				set_time_n3=value;

				Data_G[8]|= NUM_SMALL[value][0]<<16;
				Data_G[9]|= NUM_SMALL[value][1]<<16;
				Data_G[10]|=NUM_SMALL[value][2]<<16;
				Data_G[11]&=0xfff8ffff;Data_G[11]|=NUM_SMALL[value][3]<<16;
				Data_G[12]|=NUM_SMALL[value][4]<<16;
				Data_G[13]|=NUM_SMALL[value][5]<<16;
				Data_G[14]|=NUM_SMALL[value][6]<<16;
				Data_G[15]|=NUM_SMALL[value][7]<<16;

		}
		else if(step==4)
		{
				set_time_n4=value;

				Data_G[8]|= NUM_SMALL[value][0]<<21;
				Data_G[9]|= NUM_SMALL[value][1]<<21;
				Data_G[10]|=NUM_SMALL[value][2]<<21;
				Data_G[11]&=0xff1fffff;Data_G[11]|=NUM_SMALL[value][3]<<21;
				Data_G[12]|=NUM_SMALL[value][4]<<21;
				Data_G[13]|=NUM_SMALL[value][5]<<21;
				Data_G[14]|=NUM_SMALL[value][6]<<21;
				Data_G[15]|=NUM_SMALL[value][7]<<21;

		}
	}

	else
	{
		Data_R[0]= 0x00000000;
		Data_R[1]= 0x00002200;
		Data_R[2]= 0x00ef0700;
		Data_R[3]= 0x00ad2200;
		Data_R[4]= 0x00ed2200;
		Data_R[5]= 0x00292200;
		Data_R[6]= 0x00e92600;
		Data_R[7]= 0x00000000;

		if(step==0)
		{

			Data_R[8]= 0;
			Data_R[9]= 0x00006000;
			Data_R[10]=0x00006000;
			Data_R[11]=0x00e70770;
			Data_R[12]=0x00006000;
			Data_R[13]=0x00006000;
			Data_R[14]=0;
			Data_R[15]=0;

		}
		else if(step==1)
		{
			set_time_n1=value;

			Data_R[8]|= NUM_SMALL[value][0]<<3;
			Data_R[9]|= NUM_SMALL[value][1]<<3;
			Data_R[10]|=NUM_SMALL[value][2]<<3;
			Data_R[11]&=0xffffff8f;	Data_R[11]|=NUM_SMALL[value][3]<<3;
			Data_R[12]|=NUM_SMALL[value][4]<<3;
			Data_R[13]|=NUM_SMALL[value][5]<<3;
			Data_R[14]|=NUM_SMALL[value][6]<<3;
			Data_R[15]|=NUM_SMALL[value][7]<<3;

		}
		else if(step==2)
		{
			set_time_n2=value;

			Data_R[8]|= NUM_SMALL[value][0]<<8;
			Data_R[9]|= NUM_SMALL[value][1]<<8;
			Data_R[10]|=NUM_SMALL[value][2]<<8;
			Data_R[11]&=0xfffff8ff;	Data_R[11]|=NUM_SMALL[value][3]<<8;
			Data_R[12]|=NUM_SMALL[value][4]<<8;
			Data_R[13]|=NUM_SMALL[value][5]<<8;
			Data_R[14]|=NUM_SMALL[value][6]<<8;
			Data_R[15]|=NUM_SMALL[value][7]<<8;


		}
		else if(step==3)
		{
			set_time_n3=value;

			Data_R[8]|= NUM_SMALL[value][0]<<16;
			Data_R[9]|= NUM_SMALL[value][1]<<16;
			Data_R[10]|=NUM_SMALL[value][2]<<16;
			Data_R[11]&=0xfff8ffff;Data_R[11]|=NUM_SMALL[value][3]<<16;
			Data_R[12]|=NUM_SMALL[value][4]<<16;
			Data_R[13]|=NUM_SMALL[value][5]<<16;
			Data_R[14]|=NUM_SMALL[value][6]<<16;
			Data_R[15]|=NUM_SMALL[value][7]<<16;

		}
		else if(step==4)
		{
			set_time_n4=value;

			Data_R[8]|= NUM_SMALL[value][0]<<21;
			Data_R[9]|= NUM_SMALL[value][1]<<21;
			Data_R[10]|=NUM_SMALL[value][2]<<21;
			Data_R[11]&=0xff1fffff;Data_R[11]|=NUM_SMALL[value][3]<<21;
			Data_R[12]|=NUM_SMALL[value][4]<<21;
			Data_R[13]|=NUM_SMALL[value][5]<<21;
			Data_R[14]|=NUM_SMALL[value][6]<<21;
			Data_R[15]|=NUM_SMALL[value][7]<<21;

		}
	}
	for(line=0;line<16;line++)
	{
		rxd_data(line); //装载移动数据
		scan(line); //输出行扫描

		STCP_1;//锁存
		STCP_0;
		OE_0;
		delay(STD_LIGHT);	 //延时，等待一段时间，让这列显示的内容在人眼内产生“视觉暂留”现象。
		OE_1;
		delay(light);	 //亮度控制在70%  以免过大电流
	}

}


const unsigned char NUM[10][16]=	//数字字模8*16
{


		{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"0",0*/
		{0x00,0x00,0x00,0x08,0x0E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"1",1*/
		{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x20,0x20,0x10,0x08,0x04,0x42,0x7E,0x00,0x00},/*"2",2*/
		{0x00,0x00,0x00,0x3C,0x42,0x42,0x20,0x18,0x20,0x40,0x40,0x42,0x22,0x1C,0x00,0x00},/*"3",3*/
		{0x00,0x00,0x00,0x20,0x30,0x28,0x24,0x24,0x22,0x22,0x7E,0x20,0x20,0x78,0x00,0x00},/*"4",4*/
		{0x00,0x00,0x00,0x7E,0x02,0x02,0x02,0x1A,0x26,0x40,0x40,0x42,0x22,0x1C,0x00,0x00},/*"5",5*/
		{0x00,0x00,0x00,0x38,0x24,0x02,0x02,0x1A,0x26,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"6",6*/
		{0x00,0x00,0x00,0x7E,0x22,0x22,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00},/*"7",7*/
		{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00},/*"8",8*/
		{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x64,0x58,0x40,0x40,0x24,0x1C,0x00,0x00},/*"9",9*/

};

const unsigned char NUM_SMALL[11][8]=	//数字字模4*8
{

		{0x00,0x06,0x09,0x09,0x09,0x09,0x09,0x06},/*"0",0*/
		{0x00,0x02,0x03,0x02,0x02,0x02,0x02,0x07},/*"1",1*/
		{0x00,0x06,0x09,0x08,0x04,0x02,0x01,0x0F},/*"2",2*/
		{0x00,0x07,0x08,0x08,0x07,0x08,0x08,0x07},/*"3",3*/
		{0x00,0x04,0x06,0x06,0x05,0x05,0x0F,0x04},/*"4",4*/
		{0x00,0x0e,0x02,0x02,0x06,0x08,0x08,0x06},/*"5",5*/
		{0x00,0x06,0x09,0x01,0x07,0x09,0x09,0x06},/*"6",6*/
		{0x00,0x0F,0x09,0x08,0x04,0x02,0x02,0x02},/*"7",7*/
		{0x00,0x06,0x09,0x09,0x06,0x09,0x09,0x06},/*"8",8*/
		{0x00,0x06,0x09,0x09,0x0E,0x08,0x09,0x06},/*"9",9*/
		{0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00},/*".",10*/

};

const unsigned long WEEK[7]=
{


};

