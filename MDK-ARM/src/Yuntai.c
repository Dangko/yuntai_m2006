#include "Yuntai.h"

Yuntai_t Yuntai;														//云台控制相关信息
uint8_t CanReceiveData[8];                  //CAN接收的电机反馈信息

void Yuntai_Init()
{
	for(int i=0;i<2;i++)
	{
		Yuntai.Motor[i].KP = 15;
		Yuntai.Motor[i].KI = 1;
		Yuntai.Motor[i].KD = 1;
		Yuntai.Motor[i].outputMax = 5000;
	}
}
