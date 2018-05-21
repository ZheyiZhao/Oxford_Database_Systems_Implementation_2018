#ifndef SORTED_PAGE_H
#define SORTED_PAGE_H


#include "minirel.h"
#include "page.h"
#include "heappage.h"
#include "bt.h"


class SortedPage : public HeapPage {
	
private:
	
	// No private variables should be declared.
	
public:
		
	Status InsertRecord(AttrType keyType, char * recPtr, int recLen, RecordID& rid);	
	Status DeleteRecord(const RecordID& rid);
	
	void  SetType(short t)  { type = t; }
	short GetType()         { return type; }
	int   GetNumOfRecords() { return numOfSlots; }
};

#endif
