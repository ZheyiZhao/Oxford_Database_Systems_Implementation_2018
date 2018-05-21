#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/minirel.h"
#include "../include/heapfile.h"
#include "../include/scan.h"
#include "../include/join.h"
#include "../include/relation.h"
#include "../include/bufmgr.h"
/*
typedef struct JoinSpec {
	char      relName[MAX_REL_NAME_LENGTH+1];// relation name
	HeapFile *file; // heapfile which store the relation
	int       numOfAttr;// # of arributes
	int       recLen; // length of each record
	int       joinAttr; // join attribute, = i means the ith attribute
	int       offset; // offset: the offset of join attribute from the beginning of record
} JoinSpec;

*/

//---------------------------------------------------------------
// Each join method takes in at least two parameters :
// - specOfS
// - specOfR
//
// They specify which relations we are going to join, which 
// attributes we are going to join on, the offsets of the 
// attributes etc.  specOfS specifies the inner relation while
// specOfR specifies the outer one.
//
//You can use MakeNewRecord() to create the new result record.
//
// Remember to clean up before exiting by "delete"ing any pointers
// that you "new"ed.  This includes any Scan/BTreeFileScan that 
// you have opened.
//---------------------------------------------------------------



HeapFile* TupleNestedLoopJoin(JoinSpec specOfR, JoinSpec specOfS, long& pinRequests, long& pinMisses, double& duration)
{	
	//start timer
	clock_t begin = clock();
	
	//open files and relations
	Status s;
	Scan *scanR;
	scanR = specOfR.file->OpenScan(s);
	if (s != OK){
		cerr << "ERROR : cannot open scan on R.\n";
		return NULL;
	}

	HeapFile *T;
	T = new HeapFile(NULL, s); // create a temp HeapFile
	if (s != OK)
	{
	    	cerr << "Cannot create new file\n";
		return NULL;
	}
	
	// definition 
	char *recPtrR = new char[specOfR.recLen];
	char *recPtrS = new char[specOfS.recLen];
	char * newRecord = new char[specOfR.recLen + specOfS.recLen];
	RecordID ridR;
	RecordID ridS;
	RecordID ridT;
	int recLenR = specOfR.recLen;
        int recLenS = specOfS.recLen;
        int recLenNew = specOfR.recLen + specOfS.recLen;

	while (scanR->GetNext(ridR, recPtrR,recLenR) == OK){

		Scan *scanS;
		scanS = specOfS.file->OpenScan(s);
		if (s != OK){
		cerr << "ERROR : cannot open scan on the heapfile of S to sort.\n";
		}
	
		while (scanS->GetNext(ridS, recPtrS, recLenS) == OK){
			
			int* joinAttrR = (int*)&recPtrR[specOfR.offset];			   		             	int* joinAttrS = (int*)&recPtrS[specOfS.offset];
			if (*joinAttrR == *joinAttrS){
				MakeNewRecord(newRecord,recPtrR,recPtrS,recLenR,recLenS);
				T->InsertRecord (newRecord,recLenNew, ridT); 
			}
		}
		delete scanS;
	}
	
	MINIBASE_BM->GetStat(pinRequests,pinMisses);
	
	delete recPtrR;
	delete recPtrS;
	delete scanR;
	delete newRecord;
	
	//end timer 
	clock_t end = clock();
	duration =float (end - begin)/CLOCKS_PER_SEC;
	
	return T;
	


}
