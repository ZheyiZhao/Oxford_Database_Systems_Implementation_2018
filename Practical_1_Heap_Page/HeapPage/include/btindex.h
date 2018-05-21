#ifndef BTINDEX_PAGE_H
#define BTINDEX_PAGE_H


#include "minirel.h"
#include "page.h"
#include "sortedpage.h"
#include "bt.h"



class BTIndexPage : public SortedPage {
	
private:

	// No private variables should be declared.

public:
	
	Status Insert (const void *key, AttrType keyType, PageID pageNo, RecordID& rid);
	Status Delete (const void *key, AttrType keyType, RecordID& curRid);
	Status GetPageID (const void *key, AttrType keyType, PageID & pageNo);
	Status GetSibling(const void *key, AttrType keyType, PageID & pageNo, int &left);
	Status GetFirst (RecordID& rid, void *key, PageID & pageNo);
	Status GetNext (RecordID& rid, void *key, PageID & pageNo);
	
	PageID GetLeftLink (void);
	void   SetLeftLink (PageID left);
	    
	Status FindKey (void *key, void *entry, AttrType key_type);
	Status AdjustKey (const void *newKey, const void *oldKey, AttrType keyType);
	Status Redistribute(BTIndexPage *pptr, BTIndexPage *parentPtr,
					  AttrType keyType, int left, const void *deletedKey);

};

#endif
