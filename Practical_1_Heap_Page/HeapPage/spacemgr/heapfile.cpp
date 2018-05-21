#include <stdio.h>

#include "../include/heapfile.h"
#include "../include/heappage.h"
#include "../include/dirpage.h"
#include "../include/scan.h"
#include "../include/bufmgr.h"
#include "../include/db.h"

static int breakpoint()
{
	return 1;
}


//-----------------------------------------------------------------------
//  Constructor for HeapFile
//  
//  Input   : name - name of a Heap File
//	Output  : status of initialization
//  Purpose : If the heapfile already exists in the database, get the 
//            first page. If the heapfile does not yet exist, create it, 
//            get the first page.
//  Note    : You can use MINIBASE_DB->GetFileEntry() to test if the
//            file exists or not.
//-----------------------------------------------------------------------

HeapFile::HeapFile( const char *name, Status& returnStatus )
{
	DirPage *page;
	Status s;
	
	if (name == NULL)
	{
		// Create a temporary heap file
		filename = tmpnam(NULL);
		type = TEMPORARY;
		
		s = MINIBASE_BM->NewPage(dirPid, (Page *&)page);
		if (s != OK)
		{
			cerr << "Error creating new file.\n" << endl;
			returnStatus = FAIL;
			return;
		}		
	}
	else if (MINIBASE_DB->GetFileEntry(name, dirPid) != OK)
	{
		// This HeapFile does not exists, yet.  I have to create
		// it.  Since name is not NULL, this is a permenant file.
		
		filename = strcpy((char *)malloc(strlen(name)+1), name);
		type = PERMENANT;
		
		// Create a new DirPage.
		
		s = MINIBASE_BM->NewPage(dirPid, (Page *&)page);
		if (s != OK)
		{
			cerr << "Error creating new file.\n" << endl;
			returnStatus = FAIL;
			return;
		}
		
		// Add the file entry.
		
		MINIBASE_DB->AddFileEntry(name, dirPid);
	}
	else
	{
		// The HeapFile alraedy exists.  Initlized the lastDirPid page id.
		
		filename = strcpy((char *)malloc(strlen(name)+1), name);
		type = PERMENANT;

		DirPage *page;

		s = MINIBASE_BM->PinPage(dirPid, (Page *&)page);
		if (s != OK)
		{
			cerr << "HeapFile::HeapFile - Error pinning the directories\n";
			returnStatus = FAIL;
			return;
		}

		PageID currPid;
		PageID prevPid = dirPid;
		while ((currPid = page->GetNextPage()) != INVALID_PAGE)
		{
			s = MINIBASE_BM->UnpinPage(prevPid);
			if (s != OK)
			{
				cerr << "HeapFile::HeapFile - Error unpinning the directories\n";
				returnStatus = FAIL;
				return;
			}
			s = MINIBASE_BM->PinPage(currPid, (Page *&)page);
			if (s != OK)
			{
				cerr << "HeapFile::HeapFile - Error pinning the directories\n";
				returnStatus = FAIL;
				return;
			}
			prevPid = currPid;
		}

		lastDirPid = prevPid;
		s = MINIBASE_BM->UnpinPage(prevPid);
		if (s != OK)
		{
			cerr << "Error unpinning the directories\n";
			returnStatus = FAIL;
			return;
		}

		returnStatus = OK;
		return;
	}

	// We reach this point if we are create a new (temp or permenant)
	// HeapFile.

	page->Init (dirPid);
	
	// Initilized the doubly link pointer of the directory page.
	
	page->SetNextPage (INVALID_PAGE);
	page->SetPrevPage (INVALID_PAGE);
	
	lastDirPid = dirPid;

	s = MINIBASE_BM->UnpinPage(dirPid, DIRTY);
	if (s != OK)
	{
		cerr << "Error unpinning the directory." << endl;
		returnStatus = FAIL;
		return;
	}
	
	returnStatus = OK;
}


//-----------------------------------------------------------------------
// Destructor for Heap File
// 
// Input    : none
// Output   : none
// PreCond  : No pages are pinned and private members of class Heapfile 
//            are valid.
//-----------------------------------------------------------------------

HeapFile::~HeapFile()
{
	if (type == TEMPORARY)
		DeleteFile();
}


//-----------------------------------------------------------------------
// HeapFile::DeleteFile
//
// Input     : None
// Output    : None
// Purpose   ; Deletes the heapfile from the database permanently
// Condition : Heap file exists 
// PostCond  : Heap file is deleted
// Return    : OK if operation is successful, FAIL otherwise
// -----------------------------------------------------------------------

