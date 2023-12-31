	.include "4412addr.inc"
	.include "option.inc"

	.code	32
	.text
 
	.global __start
__start:

@--------------------------------------------------
@ Exception Vector Configuration
@--------------------------------------------------

	b		ResetHandler
	b		HandlerUndef		@ HandlerUndef
	b		HandlerSVC			@ handler for SVC
	b		HandlerPabort 		@ HandlerPabort
	b		HandlerDabort		@ HandlerDabort
	b		.					@ reserved
	b		HandlerIRQ			@ HandlerIRQ
	b		.					@ HandlerFIQ

@--------------------------------------------------
@ IRQ Handler
@--------------------------------------------------

	.extern ISR_Vector

HandlerIRQ:
	sub		sp,sp,#4
	push	{r0, lr}
	ldr		lr, =ICCIAR_CPU0
	ldr		r0, [lr]
	ldr		lr, =0x3FF
	and		r0, r0, lr
	ldr		lr, =ISR_Vector
	ldr		r0, [lr, r0, lsl #2]
	str		r0, [sp, #8]
	pop		{r0, lr, pc}

@--------------------------------------------------
@ Exception Handler
@--------------------------------------------------

	.extern		Undef_Handler
	.extern		Dabort_Handler
	.extern		Pabort_Handler
	.extern		SVC_Handler

HandlerUndef:
	stmfd	sp!,{r0-r3, r12, lr}
	sub 	r0, lr, #4
	mrs		r1, spsr
	and		r1, r1, #0x1f
	bl		Undef_Handler
	ldmfd	sp!,{r0-r3, r12, lr}
	subs	pc, lr, #4

@ ������ ���Ͽ� ������ �߻��� ���� �ּҷ� �����ϵ��� ���� @

HandlerDabort:
	stmfd	sp!,{r0-r3, r12, lr}

	mrc 	p15, 0, r0, c5, c0, 0  @r0에 dfsr 저장하기
	ldr		r1, =0x140f
	and		r0, r0, r1
	cmp		r0, #0xf  @permission fault, page인지 확인
	beq		1f

	sub 	r0, lr, #8
	mrs		r1, spsr
	and		r1, r1, #0x1f
	bl		Dabort_Handler
	ldmfd	sp!,{r0-r3, r12, lr}
	@subs	pc, lr, #8
	subs	pc, lr, #4

1:
	mrc		p15, 0, r0, c6, c0, 0 @인자로 permission fault page 주소 전달
	blx		Page_Fault_Handler_DABT
	ldmfd	sp!, {r0-r3, r12, lr}
	subs	pc, lr, #8	@발생한 주소로 다시 복귀

HandlerPabort:
	stmfd	sp!,{r0-r3, r12, lr}

	mrc		p15, 0, r0, c5, c0, 1  @r0에 ifsr 저장하기
	ldr		r1, =0x140f
	and		r0, r0, r1
	cmp		r0, #0xf  @permission fault, page인지 확인
	beq		2f

	sub 	r0, lr, #4
	mrs		r1, spsr
	and		r1, r1, #0x1f
	bl		Pabort_Handler
	ldmfd	sp!,{r0-r3, r12, lr}
	subs	pc, lr, #4

2:
	mrc		p15, 0, r0, c6, c0, 2 @인자로 permission fault page 주소 전달
	blx		Page_Fault_Handler_PABT
	ldmfd	sp!, {r0-r3, r12, lr}
	subs	pc, lr, #4	@발생한 주소로 다시 복귀

    .extern SVC_Handler_Vector
HandlerSVC:
	push	{r4-r6, lr}
	ldr		r4, [lr, #-4]
	ldr		r5, =SVC_Handler_Vector
	bic		r4, r4, #0xFF << 24 @get svc 호출 번호
	cps		#0x1f
	mov		r6, lr	@user sys mode의 lr 백업
	ldr		r5, [r5, r4, lsl #2] @SVC_Handler_vector안에 함수 실행
	blx 	r5
	mov		lr, r6 @user sys mode의 lr 복원
	cps		#0x13
	ldmfd	sp!, {r4-r6, pc}^

@--------------------------------------------------
@ Reset Handler Routine
@--------------------------------------------------

ResetHandler:

@--------------------------------------------------
@ Watchdog Disable
@--------------------------------------------------

	ldr		r0, =WTCON
	ldr		r1, =0x0
	str		r1, [r0]

@--------------------------------------------------
@ Coprocessor Config
@--------------------------------------------------

	@ NEON & VFP Enable

	LDR     r0,=0x05555555
	MCR     p15,0,r0,c1,c0,2
	LDR     R0,=(1<<30)
	VMSR    FPEXC,r0

	@ L1-I, L1-D Cache, Branch Predictor OFF
	mov		r1, #0
	MCR     p15,0,r1,c1,c0,0

	@ L2 Cache OFF
	mov		r1, #0
	MCR 	p15,0,R1,C1,C0,1

	@ I-Cache(12), Flow Prediction(11)

	LDR     r1,=(1<<12)|(1<<11) 	@ [4] I-Cache ON, Flow Prediction ON
	MCR     p15,0,r1,c1,c0,0

@--------------------------------------------------
@ CLOCK Configuration
@--------------------------------------------------

@--------------------------------------------------
@ DRAM Configuration
@--------------------------------------------------

@--------------------------------------------------
@ GPIO(LED OUTPUT) ON
@--------------------------------------------------
	
	@ LED Port Control

	ldr		r0, =GPM4CON
	ldr		r1, [r0]
	bic		r1, r1, #0xff<<16
	orr		r1, r1, #0x11<<16
	str		r1, [r0]

	@ Turn LED on (XO)

	ldr		r0, =GPM4DAT
	ldr		r1, [r0]
	bic		r1, r1, #0x3<<4
	orr		r1, r1, #0x1<<4
	str		r1, [r0]

@--------------------------------------------------
@ C Runtime Startup
@--------------------------------------------------

	@ RAM loading

	.extern __ZI_BASE__
	.extern __ZI_LIMIT__

	ldr		r0, =__ZI_BASE__
	ldr		r1, =__ZI_LIMIT__
	mov		r2, #0x0
1:
	cmp		r0, r1
	strlo	r2, [r0], #4
	blo		1b

	@ Stack mounting

	msr		cpsr_c, #(IRQ_BIT|FIQ_BIT|IRQ_MODE)
	ldr		sp, =IRQ_STACK_BASE

	msr		cpsr_c, #(IRQ_BIT|FIQ_BIT|FIQ_MODE)
	ldr		sp, =FIQ_STACK_BASE

	msr		cpsr_c, #(IRQ_BIT|FIQ_BIT|UNDEF_MODE)
	ldr		sp, =UNDEF_STACK_BASE

	msr		cpsr_c, #(IRQ_BIT|FIQ_BIT|ABORT_MODE)
	ldr		sp, =ABORT_STACK_BASE

	msr		cpsr_c, #(SVC_MODE)
	ldr		sp, =SVC_STACK_BASE

	@ Set Exception Vector Address

	ldr		r0, =DRAM_START
	mcr     p15,0,r0,c12,c0,0

	@ Call Main

	bl		Main

	@ HALT

	b		.

	.end
