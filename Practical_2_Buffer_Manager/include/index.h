#ifndef _INDEX_H
#define _INDEX_H

#include "minirel.h"

class IndexFileScan;

class IndexFile {

	friend class IndexFileScan;
	
public:

    virtual ~IndexFile() {}; 
	
    virtual Status Insert (const void* data, const RecordID rid) = 0;
    virtual Status Delete (const void* data, const RecordID rid) = 0;
	
};


class IndexFileScan {

public:

	virtual ~IndexFileScan() {} 
	
	virtual Status GetNext (RecordID &rid, void* keyptr) = 0;
	virtual Status DeleteCurrent () = 0;
	
	virtual int KeySize() = 0;
	
private:
	
};


#endif

