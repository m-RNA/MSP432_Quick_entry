/******************************************************************************
//MSP432P401R
//10 OLED显示（模拟IIC）
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/9/12
*******************************************************************************/

#include "sysinit.h"
#include "delay.h"
#include "oled.h"
#include "bmp.h"

/*********  声明  **********
 *
 * 0.96寸OLED
 * UP将中景园的MSP430驱动
 * 移植到了MSP432上来了
 * 并稍微修改了一下测试代码
 *
 * 引脚接法:
 * P6.0 SCL
 * P6.1 SDA
 * 可以去头文件更改引脚
****************************/

int main(void)
{
    char t;

    SysInit();    // 第3讲  时钟配置
    delay_init(); // 第4讲  滴答延时
    OLED_Init();  // 第10讲 OLED显示

    while (1)
    {
        //显示图片
        OLED_DrawBMP(0, 0, 128, 64, BMP1);
        delay_ms(500);
        OLED_Clear(); //清屏

        //显示汉字
        OLED_ShowChinese(0, 0, 0, 16);   //中
        OLED_ShowChinese(18, 0, 1, 16);  //景
        OLED_ShowChinese(36, 0, 2, 16);  //园
        OLED_ShowChinese(54, 0, 3, 16);  //电
        OLED_ShowChinese(72, 0, 4, 16);  //子
        OLED_ShowChinese(90, 0, 5, 16);  //科
        OLED_ShowChinese(108, 0, 6, 16); //技

        //显示字符串
        OLED_ShowString(8, 2, (uint8_t *)"ZHONGJINGYUAN", 16);
        OLED_ShowString(20, 4, (uint8_t *)"2014/05/01", 16);
        OLED_ShowString(0, 6, (uint8_t *)"ASCII:", 16);
        OLED_ShowString(63, 6, (uint8_t *)"CODE:", 16);

        //显示字符，数字
		for (t = ' '; t < '~'; t++)
		{
			OLED_ShowChar(48, 6, t, 16);	//显示ASCII字符
			OLED_ShowNum(103, 6, t, 3, 16); //显示ASCII字符的码值
		}
		
        delay_ms(500);
        OLED_Clear(); //清屏
    }
}
