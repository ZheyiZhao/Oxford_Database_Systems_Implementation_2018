#ifndef RELATION_H
#define RELATION_H


#include "join.h"
typedef struct Employee {
	int id;
	int age;
	int proj;
	int salary;
	int rating;
	int dept;
} Employee; // Definition of R relation


typedef struct Project {
	int id;
	int fund;
	int manager;
	int status;
} Project; // Definition of S relation

typedef struct EmployeeProject {
	int id;
	int age;
	int proj;
	int salary;
	int rating;
	int dept;
	int projid;
	int fund;
	int manager;
	int status;
} EmployeeProject; // Joined relation. We omit remove of duplicate attributes.
// We join on the proj attribute of Employee and id attribute of Project.

#define SWAP(i,j) {temp = i; i = j; j = temp;}

void CreateS(); // Create Relation S
void CreateR(); // Create Relation R
void CreateSpecForS (JoinSpec &spec); // Create Join Specification for S
void CreateSpecForR (JoinSpec &spec);// Create Join Specification for R
HeapFile *SortFile(HeapFile *S, int len, int offset);
// Sort a relation stored in HeapFile S, len is the length of record, offset is the offset
// of sort key attribute from the beginning of record, i.e. recptr+offset point to the sort key
void PrintResult(HeapFile *RS, char *name); // Print the result of Joined relation RS to file whose filename is name

#endif