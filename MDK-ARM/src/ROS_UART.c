/**
  ******************************************************************************
  * 文件名          : ROS_UART.c
  * 文件描述        : 用于与ROS之间的串口通讯
  * 创建时间        : 2021.6.13
  * 作者            : 陈凌栩
  ******************************************************************************
  *								文件描述     								   *
  ******************************************************************************
  *	
	* 本文件用于对上传至ROS的数据进行打包以及对ROS发送的指令进行解码
	*
  ******************************************************************************
  */
#include "ROS_UART.h"

#define RxHeader 0x55 			//接受帧头，检测到该帧头才进行下一步解码
#define VelHeader 0x10 			//速度控制标识符，检测到该标识符以速度控制模式控制云台
#define PosHeader 0x20      //位置控制标识符,检测到该标识符以位置控制模式控制云台
#define StopHeader 0x30     //停止标识符,检测到该标识符云台停止运动

uint8_t Receive_Data[20];
uint8_t Transmit_Data[8]={0};



void Vel_Decode(uint8_t* RxBuffer)
{
	Yuntai.Motor[0].ref = (int)((RxBuffer[0]<<8)|(RxBuffer[1]));
	Yuntai.Motor[1].ref = (int)((RxBuffer[2]<<8)|(RxBuffer[3]));
	Yuntai.WorkState = Vel_Mode;
}

void Pos_Decode(uint8_t* RxBuffer)
{
	
}

/**
* @brief 对接受数据进行解码
* @param 
* @retval None
*/
void RxBuffer_Decode(uint8_t* RxBuffer)
{
	int Header_index=0;
	uint8_t Temp_Buffer[6];
	//遍历缓存区数据，寻找帧头
	for(int i=0;i<20;i++)
	{
		if(RxBuffer[i]==RxHeader)
		{
			Header_index=i;
			break;
		}
	}
	
	switch (RxBuffer[Header_index+1])
	{
		case VelHeader:
			for(int i=0;i<6;i++) Temp_Buffer[i]=RxBuffer[Header_index+2+i];
			Vel_Decode(Temp_Buffer);
			break;
		case PosHeader:
			Pos_Decode(Temp_Buffer);
			break;
		case StopHeader:
			Yuntai.WorkState = Stop_Mode;
			break;
	}
}

/**
* @brief 对发送数据进行打包
* @param 
* @retval None
*/
void TxBuffer_Package(uint8_t* TxBuffer,float pos1,float pos2)
{
	int pos1_int = (int)(pos1*100);
	int pos2_int = (int)(pos2*100);
	
	TxBuffer[0] = 0x55;
	TxBuffer[1] = 0x20;
	TxBuffer[2] = pos1_int>>8;
	TxBuffer[3] = pos1_int;
	TxBuffer[4] = pos2_int>>8;
	TxBuffer[5] = pos2_int;
	TxBuffer[6] = 0;
	TxBuffer[7] = 0;
}

