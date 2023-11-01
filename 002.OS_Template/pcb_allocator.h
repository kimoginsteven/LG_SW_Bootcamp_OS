#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_REGISTER 16

typedef unsigned int PCB_ADR;

typedef struct PCB_NODE {
	PCB_ADR pcb_addr;
	struct PCB_NODE * before;
	struct PCB_NODE * after;
}PCB_NODE;

PCB_NODE * ptr_PCB_Creator;
PCB_NODE * ptr_PCB_Current;
PCB_NODE * ptr_PCB_Head;

extern PCB_NODE * allocate_pcb(PCB_ADR adr);
extern void add_pcb(PCB_ADR adr);

extern PCB_ADR get_current_pcb_adr();
extern void set_current_pcb_adr(PCB_ADR pcb_addr);
extern PCB_ADR get_next_pcb_adr();
