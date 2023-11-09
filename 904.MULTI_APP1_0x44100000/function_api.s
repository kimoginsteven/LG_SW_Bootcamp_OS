	.code	32
	.text

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ UTILITY FUNCTIONS
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.global Lcd_Clr_Screen
Lcd_Clr_Screen:
	svc		0
	bx		lr

	.global Lcd_Draw_BMP
Lcd_Draw_BMP:
	svc		1
	bx		lr

	.global Uart1_Printf
Uart1_Printf:
	svc		2
	bx		lr

	.global LED_Display
LED_Display:
	svc		3
	bx		lr

	.global Key_Get_Key_Pressed
Key_Get_Key_Pressed:
	svc		4
	bx		lr

	.global Key_Wait_Key_Released
Key_Wait_Key_Released:
	svc		5
	bx		lr

	.global Key_Wait_Key_Pressed
Key_Wait_Key_Pressed:
	svc		6
	bx		lr

	.global Print_Hello
Print_Hello:
	svc		7
	bx		lr

	.global Sqr
Sqr:
	svc		8
	bx		lr

	.global Long_Long_Add
Long_Long_Add:
	svc		9
	bx		lr

@IRQ_Request를 위한 함수
	.global Send_Uart_Request
Send_Uart_Request:
	svc		10
	bx		lr

	.global Send_Key3_Request
Send_Key3_Request:
	svc		11
	bx		lr

	.global Send_Key4_Request
Send_Key4_Request:
	svc		12
	bx		lr

	.global Clear_Uart_Request
Clear_Uart_Request:
	svc		13
	bx		lr

	.global Clear_Key3_Request
Clear_Key3_Request:
	svc		14
	bx		lr

	.global Clear_Key4_Request
Clear_Key4_Request:
	svc		15
	bx		lr

	.global Get_Uart_Response
Get_Uart_Response:
	svc		16
	bx		lr

	.global Get_Key3_Response
Get_Key3_Response:
	svc		17
	bx		lr

	.global Get_Key4_Response
Get_Key4_Response:
	svc		18
	bx		lr
	.global Lcd_Printf
Lcd_Printf:
	svc		19
	bx 		lr
	.global Lcd_Draw_Bar
Lcd_Draw_Bar:
	svc		20
	bx 		lr
	.end
