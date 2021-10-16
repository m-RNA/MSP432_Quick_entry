#include "test.h"
#include "draw_api.h"
#include "delay.h"
#include "oled_draw_ex_example.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
void ShowStars(void);
void ShowWatch(void);
void ShowTest(void);
void ShowSnow(void);

void demo(void)
{
	int i, j;
	//demo????
	ClearScreen();
	DrawBitmap(0, 0, TempLogo, 128, 64);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	delay_ms(300);
	delay_ms(300);

	//��?????????????????
	ClearScreen();
	SetFontSize(1);
	DrawString(0, 0, "test");
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);

	SetFontSize(2);
	DrawString(0, 8, "test");
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);

	SetFontSize(3);
	DrawString(0, 24, "test");
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);

	//????
	ClearScreen();
	for (i = 0; i < 20; i++)
	{
		DrawLine(0, 0, i * 10, 63);
		UpdateScreen();
		delay_ms(100);
	}
	for (i = 0; i < 19; i++)
	{
		DrawLine(127, 0, 128 - i * 10, 63);
		UpdateScreen();
		delay_ms(100);
	}
	delay_ms(300);
	delay_ms(300);

	//????

	for (j = 0; j < 2; j++)
	{
		if (j == 0)
			ClearScreen();
		for (i = 0; i < 31; i += 2)
		{
			DrawRect2(i * 2, i, 128 - i * 4, 64 - 2 * i);
			UpdateScreen();
			delay_ms(100);
		}
		if (j == 0)
			ClearScreen();
		delay_ms(500);
		for (i = 31; i > 0; i -= 2)
		{
			DrawFillRect2(i * 2, i, 128 - i * 4, 64 - 2 * i);
			UpdateScreen();
			delay_ms(100);
		}
		SetDrawColor(pix_black);
		SetFillcolor(pix_black);
	}
	SetDrawColor(pix_white);
	SetFillcolor(pix_white);

	//????????
	for (j = 0; j < 2; j++)
	{
		if (j == 0)
			ClearScreen();
		for (i = 0; i < 31; i += 2)
		{
			DrawRoundRect(i * 2, i, 128 - i * 4, 64 - 2 * i, 2);
			UpdateScreen();
			delay_ms(100);
		}
		if (j == 0)
			ClearScreen();
		delay_ms(500);
		for (i = 31; i > 2; i -= 2)
		{
			DrawfillRoundRect(i * 2, i, 128 - i * 4, 64 - 2 * i, 2);
			UpdateScreen();
			delay_ms(100);
		}
		SetDrawColor(pix_black);
		SetFillcolor(pix_black);
	}
	SetDrawColor(pix_white);
	SetFillcolor(pix_white);

	//????
	ClearScreen();
	DrawEllipse(63, 31, 63, 31);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	ClearScreen();
	DrawEllipse(63, 31, 16, 31);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	ClearScreen();
	DrawFillEllipse(63, 31, 63, 31);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	ClearScreen();
	DrawFillEllipse(63, 31, 16, 31);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	//??
	ClearScreen();
	DrawCircle(63, 31, 30);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	ClearScreen();
	DrawFillCircle(63, 31, 30);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);

	//??????
	ClearScreen();
	DrawTriangle(5, 10, 100, 30, 60, 50);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	ClearScreen();
	DrawFillTriangle(5, 10, 100, 30, 60, 50);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	//????????
	ClearScreen();
	DrawBitmap(0, 0, BmpTest1, 16, 16);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	DrawBitmap(16, 0, BmpTest2, 32, 32);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	DrawBitmap(48, 0, BmpTest3, 64, 64);
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);

	//????
	ClearScreen();
	for (i = 0; i < 369; i++)
	{
		DrawArc(63, 31, 30, 0, i);
		UpdateScreen();
		ClearScreen();
	}
	DrawCircle(63, 31, 30);
	UpdateScreen();
	delay_ms(500);
	//************
	for (i = 0; i < 10; i++)
	{
		DrawFillCircle(63, 31, i);
		DrawCircle(63, 31, 30);
		UpdateScreen();
		ClearScreen();
		delay_ms(100);
	}

	//????
	for (i = 0; i < 720; i++)
	{
		TypeXY temp;
		SetAngle(i);
		SetRotateCenter(63, 31);
		temp = GetRotateXY(63, 31 + 30);
		DrawFillCircle(temp.x, temp.y, 5);
		DrawCircle(63, 31, 30);
		DrawFillCircle(63, 31, 10);
		UpdateScreen();
		ClearScreen();
	}
	//*************
	for (i = 0; i < 95; i++)
	{
		TypeXY temp;
		SetAngle(720 + i);
		SetRotateCenter(63 + i, 31);
		temp = GetRotateXY(63 + i, 31 + 30);
		DrawFillCircle(temp.x, temp.y, 5);
		DrawCircle(63 + i, 31, 30);
		DrawFillCircle(63 + i, 31, 10);
		UpdateScreen();
		ClearScreen();
	}

	//????
	ShowStars();
	ShowWatch();
	ShowTest();
	delay_ms(300);
	delay_ms(300);
	//?��??
	ClearScreen();
	SetFontSize(2);
	DrawString(8, 16, " Show End ");
	UpdateScreen();
	delay_ms(300);
	delay_ms(300);
	delay_ms(300);
	delay_ms(300);
	ClearScreen();
	DrawBitmap(0, 10, DZTBGZ, 128, 40);
	UpdateScreen();
	while (1)
		;
}

