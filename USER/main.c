#include "mycommon.h"
#include "gpio.h"
#include "oled.h"
#include "bmp.h"
#include "myi2c.h"
#include "myi2c-dev.h"
int main(void)
{
    // OLED_Init();
    // OLED_Clear();
    // OLED_DrawBMP(0, 0, 128, 8, BMP1);
    // OLED_Close();
    uint8_t val[2];
    val[0] = 0x00;
    val[1] = 0xAE;
    int Oledfd = -1;
    Oledfd = open(IIC_DEV, O_RDWR);
    if (Oledfd < 0)
        errExit("OPEN");
    if (ioctl(Oledfd, I2C_SLAVE, OLED_ADD) < 0)
        errExit("SLAVE");
    if (write(Oledfd, &val, 2) < 0)
    {
        printf("I2C write error\n");
    }
    exit(EXIT_SUCCESS);
}
