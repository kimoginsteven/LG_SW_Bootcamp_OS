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
	unsigned int registers[16]; //r0-r14
};

#endif /* PROCESS_H_ */
