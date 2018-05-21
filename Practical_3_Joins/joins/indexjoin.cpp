#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/minirel.h"
#include "../include/heapfile.h"
#include "../include/scan.h"
#include "../include/join.h"
#include "../include/btfile.h"
#include "../include/btfilescan.h"
#include "../include/relation.h"
#include "../include/bufmgr.h"



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


HeapFile* IndexNestedLoopJoin(JoinSpec specOfR, JoinSpec specOfS, long& pinRequests, long& pinMisses, double& duration)
{

        clock_t begin = clock();
	Status status = OK;
        HeapFile* T = new HeapFile(NULL,status);
        if (status != OK){
                cerr << "ERROR: cannot create a file for the joined relations.\n";
                return NULL;
        }

        Scan * scanR = specOfR.file->OpenScan(status);
        if (status != OK){
                cerr << "ERROR: cannot create a file for the joined relations.\n";
                return NULL;
        }

        int recLenR = specOfR.recLen;
        int recLenS = specOfS.recLen;
        int recLenNew = specOfR.recLen + specOfS.recLen;

        RecordID ridR,ridS,ridNew;

        char * recR = new char[recLenR];
        char * recS = new char[recLenS];
        char * recNew = new char[recLenNew];
       


        Scan * scanS = specOfS.file->OpenScan(status);
    	if (status != OK){
                cerr << "ERROR: cannot create a file for S relation.\n";
                return NULL;
        }

	
	BTreeFile *btree;
	btree = new BTreeFile (status, "BTree", ATTR_INT, sizeof(int));
	while(OK == scanS->GetNext(ridS, recS, recLenS)){
		btree->Insert(recS + specOfS.offset, ridS);            
        }
	delete scanS;



	while(OK == scanR->GetNext(ridR, recR, recLenR)){
		int * joinAttrR = (int*)&recR[specOfR.offset]; 
		BTreeFileScan  *btreeScan;
		btreeScan = (BTreeFileScan *)btree->OpenScan(joinAttrR, joinAttrR);


		int key;

		while (btreeScan->GetNext(ridS, &key) == OK)
		{
	    		specOfS.file->GetRecord (ridS, recS, recLenS);
			MakeNewRecord(recNew, recR, recS, recLenR, recLenS);
	   		T->InsertRecord (recNew, recLenNew, ridNew);
	}
delete btreeScan;

}

	
	btree->DestroyFile();
	delete btree;
	
        
	delete scanR;

        
        delete recR;
        delete recS;
        delete recNew;

	MINIBASE_BM->GetStat(pinRequests,pinMisses);

	clock_t end = clock();
	duration = float(end - begin)/CLOCKS_PER_SEC;
	return T;

}
