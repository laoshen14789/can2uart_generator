#include <stdio.h>
#include "status.h"
#include "FreeRTOS.h"
#include "task.h"
int canTxFlag= -1;
int canRxFlag = -1;
int uart1TxFlag = -1;
int uart1RxFlag = -1;
int uart2TxFlag = -1;
int uart2RxFlag = -1;


void set_status(int item, int status)
{	
//	vTaskSuspendAll();
	switch(item)
	{
		case CAN_TX_STATUS:
			canTxFlag =status;
			break;

		case CAN_RX_STATUS:
			canRxFlag =status;
			break;

		case UART1_TX_STATUS:
			uart1TxFlag =status;
			break;

		case UART1_RX_STATUS:
			uart1RxFlag =status;
			break;

		case UART2_TX_STATUS:
			uart2TxFlag =status;
			break;		

		case UART2_RX_STATUS:
			uart2RxFlag =status;
			break;	
		
		default:
			break;
	}
//	xTaskResumeAll();
}

int get_status(int item)
{

	switch(item)
	{
		case CAN_TX_STATUS:
			return canTxFlag ;

		case CAN_RX_STATUS:
			return canRxFlag;

		case UART1_TX_STATUS:
			return uart1TxFlag;

		case UART1_RX_STATUS:
			return uart1RxFlag;

		case UART2_TX_STATUS:
			return uart2TxFlag;	

		case UART2_RX_STATUS:
			return uart2RxFlag;
		
		default:
			return -1;
	}
	
}
