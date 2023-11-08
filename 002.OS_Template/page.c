#include "page.h"
#include "device_driver.h"
#include "option.h"
#include "cp15.h"
#include "pcb_allocator.h"
#include "process.h"

struct PA_page_info PA_page_info_list[MAX_PA_PAGE]; //main.c 에서 동적 할당해줄 예정
unsigned int page_counter = 0;
unsigned int swap_flag = 0;

void demand_paging (unsigned int fault_addr)
{
	//CoInvalidateMainTlb();
	unsigned int cur_asid = Get_ASID();
	unsigned int section_id = fault_addr >> 20;
	unsigned int mask;
	unsigned int *fst_TT_entry = 0;
	unsigned int *snd_TT_entry = 0;
	unsigned int physical_address = 0;
	unsigned int* snd_tt_descriptor = 0;
	unsigned int snd_page_table_index;
	unsigned int snd_table_offset = 0;
	unsigned int snd_page_table_base;
	/*swap in out만을  위한 변수*/
	unsigned int swapout_asid;
	unsigned int swapout_virtual_address;
	unsigned int swapout_physical_address;
	unsigned int swapout_section_id;

	if (swap_flag) //메모리 공간이 꽉차서 swap out을 진행해야할 경우
	{
		page_counter %= MAX_PA_PAGE;
		swapout_asid = PA_page_info_list[page_counter].asid;
		swapout_virtual_address = PA_page_info_list[page_counter].virtual_address;
		swapout_physical_address = (0x44b00000 + (page_counter*0x1000));
		swapout_physical_address >>= 12;
		swapout_physical_address <<= 12;
		swapout_section_id = swapout_virtual_address >> 20;

		if (swapout_asid == 0) //app0
		{
			fst_TT_entry = (unsigned int *)MMU_PAGE_TABLE_BASE + swapout_section_id;
			*fst_TT_entry = swapout_virtual_address >> 20 << 20;;
		}
		else
		{
			fst_TT_entry = (unsigned int *)MMU_PAGE_TABLE_BASE_APP1 + swapout_section_id;
			*fst_TT_entry = (swapout_virtual_address + 0x400000) >> 20 << 20;
		}
		*fst_TT_entry |= 3<<10 | 2; // 접근 허용하기 위한 AP bit 수정, 하위 2bit 1,0으로 수정
		mask = ~(1u << 15);
		*fst_TT_entry &= mask; //ap bit

		//cache 정책 설정: WT
//		*fst_TT_entry |= WT; // CB bit 1,0 (WT)
//		mask = ~(7u << 12);
//		*fst_TT_entry &= mask; //TEX 000 설정

		//cache 정책 설정: WT_WBWA
		*fst_TT_entry |= WT_WBWA;

		CoInvalidateMainTlb();
		call_isb();

		 //swap_out 하는 페이지의 asid에 따라 ttbr 설정
		//CoSetTTBase((swapout_asid == 1 ? 0x44080000 : 0x44000000) |(1<<6)|(1<<3)|(0<<1)|(0<<0)); //WBWA
		//CoSetTTBase((swapout_asid == 1 ? 0x44080000 : 0x44000000) |(0<<6)|(2<<3)|(0<<1)|(1<<0)); // WT
		CoSetTTBase((swapout_asid == 1 ? 0x44080000 : 0x44000000)|(0<<6)|(1<<3)|(0<<1)|(1<<0)); // WT_WBWA
		call_isb();
		memcpy((void *)(swapout_virtual_address >> 12 << 12), (void *)(swapout_physical_address), 0x1000); // swap file로 swap out
		fst_TT_entry = (unsigned int *)MMU_PAGE_TABLE_BASE + swapout_section_id;

		//section 별 2차 table로 매핑을 위한 offset 설정
		if (swapout_section_id == 0x441)
			snd_table_offset = 0x0;
		else if (swapout_section_id == 0x442)
			snd_table_offset = 0x400;
		else if (swapout_section_id == 0x443)
			snd_table_offset = 0x800;
		else
			snd_table_offset = 0xc00;

		if (swapout_asid == 0) //swap out 해야할 페이지가 app0의 페이지일 경우
		{
			snd_page_table_base = SND_PAGE_TABLE_BASE_APP0 + snd_table_offset;
		}
		else //swap out 해야할 페이지가 app1의 페이지일 경우
		{
			snd_page_table_base = SND_PAGE_TABLE_BASE_APP1 + snd_table_offset;
		}
		// mmu 번역 정보 업데이트: 2차 page table로 가도록 수정
		*fst_TT_entry = snd_page_table_base;
		*fst_TT_entry |= 0x1;

		snd_page_table_index = (swapout_virtual_address % 0x100000)/0x1000;
		snd_TT_entry = (unsigned int *)snd_page_table_base + snd_page_table_index;
		*snd_TT_entry = 0x2; //접근 불가능으로 진행

		//cache 정책 설정: WT
//		*snd_TT_entry |= WT; // WT 설정
//		mask = ~(7u << 6);
//		*snd_TT_entry &= mask; //tex bit 000

		//cache 정책 설정: WT_WBWA
		*snd_TT_entry|= WT_WBWA_PAGE;

		//CoSetTTBase((cur_asid == 1 ? 0x44080000 : 0x44000000)|(1<<6)|(1<<3)|(0<<1)|(0<<0)); //WBWA
		//CoSetTTBase((cur_asid == 1 ? 0x44080000 : 0x44000000)|(0<<6)|(2<<3)|(0<<1)|(1<<0)); //WT
		CoSetTTBase((cur_asid == 1 ? 0x44080000 : 0x44000000)|(0<<6)|(1<<3)|(0<<1)|(1<<0)); // WT_WBWA
		//CoInvalidateMainTlb();
	}

	// 구조체 업데이트
	PA_page_info_list[page_counter].is_allocated = 1;
	PA_page_info_list[page_counter].asid = cur_asid; // 현재 app0의 pcb로부터 pid 저장;
	PA_page_info_list[page_counter].virtual_address = fault_addr; // 나중에 swap out 할 때 필요한 정보

	if (cur_asid == 0) //app0
	{
		fst_TT_entry = (unsigned int *)MMU_PAGE_TABLE_BASE + section_id;
		*fst_TT_entry = fault_addr >> 20 << 20;
	}
	else //app1
	{
		fst_TT_entry = (unsigned int *)MMU_PAGE_TABLE_BASE_APP1 + section_id;
		*fst_TT_entry = (fault_addr + 0x400000) >> 20 << 20;
	}

	// mmu 번역 정보 업데이트: 1차 desciptor table에서 바로 물리 메모리로 접근하도록 수정
	*fst_TT_entry |= 3<<10 | 2; // 접근 허용하기 위한 AP bit 수정, 하위 2bit 1,0으로 수정
	mask = ~(1u << 15);
	*fst_TT_entry &= mask; // 접근 허용하기 위한 AP bit 수정, 상위 AP bit 0으로 수정

	//cache 정책 설정: WT
//	*fst_TT_entry |= WT; // WT 설정
//	mask = ~(7u << 12);
//	*fst_TT_entry &= mask; //TEX 000 설정

	//cache 정책 설정: WT_WBWA
	*fst_TT_entry |= WT_WBWA;

	//적재할 물리 메모리 공간 찾기
	physical_address = (0x44b00000 + (page_counter*0x1000));
	physical_address >>= 12;
	physical_address <<= 12;

	CoInvalidateMainTlb();
	call_isb();

	// swap file에 있는 페이지를 메모리에 적재
	memcpy((void *)physical_address, (void *)(fault_addr >> 12 << 12), 0x1000); //swap file에서 물리 메모리로 적재
	//section 별 2차 table로 매핑을 위한 offset 설정
	if (section_id == 0x441)
		snd_table_offset = 0x0;
	else if (section_id == 0x442)
		snd_table_offset = 0x400;
	else if (section_id == 0x443)
		snd_table_offset = 0x800;
	else if (section_id == 0x444)
		snd_table_offset = 0xc00;

	if (cur_asid == 0) //app0
	{
		snd_page_table_base = SND_PAGE_TABLE_BASE_APP0 + snd_table_offset;
	}
	else
	{
		snd_page_table_base = SND_PAGE_TABLE_BASE_APP1 + snd_table_offset;
	}

	// mmu 번역 정보 업데이트: 2차 page table까지 가서 메모리 접근하도록 수정
	*fst_TT_entry = snd_page_table_base;
	*fst_TT_entry |= 0x1;

	//2차 page table access permission bit와 매핑할 주소 수정
	snd_page_table_index = (fault_addr >> 12) % 0x100;
	snd_tt_descriptor = (unsigned int *) snd_page_table_base + snd_page_table_index;
	*snd_tt_descriptor = physical_address;
	mask = ~(1u << 9);  // 9번째 비트를 0으로 만들기 위한 마스크
	*snd_tt_descriptor &= mask;
	*snd_tt_descriptor |= (0x3 << 4) | 0x2; //access bit 접근가능하도록 바꿔주기

	//cache 정책 설정: WT
//	*snd_tt_descriptor |= WT; // WT 설정
//	mask = ~(7u << 6);
//	*snd_tt_descriptor &= mask; //tex bit 000

	//cache 정책 설정: WT_WBWA
	*snd_tt_descriptor |= WT_WBWA_PAGE;

	//CoInvalidateMainTlb();
	call_isb();
	page_counter++;
	if (page_counter >= MAX_PA_PAGE)
		swap_flag = 1;
}
