# MSP432_oled

oled��˵����
	�ÿ�ֻ��һ����ͼͼ�ο⣬������MSP432ϵ�е�12864oled��������оƬΪssd1306�����豻����ƽ̨ʹ�ã���������ֲ  
	��ֲ���ԣ�https://github.com/hello-myj/stm32_oled
�������ţ�
	MSP432�������I2C��
		IIC: P1.7 -- SCL; P1.6 -- SDA
	MSP432��Ӳ��I2C��
		IIC_1: PB6 -- SCL; PB7 -- SDA
		IIC_2: PB10-- SCL; PB11 --SDA����δ��֤��
	MSP432��Ӳ��SPI��
		��δ��ӣ���������oled_driver.h��������
	��δ������ģ�⣬���������
	
ͼ�ο��ļ�˵����
	draw_api.h:����ĸ���api��ͼ����
	oled_config.h:����o�ļ�����������led��
	oled_config.c:�����ļ�����Ľӿ�
	oled_driver.c��oled������
	oled_draw.c:�����ͼ����
	oled_basic.c:�ײ���㷨֧��
	oled_color.c:��ɫ����
	oled_buffer.c:��Ļ����������ʱ������
	oled_font.c:����
	oled_bmp.c:bmpȡģͼ�δ��λ�ã�
	oled_debug.c�ɵ���OledPrintf������Ч����printfһ�¡�

������
	��������