Status HeapFile::DeleteFile()
{
	DirPageIterator nextDirPage(dirPid);
	DirPage *dirPage;
	PageID currDirPid;
	PageInfo *info;

	while ((currDirPid = nextDirPage()) != INVALID_PAGE)
	{
		PIN(currDirPid, dirPage);
		PageInfoIterator nextPageInfo(dirPage);

		while (info = nextPageInfo())
		{
			FREEPAGE(info->pid);
		}

		UNPIN(currDirPid, CLEAN);
		FREEPAGE(currDirPid);
	} 

	if (type == PERMENANT)
		MINIBASE_DB->DeleteFileEntry(filename);

	return OK;
}


//-----------------------------------------------------------------------
// HeapFile::GetNumOfRecords
// 
// Input     : None
// Output    : None
// Purpose   : Determine the number of records
// Return    : Number of records
//-----------------------------------------------------------------------

int HeapFile::GetNumOfRecords()
{
	DirPageIterator nextDirPage(dirPid);
	DirPage *dirPage;
	PageID currDirPid;
	PageInfo *info;
	int sum = 0;

	while ((currDirPid = nextDirPage()) != INVALID_PAGE)
	{
		PIN(currDirPid, dirPage);
		PageInfoIterator nextPageInfo(dirPage);
		while (info = nextPageInfo())
		{
			sum += info->numOfRecords;
		}
		UNPIN(currDirPid, CLEAN);
	} 

    return sum;
}


//-----------------------------------------------------------------------
// HeapFile::InsertRecord
//
// Input     : pointer to the record
//             record length
// Output    : record ID
// Purpose   : Insert a record into the Heap File
// Condition : HeapFile must exist
// PostCond  : The record is inserted into a page with enough space, 
//           : if such page is not found, a new page is added to the 
//             HeapFile
// Return    : OK if operation is successful, FAIL otherwise
//-----------------------------------------------------------------------  
            
Status HeapFile::InsertRecord(char *recPtr, int recLen, RecordID& outRid)
{
	DirPage *dirPage;
	PageID   currDirPid;
	PageID   pid;

	if (recLen >= MAX_SPACE)
	{
		cerr << " Attempting to insert records that is larger than size of a page" << endl;
		return FAIL;
	}

	DirPageIterator  nextDirPage(dirPid);
	PageInfo *info;

	currDirPid = dirPid;
	do 
	{
		PIN(currDirPid, dirPage);
		
		PageInfoIterator nextPageInfo(dirPage);
		while (info = nextPageInfo())
		{
			if (info->spaceAvailable > (short)recLen) 
				break;
		}

		if (info != NULL)
			break;

		UNPIN(currDirPid, CLEAN);

	} while ((currDirPid = nextDirPage()) != INVALID_PAGE);

	if (info == NULL)
	{
		// No data page can accomodate this record.
		// Create a new data page pid, whose dirPageRecord
		// resides on currDirPid

		NewPage(pid, currDirPid);
		PIN(currDirPid, dirPage);
	}
	else
	{
		pid = info->pid;
	}

	HeapPage *page;
	// Insert into this page.

	PIN(pid, page);
	page->InsertRecord(recPtr, recLen, outRid);
	dirPage->InsertRecordIntoPage(pid, page);
	
	UNPIN(pid, DIRTY);
	UNPIN(currDirPid, DIRTY);


	return OK;
}


//-----------------------------------------------------------------------
// HeapFile::GetRecord
//
// Input    : Record ID
// Output   : A copy of the record, record's length
// Purpose  : Reading record from the file
// Condition: HeapFile exists
// Return   : OK if record is found, DONE if record is not found 
//          : and FAIL otherwise  
//-----------------------------------------------------------------------

Status HeapFile::GetRecord (const RecordID& rid, char *recPtr, int& recLen)
{
	HeapPage *page;

	PIN(rid.pageNo, page);
	page->GetRecord(rid, recPtr, recLen);
	UNPIN(rid.pageNo, CLEAN);

	return OK;
}


//-----------------------------------------------------------------------
// HeapFile::DeleteRecord
//
// Input    : Record ID
// Output   : None
// Purpose  : Delete a record from the file, if it was the only
//          : record on the page, remove the page as well
// Condition: Heap File has to exist
// PostCond : if the record exists it is deleted, if the page becomes 
//            empty it is deleted as well
// Return   : OK if the record is deleted, DONE if the record was not found,
//            FAIL otherwise
//-----------------------------------------------------------------------

Status HeapFile::DeleteRecord (const RecordID& rid)
{
	DirPageIterator nextDirPage(dirPid);
	DirPage *dirPage;
	PageID currDirPid;
	PageInfo *info;

	while ((currDirPid = nextDirPage()) != INVALID_PAGE)
	{
		PIN(currDirPid, dirPage);
		PageInfoIterator nextPageInfo(dirPage);
		
		while (info = nextPageInfo())
		{
			if (info->pid == rid.pageNo) 
				break;
		}

		if (info != NULL)
		{
			break;
		}

		UNPIN(currDirPid, CLEAN);
	} 

	if (currDirPid == INVALID_PAGE)
	{
		return DONE;
	}
	else
	{
		// Delete the record.
		// First from the data page.
		HeapPage *page;

		PIN(info->pid, page);
		page->DeleteRecord(rid);

		// Then update the PageInfo. ARRGGGH ! must update
		// this everytime we change a page.

		dirPage->DeleteRecordFromPage(info->pid, page);

		if (page->IsEmpty())
		{
			// If HeapPage is now empty, we have to deallocate it.
			
			FREEPAGE(info->pid);
			dirPage->DeletePage(info->pid);
			if (dirPage->IsEmpty())
			{
				// If DirPage is empty, we have to deallocate it
				// too, unless it's the one and only dirPage around.

				if (dirPage->Deletable())
				{
					// First unattach itself from the link list.

					dirPage->DeleteItSelf();
					if (dirPage->IsHead())
					{
						// If this dirPage is the first page, we have
						// to change dirPid.  Since it's deletable, the
						// next pid must be valid.

						dirPid = dirPage->GetNextPage();
					}
					FREEPAGE(currDirPid);
				}
				else
				{
					UNPIN(currDirPid, DIRTY);
				}
			}
			else
			{
				UNPIN(currDirPid, DIRTY);
			}
		}
		else
		{
			UNPIN(info->pid, DIRTY);
			UNPIN(currDirPid, DIRTY);
		}
	}

	return OK;
}


//-----------------------------------------------------------------------
// HeapFile::UpdateRecord 
// 
// Input    : Record ID, pointer to a record and its length 
// Output   : none
// Purpose  : Find a record and update it
// PostCond : The record is updated and the heap file is updated
// Return   : OK if the record is updated, DONE if it was not found, 
//            FAIL otherwise
//-----------------------------------------------------------------------

Status HeapFile::UpdateRecord (const RecordID& rid, char *recPtr, int recLen)
{ 
	DirPageIterator nextDirPage(dirPid);
	DirPage *dirPage;
	PageID currDirPid;
	PageInfo *info;

	while ((currDirPid = nextDirPage()) != INVALID_PAGE)
	{
		PIN(currDirPid, dirPage);
		PageInfoIterator nextPageInfo(dirPage);
		while (info = nextPageInfo())
		{
			if (info->pid == rid.pageNo) 
			{
				UNPIN(currDirPid, CLEAN);
				break;
			}
		}

		if (info != NULL)
			break;
		UNPIN(currDirPid, CLEAN);
	} 

	if (currDirPid == INVALID_PAGE)
	{
		return DONE;
	}
	else
	{
		HeapPage *page;
		char *oldPtr;
		int  oldLen;

		PIN(info->pid, page);
		page->ReturnRecord(rid, oldPtr, oldLen);
		
		if (oldLen != recLen)
		{
			cerr << " Unable to update records of different length." << endl;
			return FAIL;
		}

		memcpy(oldPtr, recPtr, oldLen);	
		UNPIN(info->pid, DIRTY);
	}
          
	return OK;
}


//-----------------------------------------------------------------------
// HeapFile::OpenScan
// 
// Purpose  : Initiate a sequential scan
//-----------------------------------------------------------------------

Scan *HeapFile::OpenScan(Status& status)
{
	Scan *newScan;
	
	newScan = new Scan(this, status);
	
	if (status == OK)
	    return newScan;
	else 
	{
	    delete newScan;
	    return NULL;
	}
}


PageID HeapFile::NextPage(PageID pid)
{
	HeapPage *page;
	PageID next;

	PIN (pid, page);
	next = page->GetNextPage();
	UNPIN (pid, CLEAN);
	return next;
}


Status HeapFile::NewPage(PageID &pid, PageID &currDirPid)
{

	DirPageIterator nextDirPage(dirPid);
	DirPage *dirPage;
	HeapPage *newDataPage;

	while ((currDirPid = nextDirPage()) != INVALID_PAGE)
	{
		PIN(currDirPid, dirPage);
		if (dirPage->HasFreeSpace())
		{
			break;
		}
		UNPIN(currDirPid, CLEAN);
	}

	if (currDirPid == INVALID_PAGE)
	{
		// Directory Pages are full. Create new one.
		// Insert at the end of the dirPage list

		NEWPAGE (currDirPid, dirPage);
		dirPage->Init(currDirPid);
		dirPage->SetNextPage(INVALID_PAGE);
		dirPage->SetPrevPage(lastDirPid);

		DirPage *lastPage;
		
		PIN(lastDirPid, lastPage);
		lastPage->SetNextPage(currDirPid);
		UNPIN(lastDirPid, DIRTY);

		lastDirPid = currDirPid;
	}
	
	NEWPAGE (pid, newDataPage);
	
	newDataPage->Init(pid);

	// Create a new page
	dirPage->InsertPage(pid, newDataPage);

	UNPIN(pid, DIRTY);
	UNPIN(currDirPid, DIRTY);

	return OK;
}

