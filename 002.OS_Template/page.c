#include "page.h"
#include "device_driver.h"
#include "option.h"
#include "cp15.h"

struct PA_page_info PA_page_info_list[MAX_PA_PAGE]; //main.c 에서 동적 할당해줄 예정
unsigned int page_counter = 0;
unsigned int swap_counter = 0;

void demand_paging (unsigned int fault_addr)
{
	unsigned int cur_asid = Get_ASID();
	unsigned int snd_page_table_base;
	unsigned int physical_address;
	unsigned int* pTT;
	unsigned int snd_page_table_index;
	unsigned int *fst_TT_descriptor;
	int i;
	unsigned int section_id = fault_addr >> 20;

	Uart1_Printf("cur asid : %X\n", cur_asid);
	if (page_counter >= 256)
	{
		//swap();
		Uart_Printf("Swapping!!!!!!\n");
		for (;;);
		//swap_count %=256
	}
	else
	{
		// 구조체 업데이트
		PA_page_info_list[page_counter].is_allocated = 1;
		PA_page_info_list[page_counter].asid = cur_asid; // 현재 app0의 pcb로부터 pid 저장;
		PA_page_info_list[page_counter].virtual_address = fault_addr; // 나중에 swap out 할 때 필요한 정보

		if (cur_asid == 0) //app0
		{
			fst_TT_descriptor = (unsigned int *)MMU_PAGE_TABLE_BASE + section_id;
			snd_page_table_base = SND_PAGE_TABLE_BASE_APP0;
		}
		else
		{
			fst_TT_descriptor = (unsigned int *)0x44500000 + section_id;
			snd_page_table_base = SND_PAGE_TABLE_BASE_APP1;
		}

		// mmu 번역 정보 업데이트: 1차 page table에서 바로 물리 메모리로 가도록 수정
		*fst_TT_descriptor = fault_addr >> 20 << 20;
		*fst_TT_descriptor |= 3<<9 | 2; // 접근 허용하기 위한 AP bit 수정, 하위 2bit 1,0으로 수정
		Uart_Printf("page table base addr value : 0x%X 0x%X\n", fst_TT_descriptor, *fst_TT_descriptor);

		//적재할 물리 메모리 공간 찾기
		physical_address = (0x44b00000 + (page_counter*0x1000));
		physical_address >>= 12;
		physical_address <<= 12;

		for (i=0; i<10; i++)
			Uart_Printf("[test], addr, value : 0x%X, 0x%X\n",fault_addr + (i*4), *((unsigned int *)fault_addr + i));
		Uart_Printf("-\n");
		memcpy((void *)physical_address, (void *)(fault_addr >> 12 << 12), 0x1000); //swap file에서 물리 메모리로 적재
		for (i=0; i<10; i++)
			Uart_Printf("test : 0x%X\n", *((unsigned int *)physical_address + i));
		Uart_Printf("-\n");

		CoInvalidateMainTlb();
		// mmu 번역 정보 업데이트: 2차 page table로 가도록 수정
		*fst_TT_descriptor = snd_page_table_base;
		*fst_TT_descriptor |= 0x1;
		Uart_Printf("page table base after change addr value : 0x%X 0x%X\n", fst_TT_descriptor, *fst_TT_descriptor);

		//2차 page table access permission bit와 매핑할 주소 수정
		snd_page_table_index =  (fault_addr >> 12) % 0x100;
		pTT = (unsigned int *) snd_page_table_base + snd_page_table_index;
		Uart_Printf("2nd page table index:  0x%X\n", snd_page_table_index);
		Uart_Printf("New 2nd page table address before change 0x%X\n", pTT);
		Uart_Printf("New page table descriptor before change0x%X\n", *pTT);
		*pTT = physical_address;
		*pTT |= (0x3 << 4) | 0x2; //access bit 접근가능하도록 바꿔주기
		Uart_Printf("New 2nd page table address after change0x%X\n", pTT);
		Uart_Printf("New page table descriptor after change0x%X\n", *pTT);

		CoInvalidateMainTlb();
		page_counter++;
		Uart_Printf("=============================================\n\n");

	}
}
