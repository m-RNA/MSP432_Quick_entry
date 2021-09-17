#include "oled_debug.h"
#include "oled_font.h"
#include "oled_config.h"
#include "oled_draw.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"


static int _cursor_x=0;
static int _cursor_y=0;
static unsigned char wrap=1;
void SetTextWrap(unsigned char w)
{
  wrap = w;
}

void SetCursor(int x, int y)
{
  _cursor_x = x;
  _cursor_y = y;
}


static unsigned long m_pow(int x,int y)
{
      unsigned long sum = 1;
      while(y--)
      {
           sum *= x;
      }
      return sum;
}

//��ӡ�ַ�
void m_putchar(const char c)
{
	unsigned char tmp=GetFontSize();
  if (c == '\n')
  {
    _cursor_y += 8;
    _cursor_x = 0;
  }
  else if (c == '\r')
  {
    // skip em
  }
  else
  {
		SetFontSize(1);
    DrawChar(_cursor_x, _cursor_y, c);
		SetFontSize(tmp);
    _cursor_x += 6;
    if (wrap && (_cursor_x > (SCREEN_COLUMN - 6)))
    {
      m_putchar('\n');
    }
  }
        
}
 
//��ӡ�ַ���
void m_putstr(const char *str)
{
      while(*str)
      {
            m_putchar(*str++);
      }
}



int OledPrintf(const char *str,...)
{
     va_list ap;              //����һ���ɱ� �����ģ��ַ�ָ�룩 
     int val,r_val;
     char count,ch;
     char *s = NULL;
     int res = 0;             //����ֵ
 
     va_start(ap,str);        //��ʼ��ap
     while('\0' != *str)//strΪ�ַ���,�������һ���ַ��϶���'\0'���ַ����Ľ�������
     { 
          switch(*str)
          {
              case '%':	     //���Ͳ���
              str++;
              switch(*str)
              {
                   case 'd': //10�������
                        val = va_arg(ap, int); 
 			            r_val = val; 
                        count = 0; 
                        while(r_val)
                        { 
                             count++;         //�����ĳ���
                             r_val /= 10;
                        }
                        res += count;         //����ֵ��������? 
                        r_val = val; 
                        while(count)
                        { 
                              ch = r_val / m_pow(10,count - 1);
                              r_val %= m_pow(10,count - 1);
                              m_putchar(ch + '0');     //���ֵ��ַ���ת�� 
                              count--;
                        }
                        break;
                  case 'x': //16������� 
                        val = va_arg(ap, int); 
                        r_val = val; 
                        count = 0;
                        while(r_val) 
                        { 
                             count++;     //�����ĳ��� 
                             r_val /= 16; 
                        } 
                        res += count;     //����ֵ��������? 
                        r_val = val; 
                        while(count) 
                        { 
                              ch = r_val / m_pow(16, count - 1); 
                              r_val %= m_pow(16, count - 1);
                              if(ch <= 9)
                                  m_putchar(ch + '0');	//���ֵ��ַ���ת�� 
                              else 
                                  m_putchar(ch - 10 + 'a'); 
                              count--;
                        }
                 break;
									case's':         //�����ַ��� 
                      s = va_arg(ap, char *); 	
                      m_putstr(s);          //�ַ���,����ֵΪ�ַ�ָ�� 
                      res += strlen(s);   //����ֵ�������� ? 
                 break;
									case 'c':
                      m_putchar( (char)va_arg(ap, int )); //��Ҳ�Ϊʲô��дchar����Ҫдint 
                      res += 1;
                 break;
                default :;
             }
             break;
          case '\n':
               m_putchar('\n'); 
               res += 1;
               break;
          case '\r':
               m_putchar('\r'); 
               res += 1;
               break;
					default :          //��ʾԭ���ĵ�һ���������ַ���(����..��Ĳ���o)
               m_putchar(*str);
               res += 1;
          }
         str++;
     }
		 if(_cursor_y+8>=63)
			 _cursor_y=0;
     va_end(ap);
		 UpdateScreen();
     return res;
}

