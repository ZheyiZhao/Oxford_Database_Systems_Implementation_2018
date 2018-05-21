#ifndef BTLEAF_PAGE_H
#define BTLEAF_PAGE_H

#include "minirel.h"
#include "page.h"
#include "sortedpage.h"
#include "bt.h"
#include "btindex.h"


class BTLeafPage : public SortedPage {
	
private:
	
	// No private variables should be declared.
	
public:
		
	Status Insert (const void *key, AttrType key_type, RecordID dataRid, RecordID& rid);
	
	Status GetFirst (RecordID& rid, void *key, RecordID & dataRid);
	Status GetNext  (RecordID& rid, void *key, RecordID & dataRid);
	Status GetCurrent (RecordID rid, void *key, RecordID & dataRid);
	
	Status Delete (const void *key, AttrType key_type, const RecordID& dataRid);
	Status Redistribute (BTLeafPage *pptr, BTIndexPage *parentPtr, 
						 AttrType keyType, int left, const void *deletedKey);	
};

#endif
