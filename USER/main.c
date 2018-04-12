#include "mycommon.h"
#include "gpio.h"
#include "oled.h"
#include "bmp.h"
int main(void)
{
    uint8_t t;
    OLED_Init(); //初始化OLED
    OLED_Clear();

    t = ' ';
    OLED_ShowCHinese(0, 0, 0);   //中
    OLED_ShowCHinese(18, 0, 1);  //景
    OLED_ShowCHinese(36, 0, 2);  //园
    OLED_ShowCHinese(54, 0, 3);  //电
    OLED_ShowCHinese(72, 0, 4);  //子
    OLED_ShowCHinese(90, 0, 5);  //科
    OLED_ShowCHinese(108, 0, 6); //技
    while (1)
    {
        OLED_Clear();
        OLED_ShowCHinese(0, 0, 0);   //中
        OLED_ShowCHinese(18, 0, 1);  //景
        OLED_ShowCHinese(36, 0, 2);  //园
        OLED_ShowCHinese(54, 0, 3);  //电
        OLED_ShowCHinese(72, 0, 4);  //子
        OLED_ShowCHinese(90, 0, 5);  //科
        OLED_ShowCHinese(108, 0, 6); //技
        OLED_ShowString(6, 3, "0.96' OLED TEST", 16);
        //OLED_ShowString(8,2,"ZHONGJINGYUAN");
        //	OLED_ShowString(20,4,"2014/05/01");
        OLED_ShowString(0, 6, "ASCII:", 16);
        OLED_ShowString(63, 6, "CODE:", 16);
        OLED_ShowChar(48, 6, t, 16); //显示ASCII字符
        t++;
        if (t > '~')
            t = ' ';
        OLED_ShowNum(103, 6, t, 3, 16); //显示ASCII字符的码值
        sleep(2);
        OLED_DrawBMP(0, 0, 128, 8, BMP1); //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
        sleep(2);
        OLED_DrawBMP(0, 0, 128, 8, BMP1);
        sleep(2);
    }
    OLED_Close();
    exit(EXIT_SUCCESS);
}