//????????
void ShowStars(void)
{
	int i; //j;
	int count = 0;
	int fps = 60;
	typedef struct START
	{
		short x;
		short y;
		short speed;
		unsigned char speedcount;
		unsigned char isexist;
	} Star;
	Star star[128] = {0};
	srand(2);
	for (i = 0; i < 128; i++)
	{
		if (star[i].isexist == 0)
		{
			star[i].x = rand() % 127;
			star[i].y = rand() % 64;
			star[i].speedcount = 0;
			star[i].speed = rand() % 8 + 1;
			star[i].isexist = 1;
		}
	}
	while (1)
	{
		if (FrameRateUpdateScreen(fps) == 1)
		{
			count++;
			if (count >= fps * 10) //10????
				return;
		}

		for (i = 0; i < 128; i++)
		{
			if (star[i].isexist == 0)
			{

				star[i].x = 0;
				star[i].y = rand() % 64;
				star[i].speed = rand() % 6 + 1;
				star[i].speedcount = 0;
				star[i].isexist = 1;
			}
			else
			{
				star[i].speedcount++;
				if (star[i].x >= 124)
				{
					star[i].isexist = 0;
				}
				SetDrawColor(pix_black);
				DrawLine(star[i].x, star[i].y, star[i].x, star[i].y);
				SetDrawColor(pix_white);
				if (star[i].speedcount == star[i].speed)
				{
					star[i].speedcount = 0;
					star[i].x += 1;
				}
				DrawLine(star[i].x, star[i].y, star[i].x + (6 / star[i].speed) - 1, star[i].y);
			}
		}
	}
}

void ShowWatch(void)
{
	int i, j, z;
	int count = 0;
	for (i = 0; i < 12; i++)
		for (j = 0; j < 60; j++)
			for (z = 0; z < 60; z++)
			{
				RoundClock(i, j, z);
				count++;
				if (count >= 800)
				{
					return;
				}
			}
}

void ShowWave(void)
{
	while (1)
	{
	}
}

void ShowTest(void)
{
	int x0 = 63, y0 = 31;
	unsigned char i = 0, j;
	int n = 1, r = 31, v = 1, count = 0;
	int x[30], y[30];

	while (1)
	{
		ClearScreen();
		for (i = 0; i < n; i++)
		{
			x[i] = r * cos(2 * 3.1415926 * i / n) + x0;
			y[i] = r * sin(2 * 3.1415926 * i / n) + y0;
		}
		for (i = 0; i <= n - 2; i++)
		{
			for (j = i + 1; j <= n - 1; j++)
				DrawLine(x[i], y[i], x[j], y[j]);
		}
		n += v;
		if (n == 20 || n == 0)
			v = -v;
		delay_ms(100);
		UpdateScreen();
		if (++count == 90)
		{
			count = 0;
			return;
		}
	}
}

void ShowSnow(void)
{
	int a[66], i, num = 0;
	struct Snow
	{
		short x;
		short y;
		short speed;
	} snow[100];

	srand(1);
	for (i = 0; i < 66; i++)
		a[i] = (i - 2) * 10;
	ClearScreen();
	while (1)
	{
		//FrameRateUpdateScreen(60);
		if (num != 100)
		{

			snow[num].speed = 1 + rand() % 4;
			i = rand() % 66;
			snow[num].x = a[i];
			snow[num].y = 0;
			num++;
		}
		for (i = 0; i < num; i++)
		{
			snow[i].y += snow[i].speed;
			DrawPixel(snow[i].x, snow[i].y + 1);
			DrawPixel(snow[i].x + 1, snow[i].y);
			DrawPixel(snow[i].x, snow[i].y);
			DrawPixel(snow[i].x - 1, snow[i].y);
			DrawPixel(snow[i].x, snow[i].y - 1);
			if (snow[i].y > 63)
			{
				snow[i].y = 0;
			}
		}
		delay_ms(30);
		UpdateScreen();
		ClearScreen();
	}
}
