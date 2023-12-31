#define MAX_PA_PAGE 256 // 메인 메모리 용량

struct PA_page_info
{
	unsigned int is_allocated;
	unsigned int asid;
	unsigned int virtual_address;
};

extern struct PA_page_info PA_page_info_list[MAX_PA_PAGE];
extern unsigned int page_counter;
extern unsigned int swap_counter;

extern void demand_paging(unsigned int fault_addr);
