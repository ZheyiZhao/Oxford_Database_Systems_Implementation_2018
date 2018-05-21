#include <string.h>

#include "../include/bufmgr.h"
#include "../include/heappage.h"
#include "../include/dirpage.h"

Status DirPage::Init(PageID pid)
{
	numOfEntry = 0;
	curr = pid;
	next = INVALID_PAGE;
	prev = INVALID_PAGE;

	return OK;
}


Status DirPage::InsertPage(PageID pid, HeapPage *page)
{
	PageInfo info;

	// ASSERT : this page has enough space and page is a NEW page.
	//          (No record inside)

	info.pid = pid;
	info.spaceAvailable = page->AvailableSpace();
	info.numOfRecords = 0;

	
	memcpy(&data[numOfEntry*sizeof(PageInfo)], &info, sizeof(PageInfo));
 	numOfEntry++;
	return OK;
}


Status DirPage::DeletePage(PageID pid)
{
	int toDelete;

	toDelete = FindPageInfoEntry(pid);
	if (toDelete == 0)
		return FAIL;
	else
		memmove(&data[toDelete*sizeof(PageInfo)], 
			&data[(toDelete+1)*sizeof(PageInfo)], 
			(numOfEntry - toDelete)*sizeof(PageInfo));

	numOfEntry--;
	return OK;

}


PageInfo *DirPage::FindPageInfo(PageID pid)
{
	PageInfo *info;

	info = (PageInfo *)&data;
	for (int i = 0; i < numOfEntry; i++)
	{
		info = (PageInfo *)&data[i*sizeof(PageInfo)];
		if (info->pid == pid)
			return info;
	}
	return NULL;
}


Bool DirPage::HasFreeSpace()
{
	return (numOfEntry < DIR_PAGE_SIZE/sizeof(PageInfo));
}


Status DirPage::DeleteItSelf()
{
	if (next != INVALID_PAGE)
	{
		DirPage *nextPage;

		PIN(next, nextPage);
		nextPage->SetPrevPage(prev);
		UNPIN(next, DIRTY);
	}

	if (prev != INVALID_PAGE)
	{
		DirPage *prevPage;

		PIN(prev, prevPage);
		prevPage->SetNextPage(next);
		UNPIN(prev, DIRTY);
	}

	return OK;
}


int DirPage::FindPageInfoEntry(PageID pid)
{
	PageInfo *info;

	info = (PageInfo *)&data;
	for (int i = 0; i < numOfEntry; i++)
	{
		info = (PageInfo *)&data[i*sizeof(PageInfo)];
		if (info->pid == pid)
			return i;
	}
	return -1;
}


PageInfo *DirPage::GetPageInfo(int entry)
{
	if (entry < numOfEntry)
		return (PageInfo *)&data[entry*sizeof(PageInfo)];
	else
		return NULL;
}


Status DirPage::InsertRecordIntoPage (PageID pid, HeapPage *page)
{
	PageInfo *info;

	info = FindPageInfo(pid);
	if (info == NULL)
		return FAIL;
	else
	{
		info->numOfRecords++;
		info->spaceAvailable = page->AvailableSpace();
		return OK;
	}
}


Status DirPage::DeleteRecordFromPage (PageID pid, HeapPage *page)
{
	PageInfo *info;

	info = FindPageInfo(pid);
	if (info == NULL)
		return FAIL;
	else
	{
		info->numOfRecords--;
		info->spaceAvailable = page->AvailableSpace();
		return OK;
	}
}

PageID DirPage::GetNextPage()
{
	return next;
}

PageInfoIterator::PageInfoIterator(DirPage *dirPage)
{
	page = dirPage;
	currEntry = 0;
}


PageInfoIterator::~PageInfoIterator()
{
	
}


PageInfo *PageInfoIterator::operator() ()
{
	PageInfo *info;

	info = page->GetPageInfo(currEntry);
	currEntry++;

	return info;
}


DirPageIterator::DirPageIterator(PageID pid)
{
	curr = pid;
}


DirPageIterator::~DirPageIterator()
{
	
}


PageID DirPageIterator::operator() ()
{
	DirPage *page;
	PageID next;
	PageID toReturn;

	toReturn = curr;

	if (curr != INVALID_PAGE)
	{
		MINIBASE_BM->PinPage(curr, (Page *&)page);
		next = page->next;
		MINIBASE_BM->UnpinPage(curr);
		curr = next;
	}

	return toReturn;
}
