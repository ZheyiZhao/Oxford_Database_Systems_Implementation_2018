#ifndef HFPAGE_H
#define HFPAGE_H

#include "minirel.h"
#include "page.h"

const int INVALID_SLOT =  -1;

//
// CHANGE this constant whenever you update the structure of HeapPage class.
//
const int HEAPPAGE_DATA_SIZE=(MAX_SPACE - 3*sizeof(PageID) - 6*sizeof(short));

class HeapPage {

protected :

	struct Slot 
	{
		short offset; // offset of record from the start of dataarea.
		short length; // length of the record.
	};


	short   numOfSlots;  // Number of slots available (maybe filled or
	                     // empty.
	short   fillPtr;     // Offset from start of data area, where 
	                     // the records resides.
	short   freeSpace;   // Amount of free space in bytes in this page.
	
	short   type;        // Not used for HeapFile assignment, but will 
	                     // be used in B+-tree assignment.

	PageID  pid;         // Page ID of this page  
	PageID  nextPage;    // Page ID of the next page in a link list.
	PageID  prevPage;    // Page ID of the prev page in a link list.

	Slot    slots[1];    // Slots for the page.  May grow towards
	                     // the end of a page.  (May overflow into
                       // the data area.)

	char data[HEAPPAGE_DATA_SIZE];

	                     // Data area for this page.  Actual records
			     // grows from the back towards to start of 
			     // a page. 

	void CompactSlotDir();

public:

	void Init(PageID pageNo);

	PageID GetNextPage();
	PageID GetPrevPage();
	PageID PageNo() {return pid;}   
	void   SetNextPage(PageID pageNo);
	void   SetPrevPage(PageID pageNo);
	Status InsertRecord(char* recPtr, int recLen, RecordID& rid);
	Status DeleteRecord(const RecordID& rid);
	Status FirstRecord(RecordID& firstRid);
	Status NextRecord (RecordID curRid, RecordID& nextRid);
	Status GetRecord(RecordID rid, char* recPtr, int& recLen);
	Status ReturnRecord(RecordID rid, char*& recPtr, int& recLen);
	Status ReturnOffset(RecordID rid, int& offset);
	int    AvailableSpace(void);
	bool   IsEmpty(void);
	int    GetNumOfRecords();
};

#define SLOT_IS_EMPTY(s)  ((s).length == INVALID_SLOT)
#define SLOT_FILL(s, o, l) do { (s).offset = (o); (s).length = (l);} while (0)
#define SLOT_SET_EMPTY(s)  (s).length = INVALID_SLOT

#define PIN(a, b)   if (MINIBASE_BM->PinPage((a), (Page *&)(b)) != OK) {\
						cerr << "Unable to pin page " << a << endl; return FAIL;}
#define UNPIN(a, b) if (MINIBASE_BM->UnpinPage((a), (b)) != OK) {\
						cerr << "Unable to unpin page " << a << endl; return FAIL;}
#define FREEPAGE(a) if (MINIBASE_BM->FreePage((a)) != OK) {\
						cerr << "Unable to free page " << a << endl; return FAIL;}
#define NEWPAGE(a, b)  if (MINIBASE_BM->NewPage((a), (Page *&)(b)) != OK) {\
						cerr << "Unable to allocate new page " << a << endl; return FAIL;}

#define DIRTY TRUE
#define CLEAN FALSE


#endif
