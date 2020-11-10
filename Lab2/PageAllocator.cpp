#include "PageAllocator.h"
#include "Windows.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;


PageAllocator::PageAllocator(size_t size)
{
	NumberOfPages = ceil(size/PageSize);
	size = PageSize * NumberOfPages;
	memory = HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,size);
	void* temp;
	for (size_t i = 0; i < NumberOfPages;i++) {
		temp = (void*)((char*)memory + i*PageSize);
		FreePages.push_back(temp);
		PagesDescribers.insert({ temp, {State::Free,0,NULL,0} });
	}
	for (size_t i = MinBlockSize; i <= PageSize / 2; i *= 2)
	{
		PageFreeBlocks.insert({ i, {} });
	}
}

void* PageAllocator::mem_alloc(size_t size)
{
	if (size < PageSize / 2)
	{
		size_t Class =max((size_t) pow(2, ceil(log2(size))),MinBlockSize);
		if (PageFreeBlocks[Class].size() == 0)
		{
			if (FreePages.empty())
			{
				return NULL;
			}
			else {
				DividePageIntoBlocks(Class, FreePages[0]);
				FreePages.erase(FreePages.begin());
				return AllocateBlock(PageFreeBlocks[Class][0]);
			}
		}
		return AllocateBlock(PageFreeBlocks[Class][0]);
	}
	else {
		size_t NeedPages = ceil(size/PageSize);
		if (NeedPages>FreePages.size())
		{
			return NULL;
		}
		return AllocateMultipageBlock(NeedPages);
	}
}
void PageAllocator::DividePageIntoBlocks(size_t BlockSize, void* FreePage)
{
	PagesDescribers[FreePage].pageState = State::Divided;
	PagesDescribers[FreePage].blockClassSize = BlockSize;
	PagesDescribers[FreePage].freeBlocks = PageSize/BlockSize;
	PagesDescribers[FreePage].firstFreeBlock = FreePage;

	for (int i = 0; i < PagesDescribers[FreePage].freeBlocks - 1; i++)
	{
		void* block = (void*)((char*)FreePage + i * BlockSize);
		auto Header = (Block*)block;
		auto NextBlock = (void*)((char*)FreePage + (i + 1) * BlockSize);
		Header->next = NextBlock;
	}
	PageFreeBlocks[BlockSize].push_back(FreePage);
}

void* PageAllocator::AllocateBlock(void* PageFreeBlock)
{
	auto addr = PagesDescribers[PageFreeBlock].firstFreeBlock;
	size_t BlockSize= PagesDescribers[PageFreeBlock].blockClassSize;
	PagesDescribers[PageFreeBlock].firstFreeBlock = ((Block*)addr)->next;
	PagesDescribers[PageFreeBlock].freeBlocks--;
	if (PagesDescribers[PageFreeBlock].freeBlocks == 0)
	{
		PageFreeBlocks[BlockSize].clear();
	}
	return addr;
}

void* PageAllocator::AllocateMultipageBlock(size_t NeedPages)
{
	void* page = FreePages[0];
	for (size_t i = 0; i < NeedPages; i++)
	{
		void* temp = FreePages[0];
		PagesDescribers[temp].pageState = State::Multipage;
		PagesDescribers[temp].blockClassSize = NeedPages * PageSize;
		PagesDescribers[temp].freeBlocks = NeedPages;
		if (i != NeedPages - 1)
		{
			PagesDescribers[temp].firstFreeBlock = FreePages[1];
		}
		else { PagesDescribers[temp].firstFreeBlock = NULL; }
		FreePages.erase(FreePages.begin());
	}

	return page;
}

void PageAllocator::mem_dump()
{
	cout << "---------------------------------------------------------------------------------------" << endl;
	cout << "Number Of Pages : " << NumberOfPages << endl;
	cout << "Page Size : " << PageSize << endl;
	cout << "Free Pages : " << FreePages.size() << endl;
	for (size_t i = 0; i < NumberOfPages; i++)
	{
		void* page = (void*)((char*)memory + i * PageSize);
		cout << "Page :"<<page;
		if (PagesDescribers[page].pageState == State::Divided)
		{
			cout << ",State: Divided, Block Class Size: "<< PagesDescribers[page].blockClassSize<<" ,First free block: "<< PagesDescribers[page].firstFreeBlock<<" ,Number of free blocks: "<< PagesDescribers[page].freeBlocks<<endl;
		}
		else if (PagesDescribers[page].pageState == State::Free)
		{
			cout << ",State: Free" << endl;
		}
		else if (PagesDescribers[page].pageState == State::Multipage)
		{
			cout << ",State: Multipage, Block Class Size: " << PagesDescribers[page].blockClassSize << " ,Next page of block: " << PagesDescribers[page].firstFreeBlock << endl;
		}
	}
	cout << "---------------------------------------------------------------------------------------" << endl;
}

void PageAllocator::mem_free(void* addr)
{
	void* page;
	for (size_t i = 0; i < NumberOfPages; i++)
	{
		page = (void*)((char*)memory + i * PageSize);
		if (page == addr)
		{
			page = addr;
			break;
		}
	}
	if (PagesDescribers[page].pageState == State::Divided)
	{
		void* temp = PagesDescribers[page].firstFreeBlock;
		PagesDescribers[page].firstFreeBlock = addr;
		((Block*)addr)->next = temp;
		PagesDescribers[page].freeBlocks++;
		if (PagesDescribers[page].freeBlocks == PageSize / PagesDescribers[page].blockClassSize)
		{
			PagesDescribers[page].pageState = State::Free;
			FreePages.push_back(page);
			PageFreeBlocks[PagesDescribers[page].blockClassSize].clear();
		}
	}
	else if (PagesDescribers[page].pageState == State::Multipage)
	{
		for (size_t i = 0; i < PagesDescribers[page].freeBlocks; i++)
		{
			void* temp = (void*)((char*)page + i * PageSize);
			PagesDescribers[temp].pageState = State::Free;
			FreePages.push_back(temp);
		}
	}
}

void* PageAllocator::mem_realloc(void* addr,size_t size)
{
	size_t CurrSize = PagesDescribers[addr].blockClassSize;
	mem_free(addr);
	void* temp = mem_alloc(size);
	if (CurrSize < size)memcpy(temp, addr, CurrSize);
	else memcpy(temp, addr, size);
	return temp;
}