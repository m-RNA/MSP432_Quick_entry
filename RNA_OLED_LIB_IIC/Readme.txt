oled库说明：
	该库只是一个绘图图形库，适用于MSP432系列的12864oled屏，驱动芯片为ssd1306，若需被其他平台使用，请自行移植
	移植来自：https://github.com/hello-myj/stm32_oled
连接引脚：
	MSP432的软件件I2C：
		IIC: P1.7 -- SCL; P1.6 -- SDA
	MSP432的硬件I2C：
		IIC_1: PB6 -- SCL; PB7 -- SDA
		IIC_2: PB10-- SCL; PB11 --SDA（还未验证）
	MSP432的硬件SPI：
		暂未添加，若需请在oled_driver.h自行配置
	暂未添加软件模拟，后续会添加
	
图形库文件说明：
	draw_api.h:所需的各类api绘图方法
	oled_config.h:配置o文件，用于配置led屏
	oled_config.c:配置文件所需的接口
	oled_driver.c：oled屏驱动
	oled_draw.c:各类绘图方法
	oled_basic.c:底层和算法支撑
	oled_color.c:颜色控制
	oled_buffer.c:屏幕缓冲区和临时缓冲区
	oled_font.c:字体
	oled_bmp.c:bmp取模图形存放位置，
	oled_debug.c可调用OledPrintf方法，效果与printf一致。

其他：
	持续更新