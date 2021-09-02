/*******************************************************************************
**                       @ Copyright 2014 - 2020                              **
**                           www.diysoon.com                                  **
**                                                                            **
*******************************************************************************/



#include "L610_TCP.h"




extern char *strx,*extstrx,*Readystrx;
extern 	char RxBuffer[200],Rxcouter;


/*****************************************************

	������Ӫ�̷���IP
	��ӦATָ�AT+mipcall = 
	��Ӫ�̲�ͬ����ָ��������

******************************************************/

void IP_allo(void)
{
	printf("AT+mipcall=1,\"ctnet\"\r\n");//������Ӫ�̷��� IP
	HAL_Delay(3000);
	printf("AT+mipcall?\r\n");//����Ƿ����IP
	HAL_Delay(3000);
	strx=strstr((const char*)RxBuffer,(const char*)"+MIPCALL: 1");//�жϽ����+MIPCALL:1 ��������һ����

	while(strx==NULL)
	{
		Clear_Buffer();
		printf("AT+mipcall=0\r\n");
		HAL_Delay(2000);
		printf("AT+mipcall=1,\"ctnet\"\r\n");
		HAL_Delay(3000);
		printf("AT+mipcall?\r\n");//����Ƿ����IP
		HAL_Delay(3000);
		strx=strstr((const char*)RxBuffer,(const char*)"+MIPCALL: 1");//�жϽ����+MIPCALL:1 ��������һ����
	}
	HAL_Delay(1000);
	Clear_Buffer();
}


/*******************************************************
	
	��ѯ��ǰ���õ�socket ID
	socket ID ������˵����ǰû�� TCP ���ӡ�
	��ӦATָ�AT+MIPOPEN?

********************************************************/

void Check_socket(void)
{
	
	Clear_Buffer();
	printf("AT+MIPOPEN?\r\n"); 
	HAL_Delay(500);
	strx=strstr((const char*)RxBuffer,(const char*)"+MIPOPEN:");//�Ƿ񷵻�
	while(strx==NULL)
	{
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer,(const char*)"+MIPOPEN:");//�ȴ�����
	}
	Clear_Buffer();
	
}



/*******************************************************

	ͨ��socket����TCP����
	��ӦATָ�AT+MIPOPEN=1,,\"47.92.117.163\",30000,0

********************************************************/

void Creat_TCP(void)
{
	Clear_Buffer();
//	uint8_t untildata=0xff;
	printf("AT+MIPOPEN=1,,\"47.92.117.163\",30000,0\r\n");//����TCP����
	//printf("AT+MIPOPEN=1,,\"183.47.42.41\",9001,0\r\n");//����TCP����
	HAL_Delay(3000);
	
	strx=strstr((const char*)RxBuffer,(const char*)"+MIPOPEN: 1,1");//�Ƿ�ɹ�����TCP
	while(strx==NULL)
	{
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer,(const char*)"+MIPOPEN: 1,1");//�ȴ��ɹ�����TCP
	}
	Clear_Buffer();
}


/**************************************************************

	�������ݣ���TCP���ӽ�������ò���Ч���������������������
	��Ҫ���뱣�����ݵ�����
	��ӦATָ�AT+MIPSEND

***************************************************************/

void Send_Data(char *bufferdata)
{
	Clear_Buffer();
	HAL_Delay(2000);
	printf("AT+MIPSEND=1,15\r\n");
	printf( "%s",bufferdata);
	HAL_Delay(2000);
	Clear_Buffer();
	
}


/*******************************************************

	�ر�TCP����
	���������շ����������쳣�����Թرյ��������� TCP ���� 
	��ӦATָ�AT+MIPCLOSE=1

********************************************************/



void End_TCP(void)
{
	Clear_Buffer();
	HAL_Delay(2000);
	printf("AT+MIPCLOSE=1\r\n");
	
	strx=strstr((const char*)RxBuffer,(const char*)"+MIPCLOSE: 1,0");
	while(strx==NULL)
	{
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer,(const char*)"+MIPCLOSE: 1,0");//�ȴ�TCP ���ӶϿ�
	}
	Clear_Buffer();
	
	
}





/*******************************************************

	�ͷ�IP
	�����ͷ�ģ�鱾�μ�����ȡ�� IP ��ַ  
	��ӦATָ�AT+MIPCALL=0

********************************************************/



void IP_release(void)
{
	Clear_Buffer();
	HAL_Delay(2000);
	printf("AT+MIPCALL=0\r\n");
	
	strx=strstr((const char*)RxBuffer,(const char*)"+MIPCALL: 0");
	while(strx==NULL)
	{
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer,(const char*)"+MIPCALL: 0");//�ȴ�TCP ���ӶϿ�
	}
	Clear_Buffer();
	
	
}























