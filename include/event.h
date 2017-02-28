#ifndef __EVENT_H
#define __EVENT_H

#define BEEP_ON GPIOB->BSRR=GPIO_Pin_6	//·äÃùÆ÷Êä³öÒý½Å
#define BEEP_OFF GPIOB->BRR=GPIO_Pin_6
#define ITEM_BRILIGHT 0
#define ITEM_TIME_SET 1
#define ITEM_DATE 	  2
#define POWER_ON 0
#define POWER_OFF 1
#include "remote.h"
#include "delay.h"
#include "LED_Display.h"

void Scanning(void);
void Item_Display(void);
void Function_Display(void);
void Mode_FFT(void);

#endif
