#ifndef OLED_COLOR_H
#define OLED_COLOR_H
//������ɫ���򵥻���or���ӻ���
#define  COLOR_CHOOSE_DRAW   (SIMPLE_DRAW)   


typedef enum
{
	pix_black,
	pix_white,
	//............
}Type_color;

//��ȡ��ǰ���õ����ص�ɫ
Type_color GetDrawColor(void);
//���ý�Ҫ���Ƶ����ص�ɫ
void SetDrawColor(Type_color value);

Type_color GetFillColor(void);
void SetFillcolor(Type_color value);
#endif


