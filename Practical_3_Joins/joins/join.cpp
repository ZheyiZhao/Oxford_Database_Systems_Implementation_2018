#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/minirel.h"
#include "../include/heapfile.h"
#include "../include/scan.h"
#include "../include/btfile.h"
#include "../include/btfilescan.h"
#include "../include/join.h"

//-----------------------------------------------------------------
// MakeNewRecord
// 
// Purpose : Create a new record from two matching records.
// Input   : r, s - pointers to two records which satisfy` the
//                  join condition.
//           recLenR, recLenS - length of record r and s.
//           newRecord - pointer to a location where new record
//                       is going to be created. allocate space before calling.
// Precond : newRecord is malloc and has at least recLenR + recLenS 
//           bytes
//-----------------------------------------------------------------

void MakeNewRecord (char *newRecord, char *r, char *s, int recLenR, int recLenS)
{
	memcpy(newRecord, r, recLenR);
	memcpy(newRecord + recLenR, s, recLenS);
}


//--------------------------------------------------------------------
// This is a USEFUL function to sort a file.    It is also very useful
// to understand how to use HeapFile, Scan, BTreeFile, BTreeFileScan.
//--------------------------------------------------------------------
// SortFile
// 
// Purpose  : sort a relation ordered by an integer attribute
// Input    : S - pointer to the relation/HeapFile to be sorted.
//            len - length of the record in the file S. (assume fixed
//				    size.
//            offset - offset of the attribute from the beginning of the record.
// Cheat    : We create a B+-Tree using that attribute as the key. Then
//            we scan the B+-Tree and insert the records into a new
//            HeapFile.   The HeapFile guarantees that the order of 
//            insertion will be the same as the order of scan later.
// Return   : The new sorted relation/HeapFile.
//-------------------------------------------------------------------- 

HeapFile *SortFile(HeapFile *S, int len, int offset)
{
	Status s;

	Scan *scan;
	scan = S->OpenScan(s);
	if (s != OK)
	{
		cerr << "ERROR : cannot open scan on the heapfile to sort.\n";
	}

	//
	// Scan the HeapFile S, new a B+Tree and insert the records into B+Tree.
	// 

	BTreeFile *btree;
	btree = new BTreeFile (s, "BTree", ATTR_INT, sizeof(int));

	char *recPtr = new char[len];
	int recLen = len;
	RecordID rid;
	while (scan->GetNext(rid, recPtr, recLen) == OK)
	{
		btree->Insert(recPtr + offset, rid);
	}
	delete scan;

	HeapFile *sorted;
	sorted = new HeapFile(NULL, s); // create a temp HeapFile
	if (s != OK)
	{
	    	cerr << "Cannot create new file for sortedS\n";
	}

	//
	// Now scan the B+-Tree and insert the records into a 
	// new (sorted) HeapFile.
	//

	BTreeFileScan  *btreeScan;
	btreeScan = (BTreeFileScan *)btree->OpenScan(NULL, NULL);

	int key;

	while (btreeScan->GetNext(rid, &key) == OK)
	{
	    S->GetRecord (rid, recPtr, recLen);
	    sorted->InsertRecord (recPtr, recLen, rid);
	}
	btree->DestroyFile();

	delete btree;
	delete btreeScan;
	delete [] recPtr;

	return sorted;
}
