#include "Mission.h"
#include "PID.h"
#include "Yuntai.h"
#include "M2006.h"

void Mission_Start()
{
	switch (Yuntai.WorkState)
	{
		case Stop_Mode:
			Yuntai.Motor[0].ref=0;
			Yuntai.Motor[1].ref=0;
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
			break;
		case Vel_Mode:
			Caculate();
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
			CanTransmit_12(&hcan1, Yuntai.Motor[0].output, Yuntai.Motor[1].output);
			break;
		case Pos_Mode:
			break;
	}
}

