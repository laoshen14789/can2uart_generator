/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
CAN_TxHeaderTypeDef	TxHeader;      //鍙戦??

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

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 5;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_5TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
	CAN_Config();
  /* USER CODE END CAN_Init 2 */

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
  /*閰嶇疆CAN杩囨护鍣?*/
  IDH.Sub.STID = (0 >> 16) & 0xFFFF;		// ??ID?16?
  IDL.Sub.STID = (0 & 0xFFFF);				// ??ID?16?
  sFilterConfig.FilterBank = 0;                     //杩囨护鍣?0
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh         = IDH.value;										// ???????ID????,??????
  sFilterConfig.FilterIdLow          = IDL.value;										// ???????ID????,??????
  sFilterConfig.FilterMaskIdHigh     = IDH.value;										// ???????ID????,??????
  sFilterConfig.FilterMaskIdLow      = IDL.value;										// ???????ID????
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;//杩囨护鍣?0鍏宠仈鍒癋IFO0
  sFilterConfig.FilterActivation = ENABLE;          //�??娲绘护娉㈠櫒0
  sFilterConfig.SlaveStartFilterBank = 14;
	
  //杩囨护鍣ㄩ厤�??
  if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
  {
    while(1){}
  }

  //鍚姩CAN澶栧洿璁惧
  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
    while(1){}
  }

  //�??娲�?彲浠X�?���?
  if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    while(1){}
  }
  
  /*閰嶇疆浼犺緭杩囩�?*/
  TxHeader.StdId = 0x321;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;
}

//can鍙戦?佷�?缁勬暟鎹?(鍥哄畾鏍煎紡:ID�??0X12,鏍囧�?��?,鏁版嵁甯?)	
//len:鏁版嵁闀�??(�??澶т负8)				     
//msg:鏁版嵁鎸�?��,�??澶т负8涓瓧鑺?.
//杩斿洖鍊?:0,鎴愬�?;
//		 鍏朵�?,澶辫�?;
uint8_t CAN1_Send_Msg(uint8_t* msg,uint8_t len)
{	
    uint8_t i=0;
	uint32_t TxMailbox;
	uint8_t message[8];
    TxHeader.StdId=0X12;        //鏍囧�?���?���??
    TxHeader.ExtId=0x12;        //鎵╁睍鏍�?���??(29�??)
    TxHeader.IDE=CAN_ID_STD;    //浣跨敤鏍囧噯�??
    TxHeader.RTR=CAN_RTR_DATA;  //鏁版嵁甯?
    TxHeader.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, message, &TxMailbox) != HAL_OK)//鍙戦??
	{
		return 1;
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 3) {}
    return 0;
}

//can鍙ｆ帴鏀舵暟鎹煡璇?
//buf:鏁版嵁缂撳瓨�??;	 
//杩斿洖鍊?:0,鏃犳暟鎹�?跺埌;
//		 鍏朵�?,鎺ユ敹鐨�?��鎹暱搴?;
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
				break;
			
			case CAN_MESSAGE3:
				memcpy(&generator_info.generator2_s,canRxData,packet.hdr.DLC);
				break;
			
			case CAN_MESSAGE4:
				memcpy(&generator_info.generator3_s,canRxData,packet.hdr.DLC);
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
		update_generator_data();
	}
	
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
