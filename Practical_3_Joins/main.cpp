#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "include/minirel.h"
#include "include/bufmgr.h"
#include "include/heapfile.h"
#include "include/join.h"
#include "include/relation.h"

int MINIBASE_RESTART_FLAG = 0;// used in minibase part

#define NUM_OF_DB_PAGES  2000 // define # of DB pages
#define NUM_OF_BUF_PAGES 50 // define Buf manager size.You will need to change this for the analysis

int main()
{	
	int BUFF_SIZE = 5;
	int R_SIZE = 1;
	int S_SIZE = 1;
	int REPEAT = 1;
	Status s;
	int rsize =-9;
		for (int ssize = 1; ssize <= S_SIZE; ssize += 10){
		
//	for (int rsize = 1; rsize <= R_SIZE; rsize += 10){
	while (rsize < R_SIZE){
		if (rsize < R_SIZE) rsize += 10;
			for (int buff = 16; buff < 512; buff*=2){
	//cout <<"ratio: " << float (NUM_OF_REC_IN_R / rsize) /  float(NUM_OF_REC_IN_S / ssize)  << endl;
		
			
				//
	// Initialize Minibase's Global Variables
	//
	cout << "buffer pool size: " << buff << endl;
	//cout << "R size: " << NUM_OF_REC_IN_R / rsize << endl;
//	cout << "S size: " << NUM_OF_REC_IN_S / ssize << endl;
	minibase_globals = new SystemDefs(s, 
		"MINIBASE.DB",
		"MINIBASE.LOG",
		NUM_OF_DB_PAGES,   // Number of pages allocated for database
		500,
		//NUM_OF_BUF_PAGES/buff,  // Number of frames in buffer pool
		buff,
		NULL);
	
	//
	// Initialize random seed
	//

	srand(1);

	//
	// Create Random Relations R(outer relation) and S for joining. The definition is in relation.h, 
	// # of tuples: NUM_OF_REC_IN_R, NUM_OF_REC_IN_S in join.h
	//  
	//

	//cerr << "Creating random records for relation R\n";
	CreateR(rsize,ssize);
	//cerr << "Creating random records for relation S\n";
	CreateS(rsize,ssize);

	//
	// Initialize the specification for joins
	//

	JoinSpec specOfS, specOfR;
	Status status = OK;
	CreateSpecForR(specOfR);
	CreateSpecForS(specOfS);
	long pinRequests;
	long pinMisses;
	double duration;
	long sum_request=0;
	long sum_miss=0;
	double sum_duration=0;
	
	/* tuple join */
	for (int i = 0; i < REPEAT ; i++){
		HeapFile* T = new HeapFile(NULL,status);
		if (status != OK){
                	cerr << "ERROR: cannot create a new file.\n";
             
        	}
		MINIBASE_BM->ResetStat();
		T = TupleNestedLoopJoin(specOfR,specOfS, pinRequests, pinMisses, duration);
		sum_request += pinRequests; 
		sum_miss += pinMisses;
		sum_duration += duration;
		delete T;
	}
    		cout << "Tuple " << sum_request/REPEAT << " " << sum_miss/REPEAT << " " << sum_duration / REPEAT << endl;
		
	/* block join */
	sum_request = 0; sum_miss = 0;sum_duration = 0;
	for (int i = 0; i < REPEAT; i++){
		HeapFile* T = new HeapFile(NULL,status);
		MINIBASE_BM->ResetStat();
		int B = (MINIBASE_BM->GetNumOfBuffers()-3*3)*MINIBASE_PAGESIZE;
		T = BlockNestedLoopJoin(specOfR,specOfS, B, pinRequests, pinMisses, duration);	
		sum_request += pinRequests; 
		sum_miss += pinMisses;
		sum_duration += duration;
		delete T;
	}
	cout << "Block " <<sum_request/REPEAT<< " " <<sum_miss/REPEAT << " " << sum_duration / REPEAT << endl;
	/* index join *
	/* block join */
	sum_request = 0; sum_miss = 0;sum_duration = 0;
	for (int i = 0; i < REPEAT; i++){
		HeapFile* T = new HeapFile(NULL,status);
		MINIBASE_BM->ResetStat();
		
		T = IndexNestedLoopJoin(specOfR,specOfS, pinRequests, pinMisses, duration);	
		sum_request += pinRequests; 
		sum_miss += pinMisses;
		sum_duration += duration;
		delete T;
	}
	cout << "Index " <<sum_request/REPEAT<< " " <<sum_miss/REPEAT << " " << sum_duration / REPEAT << endl;
	
    //delete the created database
    remove("MINIBASE.DB");
}
}
	}
	return 1;
}
