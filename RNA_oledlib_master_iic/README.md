# stm32_oled


oled��˵����
	�ÿ�ֻ��һ����ͼͼ�ο⣬ֻ��������stm32ϵ�е�12864oled��������оƬΪssd1306�����豻����ƽ̨ʹ�ã���������ֲ

�������ţ�
	STM32��Ӳ��I2C��
		IIC_1: PB6 -- SCL; PB7 -- SDA
		IIC_2: PB10-- SCL; PB11 --SDA����δ��֤��
	STM32��Ӳ��SPI��
		SPI_1:��δ��ӣ���������oled_driver.h��������
		SPI_2:PB10--RES; PB11--DC; PB12--CS;PB13--D0;PB15--D1;
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
	ʹ�õ�ȡģ����Ϊ��PCtoLCD2002���ѷ���tool�ļ����У�ʹ�÷�����ο�tool/ȡģ����
	ע�⣺Ӳ��IIC���ֿ���������ص����ڳ�ʼ���������ϵ�
	�ÿ⻹�����ƣ����ڳ������£