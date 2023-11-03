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
