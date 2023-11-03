#include "process.h"
#include "pcb_allocator.h"
#include "device_driver.h"

struct PCB *pcb_app0_addr;
struct PCB *pcb_app1_addr;

// app 마다 pcb 동적 할당
void pcb_malloc(void) {
	pcb_app0_addr = (struct PCB *) malloc(sizeof(struct PCB));
	pcb_app1_addr = (struct PCB *) malloc(sizeof(struct PCB));
}

// 각 app의 PCB 초기화
void pcb_init(unsigned int RAM_APP0, unsigned int STACK_BASE_APP0, unsigned int STACK_BASE_APP1) {
	// app0의 PCB 초기화
	pcb_app0_addr->PID = 0x0;
	pcb_app0_addr->ASID = 0x0;
	pcb_app1_addr->CPSR = Get_CPSR(); // 이미 T bit는 0
	pcb_app0_addr->CPSR |= 0x1F; //sys mode로 강제 변환
	pcb_app0_addr->registers[13] = STACK_BASE_APP0;
	pcb_app0_addr->PC = RAM_APP0; //VA 영역

	// app1의 PCB 초기화
	pcb_app1_addr->PID = 0x1;
	pcb_app0_addr->ASID = 0x1;
	pcb_app1_addr->CPSR = Get_CPSR(); // 이미 T bit는 0
	pcb_app1_addr->CPSR |= 0x1F; //sys mode로 강제 변환
	pcb_app1_addr->registers[13] = STACK_BASE_APP1;
	pcb_app1_addr->PC = RAM_APP0; //VA 영역
}

// pcb linked list 구조체안에 생성된 pcb의 주소값 넣어주기
void pcb_add_to_list(void) {
	add_pcb((PCB_ADR) pcb_app0_addr);
	add_pcb((PCB_ADR) pcb_app1_addr);
}

// pcb 공간 free
void pcb_free(void) {
	free(pcb_app0_addr);
	free(pcb_app1_addr);
}
