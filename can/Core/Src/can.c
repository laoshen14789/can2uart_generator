/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
#include "status.h"
#include <string.h>
#include "generator.h"
CAN_TxHeaderTypeDef	TxHeader;      //发送

uint8_t canRxData[64] = {0};
generator_info_s generator_info;

typedef union
{
    __IO uint32_t value;
    struct
    {
        uint8_t REV : 1;			///< [0]    :???
        uint8_t RTR : 1;			///< [1]    : RTR(??????????)
        uint8_t IDE : 1;			///< [2]    : IDE(??????????)
        uint32_t EXID : 18;			///< [21:3] : ?????ID
        uint16_t STID : 11;			///< [31:22]: ?????ID
    } Sub;
} CAN_FilterRegTypeDef;


typedef struct
{
	CAN_RxHeaderTypeDef hdr;
	uint8_t payload[8];
}CAN_RxPacketTypeDef;


CAN_RxPacketTypeDef packet;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 3;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_5TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  CAN_Config();
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration    
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN GPIO Configuration    
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void CAN_Config(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  CAN_FilterRegTypeDef IDH = {0};
  CAN_FilterRegTypeDef IDL = {0};
  /*配置CAN过滤器*/
  IDH.Sub.STID = (0 >> 16) & 0xFFFF;		// ??ID?16?
  IDL.Sub.STID = (0 & 0xFFFF);				// ??ID?16?
  sFilterConfig.FilterBank = 0;                     //过滤器0
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh         = IDH.value;										// ???????ID????,??????
  sFilterConfig.FilterIdLow          = IDL.value;										// ???????ID????,??????
  sFilterConfig.FilterMaskIdHigh     = IDH.value;										// ???????ID????,??????
  sFilterConfig.FilterMaskIdLow      = IDL.value;										// ???????ID????
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;//过滤器0关联到FIFO0
  sFilterConfig.FilterActivation = ENABLE;          //激活滤波器0
  sFilterConfig.SlaveStartFilterBank = 14;
	
  //过滤器配置
  if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
  {
    while(1){}
  }

  //启动CAN外围设备
  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
    while(1){}
  }

  //激活可以RX通知
  if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    while(1){}
  }
  
  /*配置传输过程*/
  TxHeader.StdId = 0x321;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;
}

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
uint8_t CAN1_Send_Msg(uint8_t* msg,uint8_t len)
{	
    uint8_t i=0;
	uint32_t TxMailbox;
	uint8_t message[8];
    TxHeader.StdId=0X12;        //标准标识符
    TxHeader.ExtId=0x12;        //扩展标识符(29位)
    TxHeader.IDE=CAN_ID_STD;    //使用标准帧
    TxHeader.RTR=CAN_RTR_DATA;  //数据帧
    TxHeader.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, message, &TxMailbox) != HAL_OK)//发送
	{
		return 1;
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 3) {}
    return 0;
}

//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
uint8_t CAN1_Receive_Msg(uint8_t *buf,CAN_HandleTypeDef * canHandle)
{
 	uint32_t i;

//	if(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) != 1)
//	{
//		return 0xF1;
//	}

	if(HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &packet.hdr, packet.payload) != HAL_OK)
	{
		return 0xF2;
	}
    for(i=0;i<packet.hdr.DLC;i++)
		buf[i]=packet.payload[i];
	return packet.hdr.DLC;
}

int count = 0;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
	if(CAN1_Receive_Msg(canRxData,CanHandle) !=0)
	{
		switch(packet.hdr.StdId)
		{
			case CAN_MESSAGE1:
				memcpy(&generator_info.generator0_s,canRxData,packet.hdr.DLC);
				break;
			
			case CAN_MESSAGE2:
				memcpy(&generator_info.generator1_s,canRxData,packet.hdr.DLC);
				count++;
				break;
			
			case CAN_MESSAGE3:
				memcpy(&generator_info.generator2_s,canRxData,packet.hdr.DLC);
				count++;
				break;
			
			case CAN_MESSAGE4:
				memcpy(&generator_info.generator3_s,canRxData,packet.hdr.DLC);
				count++;
				break;
			
			case CAN_MESSAGE5:
				memcpy(&generator_info.generator4_s,canRxData,packet.hdr.DLC);
				break;
			
			default:
				break;
		}
		HAL_CAN_ActivateNotification(CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING);
		set_status(CAN_RX_STATUS,1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//		update_generator_data();
	}
	
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
