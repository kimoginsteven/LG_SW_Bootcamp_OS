#include "Device_Driver.h"

#include ".\images\image0.h"
#include ".\images\img101.h"

#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07e0
#define RED		0xf800
#define YELLOW	0xffe0
#define VIOLET	0xf81f

#define DELAY	8000

const unsigned short * img[]={image0, img101};

void Main(void)
{
	Lcd_Clr_Screen();

	for(;;)
	{
		Lcd_Draw_BMP(0,0,img[0]);
		Delay(DELAY);
		Lcd_Clr_Screen();

		Lcd_Draw_BMP(0,0,img[1]);
		Delay(DELAY);
		Lcd_Clr_Screen();
	}
}
