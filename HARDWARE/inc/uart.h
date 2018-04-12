#ifndef _UART_H_
#define _UART_H_
#include "mycommon.h"

#define FALSE  -1
#define TRUE   0
#define UART_PORT0  "/dev/ttyS0"
#define UART_PORT1  "/dev/ttyS1"
#define UART_PORT2  "/dev/ttyS2"

extern int UART_Open(int fd,char* port);
extern void UART_Close(int fd);
extern int UART_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity);
extern int UART_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity);
extern int UART_Recv(int fd, char *rcv_buf,int data_len);
extern int UART_Send(int fd, char *send_buf,int data_len);


#endif // _UART_H_
