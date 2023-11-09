#include "Device_Driver.h"

#define DELAY	2000


#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07e0
#define RED		0xf800
#define YELLOW	0xffe0
#define VIOLET	0xf81f

int i;
int uart_response;
int key3_response;
int key4_response;
char name[100];
int name_idx = 0;
int tmp;

void Main(void)
{
	//For Test & Demo
//	Print_Hello();
//	Uart_Printf("test Sqr func : %d\n", Sqr(122));
//	Uart_Printf("test longlong func : %lld\n", Long_Long_Add((long long)200000000000, (long long)200000000000));

	// IRQ 요청 진행
	Send_Uart_Request(1);
	Send_Key3_Request(1);
	Send_Key4_Request(1);


	Uart_Printf("당신의 이름을 입력하시고 enter를 쳐주세요!\n");
	Lcd_Printf(600, 300, WHITE, BLACK, 1, 1, "당신의 이름을 입력하시고 enter를 쳐주세요!");

	for (;;)
	{
		for (i=0; i<100; i++)
		{
			//Uart_Printf("[app1] %d\n", i);
			Delay(DELAY);
			uart_response = Get_Uart_Response(1);
			key3_response = Get_Key3_Response(1);
			key4_response = Get_Key4_Response(1);
			if (uart_response)
			{
				if (uart_response == 13)
				{
					if (name_idx == 0)
						continue;



					name[name_idx] = '\0';
					Uart1_Printf("=====================================\n");
					Uart1_Printf("Hi ***%s***, You look Perfect Tonight~~\n", name);
					Uart1_Printf("=====================================\n\n");
					name_idx = 0;
					Uart_Printf("당신의 이름을 입력하시고 enter를 쳐주세요!\n");

					Lcd_Draw_Bar(512, 0, 1024, 600, BLACK);
//					Lcd_Printf(600, 285, WHITE, BLACK, 1, 1,"=====================================");
//					Lcd_Printf(600, 300, WHITE, BLACK, 1, 1,"Hi ***%s***, You look Perfect Tonight~~", name);
//					Lcd_Printf(600, 315, WHITE, BLACK, 1, 1,"=====================================");

					Lcd_Printf(540, 284, WHITE, BLACK, 2, 2,"Hi %s, ", name);
					Lcd_Printf(540, 316, WHITE, BLACK, 2, 2,"You look Perfect Tonight~~");

					Delay(DELAY*6);
					Lcd_Draw_Bar(512, 0, 1024, 600, BLACK);
					Lcd_Printf(600, 300, WHITE, BLACK, 1, 1,"당신의 이름을 입력하시고 enter를 쳐주세요!\n");
				}
				else if (!((65 <= uart_response && uart_response <= 90) || (97 <= uart_response && uart_response <= 122)))
				{
					Uart1_Printf("Please input a valid character!!!\n");
					Lcd_Draw_Bar(512, 0, 1024, 600, BLACK);
					Lcd_Printf(600, 300, WHITE, BLACK, 1, 1, "Please input a valid character!!!");
					Delay(DELAY*6);
					Lcd_Draw_Bar(512, 0, 1024, 600, BLACK);
					name_idx = 0;
					Uart_Printf("당신의 이름을 입력하시고 enter를 쳐주세요!\n");
					Lcd_Printf(600, 300, WHITE, BLACK, 1, 1, "당신의 이름을 입력하시고 enter를 쳐주세요!");
				}
				else {
					tmp = (97 <= uart_response && uart_response <= 122) ? uart_response - 32 : uart_response;
					name[name_idx] = tmp;

					name_idx++;
				}

			}
			if (key3_response)
			{
				Uart1_Printf("[App1] key3이 눌렸어요\n");

//				Uart1_Printf("                 _ _  \n");
//				Uart1_Printf("                (_|_) \n");
//				Uart1_Printf("   __ _ ___  ___ _ _  \n");
//				Uart1_Printf("  / _` / __|/ __| | | \n");
//				Uart1_Printf(" | (_| \\__ \\ (__| | | \n");
//				Uart1_Printf("  \\__,_|___/\\___|_|_| \n");
//				Uart1_Printf("                      \n");
//				Uart1_Printf("\n");
				Uart_Printf("	 __        _______ \n");
				Uart_Printf("	|  |      /  _____|\n");
				Uart_Printf("	|  |     |  |  __  \n");
				Uart_Printf("	|  |     |  | |_ | \n");
				Uart_Printf("	|  `----.|  |__| | \n");
				Uart_Printf("	|_______| \\______| \n\n");


				Lcd_Draw_Bar(512, 0, 1024, 600, BLACK);
				Lcd_Printf(660, 260, WHITE, BLACK, 1, 1," __        _______ ");
				Lcd_Printf(660, 276, WHITE, BLACK, 1, 1,"|  |      /  _____|");
				Lcd_Printf(660, 292, WHITE, BLACK, 1, 1,"|  |     |  |  __  ");
				Lcd_Printf(660, 308, WHITE, BLACK, 1, 1,"|  |     |  | |_ | ");
				Lcd_Printf(660, 324, WHITE, BLACK, 1, 1,"|  `----.|  |__| | ");
				Lcd_Printf(660, 340, WHITE, BLACK, 1, 1,"|_______| \\______|");

				Delay(DELAY*4);
				Lcd_Draw_Bar(512, 0, 1024, 600, BLACK);
				Lcd_Printf(600, 300, WHITE, BLACK, 1, 1, "당신의 이름을 입력하시고 enter를 쳐주세요!");
			}
			if (key4_response)
			{
				Uart1_Printf("[App1] key4이 눌렸어요\n");
//				Uart1_Printf("                 _ _  \n");
//				Uart1_Printf("                (_|_) \n");
//				Uart1_Printf("   __ _ ___  ___ _ _  \n");
//				Uart1_Printf("  / _` / __|/ __| | | \n");
//				Uart1_Printf(" | (_| \\__ \\ (__| | | \n");
//				Uart1_Printf("  \\__,_|___/\\___|_|_| \n");
//				Uart1_Printf("                      \n");
//				Uart1_Printf("\n");
				Uart_Printf("	.______     ______     ______   .___________.  ______      ___      .___  ___. .______   \n");
				Uart_Printf("	|   _  \\   /  __  \\   /  __  \\  |           | /      |    /   \\     |   \\/   | |   _  \\  \n");
				Uart_Printf("	|  |_)  | |  |  |  | |  |  |  | `---|  |----`|  ,----'   /  ^  \\    |  \\  /  | |  |_)  | \n");
				Uart_Printf("	|   _  <  |  |  |  | |  |  |  |     |  |     |  |       /  /_\\  \\   |  |\\/|  | |   ___/  \n");
				Uart_Printf("	|  |_)  | |  `--'  | |  `--'  |     |  |     |  `----. /  _____  \\  |  |  |  | |  |      \n");
				Uart_Printf("	|______/   \\______/   \\______/      |__|      \\______|/__/     \\__\\ |__|  |__| | _|      \n");

				Lcd_Draw_Bar(512, 0, 1024, 600, BLACK);

				Lcd_Printf(540, 212, WHITE, BLACK, 1, 1,".______     ______     ______   .___________.");
				Lcd_Printf(540, 228, WHITE, BLACK, 1, 1,"|   _  \\   /  __  \\   /  __  \\  |           |");
				Lcd_Printf(540, 244, WHITE, BLACK, 1, 1,"|  |_)  | |  |  |  | |  |  |  | `---|  |----`");
				Lcd_Printf(540, 260, WHITE, BLACK, 1, 1,"|   _  <  |  |  |  | |  |  |  |     |  | ");
				Lcd_Printf(540, 276, WHITE, BLACK, 1, 1,"|  |_)  | |  `--\'  | |  `--\'  |     |  |");
				Lcd_Printf(540, 292, WHITE, BLACK, 1, 1,"|______/   \\______/   \\______/      |__|     ");

				Lcd_Printf(540, 308, WHITE, BLACK, 1, 1,"  ______      ___      .___  ___. .______   ");
				Lcd_Printf(540, 324, WHITE, BLACK, 1, 1," /      |    /   \\     |   \\/   | |   _  \\  ");
				Lcd_Printf(540, 340, WHITE, BLACK, 1, 1,"|  ,----\'   /  ^  \\    |  \\  /  | |  |_)  | ");
				Lcd_Printf(540, 356, WHITE, BLACK, 1, 1,"|  |       /  /_\\  \\   |  |\\/|  | |   ___/  ");
				Lcd_Printf(540, 372, WHITE, BLACK, 1, 1,"|  `----. /  _____  \\  |  |  |  | |  |      ");
				Lcd_Printf(540, 388, WHITE, BLACK, 1, 1," \\______|/__/     \\__\\ |__|  |__| | _|      ");


				Delay(DELAY*4);
				Lcd_Draw_Bar(512, 0, 1024, 600, BLACK);
				Lcd_Printf(600, 300, WHITE, BLACK, 1, 1, "당신의 이름을 입력하시고 enter를 쳐주세요!");

			}
		}
	}

	// IRQ 요청 진행
	Clear_Uart_Request(1);
	Clear_Key3_Request(1);
	Clear_Key4_Request(1);

}
