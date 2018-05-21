#ifndef _BTREE_FILESCAN_H
#define _BTREE_FILESCAN_H

#include "btfile.h"

class BTreeFile;

class BTreeFileScan : public IndexFileScan {
	
public:
	
	friend class BTreeFile;

    Status GetNext (RecordID & rid, void* keyptr);
    Status DeleteCurrent ();

	~BTreeFileScan();
	
	int KeySize();

private:

    BTreeFile *treep;           // B+ tree we're scanning 

    BTLeafPage *leafp;          // leaf page containing current record
    RecordID curRid;                 // position in current leaf; note: this is 
                                // the RID of the key/RID pair within the
                                // leaf page.

    Bool didfirst;              // false only before get_next is called
    Bool deletedcurrent;        // true after delete_current is called (read
                                // by get_next, written by delete_current).
    
    const void *endkey;         // if 0, then go all the way right
                                // else, stop when current record > this value.
                                // (that is, implement an inclusive range 
                                // scan -- the only way to do a search for 
                                // a single value).
};

#endif