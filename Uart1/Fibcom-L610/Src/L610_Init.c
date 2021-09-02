/*******************************************************************************
**                       @ Copyright 2014 - 2020                              **
**                           www.diysoon.com                                  **
**                                                                            **
*******************************************************************************/

#include "L610_Init.h"
#include "L610_TCP.h"



char *strx,*extstrx,*Readystrx;



/*****************************************************************

		模块开机函数
		模块上电后默认是关机状态，需要控制IO口输出电平控制模块开机

*******************************************************************/


void OPEN_MOUDLE(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);	

}



/*****************************************************************

		等待模块开机指示

*******************************************************************/
void WAIT_OPEN(void)
{
	while (pow_flg != 1)
	{
		UART2_Printf("WAIT_OPEN while 1\r\n");
		HAL_Delay(1);
	}
	HAL_Delay(1);
}






/********************************************************

	清空Buffer中的数据

*********************************************************/

void Clear_Buffer(void)//清空缓存
{
    uint8_t i;
  //  HAL_UART_Transmit(&huart1,(uint8_t *)RxBuffer,20,20);
    for(i=0;i<Rxcouter;i++)
    RxBuffer[i]=0;//缓存
	//memset(RxBuffer,0,sizeof(RxBuffer));
    Rxcouter=0;
}




/********************************************************

	模块初始化函数，通过AT指令对模块各个部分进行初始化，
	初始化结束后模块可以通过TCP协议向服务器发送数据。
	最后是请求运营商分配IP

*********************************************************/
void Fibcom_L610_init(void)
{
	UART2_Printf("Fibcom_L610_init 000\r\n");
	Clear_Buffer();
	UART2_Printf("Fibcom_L610_init 11\r\n");
	AT_check();
	UART2_Printf("Fibcom_L610_init 22\r\n");
	HAL_Delay(200);
	CSQ_check();
	UART2_Printf("Fibcom_L610_init 33\r\n");
	HAL_Delay(200);
	CSQ_check();
	HAL_Delay(200);
	CSQ_check();	
	HAL_Delay(200);
	ATI_check();
	HAL_Delay(200);
	CPIN_check();
	HAL_Delay(200);
	GPRS_check();
	HAL_Delay(200);
	EPS_check();
	HAL_Delay(200);
	Hex_set();
	HAL_Delay(200);
	IP_allo();
	HAL_Delay(200);
	CSQ_check();
	HAL_Delay(200);
	
	
	
}

/*******************************************************

	检测模块AT指令状态，回复OK可以正常发送AT指令，结束后
	清除buffer。
	对应AT指令：AT

*******************************************************/

void AT_check(void)
{
	printf("ATE\r\n"); 
	HAL_Delay(500);
	UART2_Printf(" AT_check RxBuffer [%s]",RxBuffer);
	strx=strstr((const char*)(RxBuffer),(const char*)"OK");//返回OK
	/*等待返回OK*/
	while(strx==NULL)
	{
		Clear_Buffer();	
		printf("AT\r\n"); 
		HAL_Delay(500);
		strx=strstr((const char*)(RxBuffer+4),(const char*)"OK");//返回OK
	}
	Clear_Buffer();
}
/***************************************************

	检查模块信号值，如果信号值太差可能会导致联网失败
	对应AT指令：AT+CSQ?

***************************************************/

void CSQ_check(void)
{
	int csq=0;
	int csq_2=0;
	
	printf("AT+CSQ\r\n"); //检查CSQ
	HAL_Delay(500);
	UART2_Printf(" CSQ_check RxBuffer [%s]",RxBuffer);
	if(strstr(RxBuffer,"+CSQ: "))
	{
		sscanf((const char *)RxBuffer,"\r\n+CSQ: %d,%d",&csq,&csq_2);
		UART2_Printf(" CSQ_check csq %d csq2 %d",csq,csq_2);
	}
	
	
	Clear_Buffer();	
	HAL_Delay(1000);
}

/***************************************************

	查询模块固件版本号，便于问题分析，本步骤不强制
	对应AT指令：ATI

****************************************************/

void ATI_check(void)
{
	printf("ATI\r\n"); //检查模块的版本号
	HAL_Delay(500);
}
/****************************************************

	检查SIM卡状态，如果SIM卡状态不正常，模块就无法正常建立无线通信
	对应AT指令 ：AT+CPIN?

*****************************************************/

void CPIN_check(void)
{
	printf("AT+CPIN?\r\n");//检查SIM卡是否在位,卡的缺口朝外放置 
	HAL_Delay(500);
	strx=strstr((const char*)RxBuffer,(const char*)"+CPIN: READY");//查看是否返回ready
	/*等待返回OK*/
	while(strx==NULL)
	{
		Clear_Buffer();
		printf("AT+CPIN?\r\n");
		HAL_Delay(500);
		strx=strstr((const char*)RxBuffer,(const char*)"+CPIN: READY");//检查SIM卡是否在位，等待卡在位，
	}
	Clear_Buffer();	
}


/******************************************************

  查询GPRS服务是否可用,检查模块网络是否正常
	对应AT指令：AT+CGREG?	

*******************************************************/

void GPRS_check(void)
{
	printf("AT+CGREG?\r\n");
	HAL_Delay(500);
	strx=strstr((const char*)RxBuffer,(const char*)"+CGREG: 0,1");//返回正常
	extstrx=strstr((const char*)RxBuffer,(const char*)"+CGREG: 0,5");//返回正常，漫游
	while(strx==NULL&&extstrx==NULL)
	{
		Clear_Buffer();
		printf("AT+CGREG?\r\n");//查看是否注册网络
		HAL_Delay(500);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGREG: 0,1");//返回正常
		extstrx=strstr((const char*)RxBuffer,(const char*)"+CGREG: 0,5");//返回正常，漫游
	}
	Clear_Buffer();
}

/******************************************************

  查询EPS服务是否可用,检查模块网络是否正常
	对应AT指令：AT+CEREG?	

*******************************************************/

void EPS_check(void)
{
	printf("AT+CEREG?\r\n");
	HAL_Delay(500);
	strx=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,1");//返回正常
	extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,5");//返回正常，漫游
	while(strx==NULL&&extstrx==NULL)
	{
		Clear_Buffer();
		printf("AT+CGREG?\r\n");//查看是否注册GSM网络
		HAL_Delay(500);
		strx=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,1");//返回正常
		extstrx=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,5");//返回正常，漫游
	}
	Clear_Buffer();	
	
	
}


/******************************************************************

	设置当模块收到服务器的数据时，上报数据的的格式。默认 0，掉电保存。
	对应AT指令：AT+GTSET="IPRFMT",0
	
******************************************************************/

void Hex_set(void)
{
	
	printf("AT+GTSET=\"IPRFMT\",0\r\n");
	HAL_Delay(500);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	/*等待返回OK*/
	while(strx==NULL)
	{
		Clear_Buffer();
		printf("AT+GTSET=\"IPRFMT\",0\r\n");
		HAL_Delay(500);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	}
	Clear_Buffer();	
	
}





