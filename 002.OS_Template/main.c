#include "device_driver.h"
#include "process.h"
#include "pcb_allocator.h"

extern WIN_INFO_ST ArrWinInfo[5];

#define BLACK	0x0000
#define WHITE	0xffff
#define BLUE	0x001f
#define GREEN	0x07e0
#define RED		0xf800
#define YELLOW	0xffe0
#define VIOLET	0xf81f

#define 	RAM_APP0			0x44100000
#define 	RAM_APP1			(RAM_APP0+SIZE_APP0)
#define 	SIZE_STACK0			(1*1024*1024)
#define 	SIZE_STACK1			(1*1024*1024)
#define 	STACK_LIMIT_APP0	(RAM_APP1+SIZE_APP1)
#define 	STACK_LIMIT_APP1	(STACK_LIMIT_APP0+SIZE_STACK0)
#define 	STACK_BASE_APP0		(STACK_LIMIT_APP0+SIZE_STACK0)
#define 	STACK_BASE_APP1		(STACK_LIMIT_APP1+SIZE_STACK1)

#define 	SIZE_APP0			(4*1024*1024)
#define 	SIZE_APP1			(4*1024*1024)

#define 	SECTOR_APP0			100
#define 	SECTOR_APP1			5000

#define SECTOR_SIZE 		512
#define ALIGN_SECTOR(x)	 	((((x+(SECTOR_SIZE-1))&~(SECTOR_SIZE-1))/SECTOR_SIZE))

void App_Read(unsigned int sector, unsigned int size, unsigned int addr)
{
	int i;

	for(i=sector; i<(sector+ALIGN_SECTOR(size)); i++)
	{
		SD_Read_Sector(i, 1,(void *)addr);
		addr += SECTOR_SIZE;
	}
}

void Main(void)
{
	CoInitMmuAndL1L2Cache();
	Uart_Init(115200);
	LED_Init();
	Key_Poll_Init();

	Timer0_Int_Delay(1, 2000);

	Uart_Printf("\nOS Template\n");

	ArrWinInfo[0].bpp_mode = BPPMODE_16BPP_565;
	ArrWinInfo[0].bytes_per_pixel = 2;
	ArrWinInfo[0].p_sizex = 1024;
	ArrWinInfo[0].p_sizey = 600;
	ArrWinInfo[0].v_sizex = 1024;
	ArrWinInfo[0].v_sizey = 600;
	ArrWinInfo[0].posx = (1024 - ArrWinInfo[0].p_sizex) / 2;
	ArrWinInfo[0].posy = (600 - ArrWinInfo[0].p_sizey) / 2;

	Lcd_Init();
	Lcd_Win_Init(0, 1);
	Lcd_Brightness_Control(8);

	Lcd_Select_Display_Frame_Buffer(0, 0);
	Lcd_Select_Draw_Frame_Buffer(0, 0);
	Lcd_Draw_Back_Color(GREEN);

	GIC_CPU_Interface_Enable(0,1);
	GIC_Set_Priority_Mask(0,0xFF);
	GIC_Distributor_Enable(1);

#if 0 // SD Loading
	{
		extern volatile unsigned int sd_insert_flag;
		SDHC_Init();
		SDHC_ISR_Enable(1);
		if(!sd_insert_flag) Uart_Printf("SD 카드 삽입 요망!\n");
		while(!sd_insert_flag);
		SDHC_Card_Init();

		Uart_Printf("APP0 Loading!\n");
		App_Read(SECTOR_APP0, SIZE_APP0, RAM_APP0);
		Uart_Printf("APP1 Loading!\n");
		App_Read(SECTOR_APP1, SIZE_APP0, RAM_APP1);
	}
#endif

	// app 마다 pcb 만들고 linked list 구조체에 넣어주기
	struct PCB *pcb_app0_addr = (struct PCB *) malloc(sizeof(struct PCB));
	struct PCB *pcb_app1_addr = (struct PCB *) malloc(sizeof(struct PCB));
	Uart_Printf("\n app0 pcb_addr: %X\n", pcb_app0_addr);
	Uart_Printf("\n app1 pcb_addr: %X\n", pcb_app1_addr);

	// app0의 PCB 초기화
	pcb_app0_addr->PID = 0;

	// app1의 PCB 초기화
	pcb_app1_addr->PID = 1;
	pcb_app1_addr->CPSR = Get_CPSR(); //이미 T bit는 0
	pcb_app1_addr->CPSR |= 0x1F; //sys mode로 강제 변환
	pcb_app1_addr->registers[0] = 123;
	Uart_Printf("app1 cpsr: %X\n", pcb_app1_addr->CPSR);
	pcb_app1_addr->PC = RAM_APP0; //VA 영역
	Uart_Printf("app1 PC: %X\n", pcb_app1_addr->PC);

	// pcb linked list 구조체 생성된 pcb의 주소값 넣어주기
	add_pcb((PCB_ADR) pcb_app0_addr);
	add_pcb((PCB_ADR) pcb_app1_addr);

	for(;;)
	{
		unsigned char x;

		// Uart_Printf("\n실행할 APP을 선택하시오 [1]APP0, [2]APP1 >> ");
		// x = Uart1_Get_Char();
		//app0을 먼저 실행
		Uart_Printf("app0 부터 실행합니다\n");
		x = '1';
		/*
		if(x == '1')
		{
			Uart_Printf("\nAPP0 RUN\n", x);
			SetTransTable(RAM_APP0, (RAM_APP0+SIZE_APP0-1), RAM_APP0, RW_WBWA);
			SetTransTable(STACK_LIMIT_APP0, STACK_BASE_APP1-1, STACK_LIMIT_APP0, RW_WBWA);
			CoInvalidateMainTlb();
			//struct PCB * pcb_address = (struct PCB *) ptr_PCB_Current->pcb_addr;
			//Uart_Printf("\n app0 pcb_addr output: %X\n", pcb_address);

			//asid 저장
			Run_App(RAM_APP0, STACK_BASE_APP0);
		}

		if(x == '2')
		{
			Uart_Printf("\nAPP1 RUN\n", x);
			SetTransTable_app2(RAM_APP0, (RAM_APP0+SIZE_APP1-1), RAM_APP1, RW_WBWA);
			SetTransTable(STACK_LIMIT_APP1, STACK_BASE_APP1-1, STACK_LIMIT_APP1, RW_WBWA);
			CoInvalidateMainTlb();
			Run_App(RAM_APP0, STACK_BASE_APP1);
		}
		*/
		Uart_Printf("\nAPP0 RUN\n", x);
		SetTransTable(RAM_APP0, (RAM_APP0+SIZE_APP0-1), RAM_APP0, RW_WBWA);
		SetTransTable(STACK_LIMIT_APP0, STACK_BASE_APP1-1, STACK_LIMIT_APP0, RW_WBWA);
		SetTransTable_app2(RAM_APP0, (RAM_APP0+SIZE_APP1-1), RAM_APP1, RW_WBWA);
		SetTransTable(STACK_LIMIT_APP1, STACK_BASE_APP1-1, STACK_LIMIT_APP1, RW_WBWA);
		CoInvalidateMainTlb();
		Run_App(RAM_APP0, STACK_BASE_APP0);

	}
	free(pcb_app0_addr);
	free(pcb_app1_addr);
}
