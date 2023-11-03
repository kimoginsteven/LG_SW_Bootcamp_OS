/*
 * process.h
 *
 *  Created on: 2023. 11. 1.
 *      Author: RENTALHUB
 */

#ifndef PROCESS_H_
#define PROCESS_H_

struct PCB {
	unsigned int PID;
	unsigned int ASID; // cache용
	unsigned int PC;
	unsigned int CPSR;
	unsigned int registers[15]; // r0-r14
};

extern struct PCB *pcb_app0_addr;
extern struct PCB *pcb_app1_addr;

void pcb_malloc(void);
void pcb_init(unsigned int RAM_APP0, unsigned int STACK_BASE_APP0, unsigned int STACK_BASE_APP1);
void pcb_add_to_list(void);
void pcb_free(void);

#endif /* PROCESS_H_ */

