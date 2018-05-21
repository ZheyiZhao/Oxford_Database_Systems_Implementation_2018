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

//---------------------------------------------------------------
// Each join method takes in at least two parameters :
// - specOfS
// - specOfR
//
// They specify which relations we are going to join, which// attributes we are going to join on, the offsets of the 
// attributes etc.  specOfS specifies the inner relation while
// specOfR specifies the outer one.
//
//You can use MakeNewRecord() to create the new result record.
//
// Remember to clean up before exiting by "delete"ing any pointers
// that you "new"ed.  This includes any Scan/BTreeFileScan that 
// you have opened.
//---------------------------------------------------------------
HeapFile* BlockNestedLoopJoin(JoinSpec specOfR, JoinSpec specOfS, int B, long& pinRequests, long& pinMisses, double& duration)
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
        char * recRBlock = new char [B];
        char * recR = new char[recLenR];
        char * recS = new char[recLenS];
        char * recNew = new char[recLenNew];
        int NumRecord = B / recLenR;//number of records per block
        bool EndofR = false;

        while(!EndofR){
	char * recRBlock = new char [B];
                for (int i = 0; i < NumRecord; i++){
                        if (OK != scanR->GetNext(ridR, recRBlock + i*recLenR, recLenR)){
                                EndofR = true;
		
                                break;
                        }
		}

        Scan * scanS = specOfS.file->OpenScan(status);
    	if (status != OK){
                cerr << "ERROR: cannot create a file for S relation.\n";
                return NULL;
        }
		while(OK == scanS->GetNext(ridS, recS, recLenS)){
                        int * joinAttrS = (int*)&recS[specOfS.offset]; // relation of employees
                        for (int j = 0; j < B; j += recLenR){

                                int * joinAttrR = (int*)&recRBlock[j+specOfR.offset];
                                if (*joinAttrS == *joinAttrR){
                                	MakeNewRecord(recNew, recRBlock+j, recS, recLenR, recLenS);
                                        T->InsertRecord(recNew, recLenNew, ridNew);
                                }
                        }
                }


        }
        delete recR;
        delete recS;
        delete recNew;
        delete recRBlock;	

	MINIBASE_BM->GetStat(pinRequests,pinMisses);

	clock_t end = clock();
	duration = float(end - begin)/CLOCKS_PER_SEC;
	return T;
}

