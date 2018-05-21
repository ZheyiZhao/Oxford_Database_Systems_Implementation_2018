
#include <stdio.h>
#include <stdlib.h>

#include "../include/heapfile.h"
#include "../include/scan.h"
#include "../include/heappage.h"
#include "../include/bufmgr.h"



//------------------------------------------------------------------
// Constructor of Scan
//
//------------------------------------------------------------------

Scan::Scan (HeapFile *hf, Status& status)
{
	currDirPid = hf->GetFirstDirPage();
	firstDirPid = currDirPid;
	currEntry = 0;
	
	noMore = FALSE;
	
	MINIBASE_BM->PinPage(currDirPid, (Page *&)dirPage);
	
	PageInfo *info;
	
	info = dirPage->GetPageInfo(currEntry);
	currEntry++;
	
	if (info == NULL)
	{
		noMore = TRUE;
		status = OK;
	}
	else
	{
		currPid = info->pid;
		MINIBASE_BM->PinPage(currPid, (Page *&)page);
		status = page->FirstRecord(currRid);
		if (status != OK)
			return;
	}
}

//------------------------------------------------------------------
// Destructor of Scan
// 
//------------------------------------------------------------------

Scan::~Scan()
{
	// No error checking here. What can we do if something goes
	// wrong ?

	if (page) 
		MINIBASE_BM->UnpinPage(currPid, CLEAN);
	if (dirPage)
		MINIBASE_BM->UnpinPage(currDirPid, CLEAN);
}


//------------------------------------------------------------------
// Scan;;GetNext  
// 
// Input    : recPtr: pointer to the copy of record, you must allocate
//				space for this pointer before calling this function
// Output   : a copy of the record, its length and its RecordID
// Purpose  : to retrieve next record
// Return   : OK if successful, DONE if no more records, FAIL if error 
// Result	: If there are more records left, retrieve the record with
//				rid of currRid, update currRid to the next record.
//------------------------------------------------------------------

Status Scan::GetNext(RecordID& rid, char *recPtr, int& recLen)
{
	Status s;
	
	if (noMore)
	{
		// Take care of empty file
		return DONE;
	}
	
	rid = currRid;
	s = page->GetRecord(rid, recPtr, recLen);
	if (s != OK)
		return FAIL;
	
	// Prepare for next GetNext call..
	
	s = page->NextRecord(currRid, currRid);
	if (s == DONE)
	{
		// No more record on page currPid
		
		PageInfo *info;
		
		UNPIN(currPid, CLEAN);
		page = NULL;
		info = dirPage->GetPageInfo(currEntry);
		currEntry++;
		if (info == NULL)
		{
			// No more record on page currDirPid
			
			PageID next;
			
			next = dirPage->GetNextPage();
			UNPIN(currDirPid, CLEAN);
			dirPage = NULL;
			if (next == INVALID_PAGE)
			{
				// No more record on this file !
				
				noMore = TRUE;
				return OK;
			}
			PIN(next, dirPage);
			currDirPid = next;
			currEntry = 0;
			info = dirPage->GetPageInfo(currEntry);
			currEntry++;
		}
		currPid = info->pid;
		PIN(currPid, page);
		
		s = page->FirstRecord(currRid);
		if (s != OK)
			return FAIL;
	}
	
	return OK;
}

// Scan:: MoveTO
// Input: RecordID of the position you wish to move to
// Output: Status, OK if success
// Result: update currRid to the rid you input,
//		and move to that record. You can call GetNext
//		to get that record.
Status Scan::MoveTo (RecordID rid)
{
	currRid = rid;
	if (currPid != rid.pageNo || noMore == (int)TRUE)
	{	
		// Damn. I forgot to unpin the current one 
		// before I pin the new pages.

		if (page != NULL) 
		{
			UNPIN(currPid, CLEAN);
			page = NULL;
		}

		if (dirPage != NULL)
		{
			UNPIN(currDirPid, CLEAN);
			dirPage = NULL;
		}

		currPid = rid.pageNo;

		DirPageIterator nextDirPage(firstDirPid);
		PageInfo *info;
		
		while ((currDirPid = nextDirPage()) != INVALID_PAGE)
		{
			PIN(currDirPid, dirPage);
			PageInfoIterator nextPageInfo(dirPage);
			currEntry = 0;
			while (info = nextPageInfo())
			{
				currEntry++;
				if (info->pid == currPid) 
					break;
			}
			
			if (info != NULL)
			{
				break;
			}
			
			UNPIN(currDirPid, CLEAN);
			dirPage = NULL;
		} 
		if (info == NULL)
		{
			return FAIL;
		}

		PIN(currPid, page);		
	}
	
	noMore = FALSE;
	return OK;
}
