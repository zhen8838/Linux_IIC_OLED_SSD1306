#ifndef __GPIO_H__
#define __GPIO_H__
#include "mycommon.h"

#define PIO_BASE_ADDRESS 0x01C20800
extern unsigned int *gpio_map;
typedef struct
{
	unsigned int CFG[4];
	unsigned int DAT;
	unsigned int DRV0;
	unsigned int DRV1;
	unsigned int PUL0;
	unsigned int PUL1;
} PIO_Struct;

typedef struct
{
	PIO_Struct Pn[7];
} PIO_Map;

typedef enum {
	PA = 0,
	PB = 1,
	PC = 2,
	PD = 3,
	PE = 4,
	PF = 5,
	PG = 6,
} PORT;

typedef enum {
	PIN_0 = 0,
	PIN_1 = 1,
	PIN_2 = 2,
	PIN_3 = 3,
	PIN_4 = 4,
	PIN_5 = 5,
	PIN_6 = 6,
	PIN_7 = 7,
	PIN_8 = 8,
	PIN_9 = 9,
	PIN_10 = 10,
	PIN_11 = 11,
	PIN_12 = 12,
	PIN_13 = 13,
	PIN_14 = 14,
	PIN_15 = 15,
	PIN_16 = 16,
} PIN;

typedef enum {
	IN = 0x00,
	OUT = 0x01,
	AUX = 0x02,
	INT = 0x06,
	DISABLE = 0x07,
} PIN_MODE;

typedef enum{
	HIGH=1,
	LOW=0,
} PIN_STATE;

extern PIO_Map *PIO;

extern void GPIO_Init(void);
extern void GPIO_ConfigPin(PORT port, PIN pin, PIN_MODE mode);
extern void GPIO_SetPin(PORT port, PIN pin, PIN_STATE state);
extern unsigned int GPIO_GetPin(PORT port, unsigned int pin);
extern void GPIO_Free(void);
#endif
