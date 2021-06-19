/**
  ******************************************************************************
  * �ļ���          : ROS_UART.c
  * �ļ�����        : ������ROS֮��Ĵ���ͨѶ
  * ����ʱ��        : 2021.6.13
  * ����            : ������
  ******************************************************************************
  *								�ļ�����     								   *
  ******************************************************************************
  *	
	* ���ļ����ڶ��ϴ���ROS�����ݽ��д���Լ���ROS���͵�ָ����н���
	*
  ******************************************************************************
  */
#include "ROS_UART.h"

#define RxHeader 0x55 			//����֡ͷ����⵽��֡ͷ�Ž�����һ������
#define VelHeader 0x10 			//�ٶȿ��Ʊ�ʶ������⵽�ñ�ʶ�����ٶȿ���ģʽ������̨
#define PosHeader 0x20      //λ�ÿ��Ʊ�ʶ��,��⵽�ñ�ʶ����λ�ÿ���ģʽ������̨
#define StopHeader 0x30     //ֹͣ��ʶ��,��⵽�ñ�ʶ����ֹ̨ͣ�˶�

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
* @brief �Խ������ݽ��н���
* @param 
* @retval None
*/
void RxBuffer_Decode(uint8_t* RxBuffer)
{
	int Header_index=0;
	uint8_t Temp_Buffer[6];
	//�������������ݣ�Ѱ��֡ͷ
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
* @brief �Է������ݽ��д��
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

