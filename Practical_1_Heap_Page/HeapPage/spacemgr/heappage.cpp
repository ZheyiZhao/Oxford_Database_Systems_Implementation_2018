#include <iostream>
#include <stdlib.h>
#include <memory.h>

#include "../include/heappage.h"
#include "../include/heapfile.h"
#include "../include/bufmgr.h"
#include "../include/db.h"

using namespace std;

//------------------------------------------------------------------
// Constructor of HeapPage
//
// Input     : Page ID
// Output    : None
//------------------------------------------------------------------

void HeapPage::Init(PageID pageNo)
{
    this->prevPage = INVALID_PAGE;
    this->nextPage = INVALID_PAGE;
    this->pid = pageNo; // page number
    fillPtr = sizeof(Slot); //data area offset
    freeSpace = HEAPPAGE_DATA_SIZE - sizeof(Slot); //minus preserved slot space
}

void HeapPage::SetNextPage(PageID pageNo)
{
	this->nextPage = pageNo;
}

void HeapPage::SetPrevPage(PageID pageNo)
{
	this->prevPage = pageNo;
}

PageID HeapPage::GetNextPage()
{
    return this->nextPage;
}

PageID HeapPage::GetPrevPage()
{
    return this->prevPage;

}


//------------------------------------------------------------------
// HeapPage::InsertRecord
//
// Input     : Pointer to the record and the record's length 
// Output    : Record ID of the record inserted.
// Purpose   : Insert a record into the page
// Return    : OK if everything went OK, DONE if sufficient space 
//             does not exist
//------------------------------------------------------------------

Status HeapPage::InsertRecord(char *recPtr, int length, RecordID& rid)
{
    //not sufficient space
    if (length > this->AvailableSpace())
        return DONE;
    //sufficient space
    this->freeSpace -= length;
    for (int i = 0;  i< length; i++)
        this->data[fillPtr+i] = recPtr[i];
    
    int slotNum = (sizeof(slots)/sizeof(Slot)) + 1;
    this->slots[slotNum].offset = fillPtr;
    this->slots[slotNum].length = length;
    
    this->fillPtr += length;
    
    rid.pageNo = this->pid;
    rid.slotNo = slotNum;
    
    numOfSlots++;
    return OK;
}


//------------------------------------------------------------------
// HeapPage::DeleteRecord 
//
// Input    : Record ID
// Output   : None
// Purpose  : Delete a record from the page
// Return   : OK if successful, FAIL otherwise  
//------------------------------------------------------------------ 

Status HeapPage::DeleteRecord(const RecordID& rid)
{
    int slotSize = (sizeof(slots)/sizeof(Slot));
    if (rid.slotNo > slotSize || rid.slotNo < 0) //invalid slot
        return FAIL;
    if (this->slots[rid.slotNo].offset == -1) //empty slot
        return FAIL;
    this->slots[rid.slotNo].offset = -1;
    numOfSlots--;
    return OK;
}


//------------------------------------------------------------------
// HeapPage::FirstRecord
//
// Input    : None
// Output   : record id of the first record on a page
// Purpose  : To find the first record on a page
// Return   : OK if successful, DONE otherwise
//------------------------------------------------------------------

Status HeapPage::FirstRecord(RecordID& rid)
{
    int slotSize = (sizeof(slots)/sizeof(Slot));
    for (int i = 0 ; i <= slotSize ; i++)
        if (this->slots[i].offset != -1){
            rid.pageNo = this->pid;
            rid.slotNo = i;
            return OK;
        }
    return DONE;
}


//------------------------------------------------------------------
// HeapPage::NextRecord
//
// Input    : ID of the current record
// Output   : ID of the next record
// Return   : Return DONE if no more records exist on the page; 
//            otherwise OK
//------------------------------------------------------------------

Status HeapPage::NextRecord (RecordID curRid, RecordID& nextRid)
{
    int curSlot = curRid.slotNo;
    int slotSize = (sizeof(slots)/sizeof(Slot));
    if (curSlot > slotSize || curSlot < 0)
        return FAIL;
    for (int i = curSlot + 1 ; i <= slotSize ; i++)
        if (this->slots[i].offset != -1){
            nextRid.pageNo = this->pid;
            nextRid.slotNo = i;
            return OK;
        }
    return DONE;
}


//------------------------------------------------------------------
// HeapPage::GetRecord
//
// Input    : Record ID
// Output   : Records length and a copy of the record itself
// Purpose  : To retrieve a _copy_ of a record with ID rid from a page
// Return   : OK if successful, FAIL otherwise
//------------------------------------------------------------------

Status HeapPage::GetRecord(RecordID rid, char *recPtr, int& length)
{
    int slotSize = (sizeof(slots)/sizeof(Slot));
    if (rid.slotNo > slotSize || rid.slotNo < 0)
        return FAIL;
    int index = slots[rid.slotNo].offset;
    if (index == -1)
        return FAIL;
    int len = slots[rid.slotNo].length;
    for (int i = 0; i < len; i++){
        recPtr[i] = data[index+i];
    }
    length = len;
    return OK;
}


//------------------------------------------------------------------
// HeapPage::ReturnRecord
//
// Input    : Record ID
// Output   : pointer to the record, record's length
// Purpose  : To output a _pointer_ to the record
// Return   : OK if successful, FAIL otherwise
//------------------------------------------------------------------

Status HeapPage::ReturnRecord(RecordID rid, char*& recPtr, int& length)
{
    int slotSize = (sizeof(slots)/sizeof(Slot));
    if (rid.slotNo > slotSize || rid.slotNo < 0)
        return FAIL;
    int index = slots[rid.slotNo].offset;
    if (index == -1)
        return FAIL;
    int len = slots[rid.slotNo].length;
    
    length = len;
    recPtr = &data[index];
    return OK;
}


//------------------------------------------------------------------
// HeapPage::AvailableSpace
//
// Input    : None
// Output   : None
// Purpose  : To return the amount of available space
// Return   : The amount of available space on the heap file page.
//------------------------------------------------------------------

int HeapPage::AvailableSpace(void)
{
    return freeSpace - sizeof(Slot);

}


//------------------------------------------------------------------
// HeapPage::IsEmpty
// 
// Input    : None
// Output   : None
// Purpose  : Check if there is any record in the page.
// Return   : true if the HeapPage is empty, and false otherwise.
//------------------------------------------------------------------

bool HeapPage::IsEmpty(void)
{
    if (this->numOfSlots == 0)
        return TRUE;
    else return FALSE;
}


void HeapPage::CompactSlotDir()
{
  // Complete this method to get the S+ mark.
  // This method is not required for an S mark.
}

int HeapPage::GetNumOfRecords()
{
    int slotSize = (sizeof(slots)/sizeof(Slot));
    int cnt = 0;
    for (int i =0 ; i <= slotSize; i++)
        if (slots[i].offset != -1)
            cnt++;
    
    return cnt;
}
