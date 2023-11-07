	.include "4412addr.inc"
	.include "option.inc"

	.code	32
	.text

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ UTILITY FUNCTIONS
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global Run_App
Run_App:

	push	{r4, lr}
	mrs		r4, cpsr
	cps		#0x1f
	mov 	sp, r1
	blx		r0
	msr		cpsr_cxsf, r4
	pop		{r4, pc}

	.global Get_User_SP
Get_User_SP:

	mrs		r1, cpsr
	cps		#0x1f
	mov 	r0, sp
	msr		cpsr_cxsf, r1
	bx 		lr

	.global Get_User_Stack_Base
Get_User_Stack_Base:

	ldr		r0, =SYS_STACK_BASE
	bx 		lr

	.global Get_User_Stack_Limit
Get_User_Stack_Limit:

	ldr		r0, =STACK_LIMIT
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int PABT_Falut_Status(void)
@ 명령어 Fault 발생 이유를 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  PABT_Falut_Status
PABT_Falut_Status:

	mrc		p15, 0, r0, c5, c0, 1
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int PABT_Falut_Address(void)
@ 데이터 Fault 발생 주소를 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  PABT_Falut_Address
PABT_Falut_Address:

	mrc		p15, 0, r0, c6, c0, 2
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int DABT_Falut_Status(void)
@ 데이터 Fault 발생 이유를 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  DABT_Falut_Status
DABT_Falut_Status:

	mrc		p15, 0, r0, c5, c0, 0
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int DABT_Falut_Address(void)
@ 데이터 Fault 발생 주소를 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global  DABT_Falut_Address
DABT_Falut_Address:

	mrc		p15, 0, r0, c6, c0, 0
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int Get_SP(void)
@ 현재 모드의 스택 포인터 값을 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global Get_SP
Get_SP:

	mov 	r0, sp
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int Main_ID(void)
@ Core의 버전 값을 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global Main_ID
Main_ID:

	mrc 	p15, 0, r0, c0, c0, 0
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int TLB_Type(void)
@ TLB Type 값을 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.global TLB_Type
TLB_Type:

	mrc 	p15, 0, r0, c0, c0, 3
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int Get_CPSR(void);
@ CPSR 값을 리턴
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.global Get_CPSR
Get_CPSR:
	mrs		r0, cpsr
	bx 		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void Save_Context(void);
@ context 백업을 위한 코드
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.extern ptr_PCB_Current
	.extern Timer0_ISR_context_switch
	.global Save_Context
Save_Context:

	@IRQ mode의  백업
	push	{r0, r14}

	@현재까지 실행되던 app의 pcb 주소 가지고 오기, r13 : pcb의 시작 주소
	ldr		r14, =ptr_PCB_Current
	ldr		r14, [r14]
	ldr 	r14, [r14, #0]

	@r0-r14까지 백업하기
	add		r14, r14, #16
	stmia	r14, {r0-r14}^ @user/sys 모드의 register 복원, stmeqia를 안했다
	sub		r14, r14, #16

	mov		r1, r14 //pcb node의 주소를 r1으로 변경, 이제 r1을 마음것 사용해도 됨

	@IRQ mode의  복원
	pop		{r0, r14} //이제 r14는 복귀할 주소 가르킴

	@현재까지 실행되던 app에서 다음 실행할 명령어의 주소를 pc에 저장
	sub		r0, r14, #4
	str		r0, [r1, #8]

	@현재까지 실행되던 app의 cpsr 상태 저장하기
	mrs		r0, spsr
	str		r0, [r1, #12]


	b		Timer0_ISR_context_switch

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void Get_Context_And_Switch(void);
@ context 복원을 위한 코드
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.extern ptr_PCB_Current
	.global Get_Context_And_Switch
Get_Context_And_Switch:

	@현재까지 실행되던 app의 pcb 주소 가지고 오기
	ldr		r14, =ptr_PCB_Current
	ldr		r14, [r14]
	ldr 	r14, [r14, #0]

	@r0-r14까지 복원
	add		r14, r14, #16
	ldmia	r14, {r0-r14}^ //user,sys 모든 register 저장
	sub		r14, r14, #16

	@r0를 사용하기 전에 미리 백업
	push	{r0-r1}

	@spsr 복원
	add		r0, r14, #12
	ldr		r0, [r0]
	msr		spsr, r0

	@r0를 복원
	pop		{r0-r1}

	@ 다음 실행할 명령를 pc로 전달
	add 	r14, r14, #8
	ldr		r14,[r14]
	movs	pc, r14

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ unsigned int Get_ASID(void);
@ 현재 process의 ASID 반환
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.global Get_ASID
Get_ASID:
	mrc		p15, 0, r0, c13, c0, 1
	and		r0, r0, #0xff
	isb
	bx		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void Set_ASID(unsigned int);
@ 현재 process의 ASID set
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.global Set_ASID
Set_ASID:
	mrc		p15, 0, r1, c13, c0, 1
	bic		r1, r1, #0xff
	orr		r1, r1, r0
	mcr		p15, 0, r1, c13, c0, 1
	isb
	bx		lr

	.global call_isb
call_isb:
	isb
	bx		lr
	.end
