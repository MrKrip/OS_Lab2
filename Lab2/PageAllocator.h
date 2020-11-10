#pragma once
#include <iostream>
#include <map>
#include <vector>

#define PageSize 4096
#define MinBlockSize 16

using namespace std;


class PageAllocator {
private:
	enum class State {
		Free,
		Divided,
		Multipage
	};

	struct PageDescriber {
		State pageState;
		size_t blockClassSize;
		void* firstFreeBlock;
		size_t freeBlocks;
	};

	struct Block {
		void* next;
	};

	vector <void*> FreePages;
	map <void*, PageDescriber> PagesDescribers;
	map <size_t, vector <void*>> PageFreeBlocks;
	size_t NumberOfPages;
	void* memory;
	void DividePageIntoBlocks(size_t BlockSize, void* FreePage);
	void* AllocateBlock(void* FreeClassBlock);
	void* AllocateMultipageBlock(size_t NeedPages);
public:
	PageAllocator(size_t size);

	void* mem_alloc(size_t size);
	void mem_free(void* addr);
	void* mem_realloc(void* addr, size_t size);
	void mem_dump();
	
};
