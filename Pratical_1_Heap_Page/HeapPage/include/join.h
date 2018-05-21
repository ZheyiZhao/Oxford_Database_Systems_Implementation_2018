#ifndef JOIN_H
#define JOIN_H

#include "minirel.h"
#include "time.h"
#define MAX_REL_NAME_LENGTH 6 // MAX relation name length
#define MAX_ATTR 10 // Max # of attributes

typedef struct JoinSpec {
	char      relName[MAX_REL_NAME_LENGTH+1];// relation name
	HeapFile *file; // heapfile which store the relation
	int       numOfAttr;// # of arributes
	int       recLen; // length of each record
	int       joinAttr; // join attribute, = i means the ith attribute
	int       offset; // offset: the offset of join attribute from the beginning of record
} JoinSpec;

#define ATTR_INT  attrInteger
#define ATTR_STRING attrString

#define NUM_OF_REC_IN_R 10000 // # of records in R
#define NUM_OF_REC_IN_S 2500 // # of records in S
#define NUM_OF_ATTR_IN_R 6
#define NUM_OF_ATTR_IN_S 4

// Make a new Record for a matching pair of records from R, S. 
// You need to allocate space for newRecord before calling this function.
void MakeNewRecord(char *newRecord, char *r, char *s, int recLenR, int recLenS);

void TupleNestedLoopJoin(JoinSpec, JoinSpec, long& pinRequests, long& pinMisses, double& duration);
// int arg is blocksize
void BlockNestedLoopJoin(JoinSpec, JoinSpec, int B, long& pinRequests, long& pinMisses, double& duration);
void IndexNestedLoopJoin(JoinSpec, JoinSpec, long& pinRequests, long& pinMisses, double& duration);
void SortMergeJoin(JoinSpec, JoinSpec, long& pinRequests, long& pinMisses, double& duration);

#endif

