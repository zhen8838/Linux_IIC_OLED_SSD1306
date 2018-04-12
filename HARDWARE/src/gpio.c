#include "gpio.h"

PIO_Map *PIO = NULL;
unsigned int *gpio_map;

void GPIO_Init(void)
{
	unsigned int fd;
	unsigned int addr_start, addr_offset;
	unsigned int PageSize, PageMask;

	if ((fd = open("/dev/mem", O_RDWR)) == -1)
	{
		printf("open error\r\n");
		return;
	}

	PageSize = sysconf(_SC_PAGESIZE); //页大小
	PageMask = (~(PageSize - 1));	 //页掩码
	printf("PageSize:%d,PageMask:%.8X\r\n", PageSize, PageMask);

	addr_start = PIO_BASE_ADDRESS & PageMask;
	addr_offset = PIO_BASE_ADDRESS & ~PageMask;
	printf("addr_start:%.8X,addr_offset:%.8X\r\n", addr_start, addr_offset);

    if ((gpio_map = mmap(NULL, PageSize * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr_start)) == NULL)
	{
		printf("mmap error\r\n");
		close(fd);
		return;
	}
	printf("gpio_map:%.8X\r\n", gpio_map);

	PIO = (PIO_Map *)((unsigned int)gpio_map + addr_offset);
	 printf("PIO:%.8X\r\n", PIO);

	close(fd);

	
}

void GPIO_ConfigPin(PORT port, PIN pin, PIN_MODE mode)
{
	if (gpio_map == NULL)
		return;
	PIO->Pn[port].CFG[pin / 8] &= ~((unsigned int)0x07 << pin % 8 * 4);
	PIO->Pn[port].CFG[pin / 8] |= ((unsigned int)mode << pin % 8 * 4);
}

void GPIO_SetPin(PORT port, PIN pin, PIN_STATE state)
{
	if (gpio_map == NULL)
		return;
	if (state)
		PIO->Pn[port].DAT |= (1 << pin);
	else
		PIO->Pn[port].DAT &= ~(1 << pin);
}
