#include "pcb_allocator.h"

PCB_NODE * allocate_pcb(PCB_ADR pcb_addr) {
	PCB_NODE * node = (PCB_NODE*)malloc(sizeof(PCB_NODE));
	if (node == NULL) {
		// error
	}

	node->pcb_addr = pcb_addr;

	node->before = NULL;
	node->after = NULL;

	return node;
}

void add_pcb(PCB_ADR pcb_addr) {
	PCB_NODE * node = allocate_pcb(pcb_addr);

	if (ptr_PCB_Creator == NULL) {
		ptr_PCB_Creator = node;
		ptr_PCB_Current = node;
		ptr_PCB_Head = node;

		node->after = ptr_PCB_Creator;
		node->before = ptr_PCB_Creator;
	}
	else {
		node->before = ptr_PCB_Creator;
		node->after = ptr_PCB_Creator->after;
		ptr_PCB_Creator->after->before = node;
		ptr_PCB_Creator->after = node;
		ptr_PCB_Creator = node;
	}
}

PCB_ADR get_current_pcb_adr() {
	return ptr_PCB_Current->pcb_addr;
}

void set_current_pcb_adr(PCB_ADR pcb_addr) {
	ptr_PCB_Current->pcb_addr = pcb_addr;
}

PCB_ADR get_next_pcb_adr() {
	ptr_PCB_Current = ptr_PCB_Current->after;
	return ptr_PCB_Current->pcb_addr;
}

