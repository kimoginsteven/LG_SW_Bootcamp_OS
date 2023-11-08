#include "Device_Driver.h"

//#include ".\images\image0.h"
//#include ".\images\img101.h"
#include ".\images\birthday01.h"
#include ".\images\birthday02.h"
#include ".\images\birthday03.h"
#include ".\images\birthday04.h"
#include ".\images\birthday05.h"
#include ".\images\birthday06.h"
#include ".\images\birthday07.h"
#include ".\images\birthday08.h"
#include ".\images\birthday09.h"
#include ".\images\birthday10.h"
#include ".\images\birthday11.h"
#include ".\images\birthday12.h"
#include ".\images\birthday13.h"

#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07e0
#define RED		0xf800
#define YELLOW	0xffe0
#define VIOLET	0xf81f

#define DELAY	4000

//const unsigned short * img[]={image0, img101};
const unsigned short * img[]={birthday01, birthday02, birthday03, birthday04,
								birthday05, birthday06, birthday07, birthday08,
								birthday09, birthday10, birthday11, birthday12, birthday13};


void Main(void)
{
	Lcd_Clr_Screen();

	// IRQ 요청 진행
	Send_Key3_Request(0);
	Send_Key4_Request(0);

	int key3_response_1;
	int key4_response_1;
	int key3_response_2;
	int key4_response_2;
	int tmp = 0;

	// key3_response = Get_Key3_Response(0);
	// key4_response = Get_Key4_Response(0);

	Lcd_Clr_Screen();
	int x, y;
	int i;
	for(;;)
	{
		i = 0;
		x = 272;
		y = 165;
		for (; i < 13; i++)
		{
			if (i == 10)
			{
				x = 240;
				y = 119;
			}
			if (i == 11)
			{
				x = 513;
				y = 75;
			}
			if (i == 12){
				x = 272;
				y = 165;
			}
			Lcd_Draw_BMP(x,y,img[i]);
			Delay(DELAY);
			if (i >= 9)
			{
				Delay(DELAY);
			}
			key3_response_1 = Get_Key3_Response(0);
			key4_response_1 = Get_Key4_Response(0);
			if (key3_response_1)
			{
				for (;;)
				{
					key4_response_2 = Get_Key4_Response(0);
					if (key4_response_2)
						break;
				}

			}
			if (key4_response_1)
			{
				for (;;)
				{
					key3_response_2 = Get_Key3_Response(0);
					if (key3_response_2)
						break;
				}
			}


			Lcd_Clr_Screen();
		}
//		Lcd_Draw_BMP(240,119,img[10]);
//		Delay(DELAY*2);
//		Lcd_Clr_Screen();
//
//		Lcd_Draw_BMP(513,75,img[11]);
//		Delay(DELAY*2);
//		Lcd_Clr_Screen();
//
//		Lcd_Draw_BMP(272,165,img[12]);
//		Delay(DELAY*2);
//		Lcd_Clr_Screen();
//		Lcd_Draw_BMP(0,0,img[0]);
//		Delay(DELAY);
//		Lcd_Clr_Screen();
//
//		Lcd_Draw_BMP(0,0,img[1]);
//		Delay(DELAY);
//		Lcd_Clr_Screen();
	}

}
//void Lcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,   ...)
// : (x,   y)에 글씨색 color,    배경색 bkcolor,    가로 zx배,    새로 zy배로 확대한 문자열 출력
// 1024*600, 글자 크기 1, 1, 기준 가로는 문자 128개, 세로는 문자 40개 길이가 넘어갈경우 모듈러됨
