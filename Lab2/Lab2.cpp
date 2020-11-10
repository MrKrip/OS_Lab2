#include <iostream>
#include "Windows.h"
#include <map>
#include <vector>
#include "PageAllocator.h"

using namespace std;

int main()
{
	PageAllocator aloc = PageAllocator(10*PageSize);
	aloc.mem_dump();
	void* a1 =aloc.mem_alloc(200);
	void* a2 = aloc.mem_alloc(300);
	void* a3 = aloc.mem_alloc(200);
	void* a4 = aloc.mem_alloc(200);
	void* a5 = aloc.mem_alloc(3*PageSize);
	aloc.mem_dump();
	aloc.mem_free(a1);
	aloc.mem_free(a5);
	aloc.mem_dump();
	void* a6 = aloc.mem_realloc(a2,4*PageSize);
	aloc.mem_dump();
}
