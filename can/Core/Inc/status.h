#ifndef __STATUS_H
#define __STATUS_H

#include "main.h"

typedef enum{
	CAN_TX_STATUS =1,
	CAN_RX_STATUS =2,
	UART1_TX_STATUS =3,
	UART1_RX_STATUS =4,
	UART2_TX_STATUS =5,
	UART2_RX_STATUS =6
}sys_e;

void set_status(int item, int status);
int get_status(int item);
#endif
