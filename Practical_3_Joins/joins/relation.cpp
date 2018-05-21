#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/heapfile.h"
#include "../include/scan.h"
#include "../include/join.h"
#include "../include/relation.h"
#include "../include/btfile.h"
#include "../include/btfilescan.h"

void toString(const int n, char* str)
{
	//pad the prefix of the string with 0's
	sprintf(str, "%07d", n);
	//cout<<str<<endl;
}

//--------------------------------------------------------
// Used in CreateR and CreateS, to generate random 
// permutations.   Not useful.
//--------------------------------------------------------

static 
void RandomPermutate (short *permutation, int n)
{
	int temp, random;
	for (int j = 0; j < n; j++)
	{
		permutation[j] = j;
	}

	for (int j = n - 1; j > 0; j--)
	{
		random = rand() % j;
		temp = permutation[j];
		permutation[j] = permutation[random];
		permutation[random] = temp;
	}
}


//--------------------------------------------------------
// Create a random relation R.  R is stored on disk as a 
// HeapFile named "R".  R contains records of type 
// Employee (defined in relation.h)
//--------------------------------------------------------

void CreateR(int rsize,int ssize)
{
	Status s;
	HeapFile *F = new HeapFile ("R", s); // new HeapFile storing records of R
	if (s != OK)
	{
		cerr << "Cannot create new HeapFile R\n";
		exit(1);
	}

	Employee e;
	RecordID rid;
	short permutation[NUM_OF_REC_IN_R/rsize];

	RandomPermutate (permutation, NUM_OF_REC_IN_R/rsize); // generate a random array of integer

	for (int i = 0; i < NUM_OF_REC_IN_R/rsize; i++)
	{
		e.id   = permutation[i];
		e.age  = rand() % 20 + 20;
		e.proj = rand() % (int)(NUM_OF_REC_IN_S/ssize);
		e.salary = (rand() % 300)*100;
		e.rating = rand() % 5;
		e.dept  = rand() % 30;

		s = F->InsertRecord((char *)&e, sizeof(Employee), rid); // insert records into heapfile
		if (s != OK)
		{
			cerr << "Cannot insert record " << i << " into R\n";
			exit(1);
		}
	}

	delete F; // close HeapFile
}


//----------------------------------------------------------
// Initializes the structure spec to indicate which attribute
// of R is the join attribute.  We also open the heapfile "R".
//----------------------------------------------------------

void
CreateSpecForR (JoinSpec &spec)
{
	Status s;

	strcpy(spec.relName, "R");
	spec.numOfAttr = NUM_OF_ATTR_IN_R;
	spec.joinAttr = 2;
	spec.recLen = sizeof(Employee);
	spec.file = new HeapFile (spec.relName, s);
	if (s != OK)
	{
		cerr << "ERROR : cannot open HeapFile " << 
			spec.relName << endl;
		exit(1);
	}
	spec.offset = spec.joinAttr*sizeof(int);
}


//--------------------------------------------------------
// Create a random relation S.  S is stored on disk as a 
// HeapFile named "S".  S contains records of type 
// Project (defined in relation.h)
//--------------------------------------------------------

void CreateS(int rsize,int ssize)
{
	Status s;
	HeapFile *F = new HeapFile ("S", s);
	if (s != OK)
	{
		cerr << "Cannot create new HeapFile S\n";
		exit(1);
	}

	Project e;
	RecordID rid;
	short permutation[NUM_OF_REC_IN_S/ssize];

	RandomPermutate (permutation, NUM_OF_REC_IN_S/ssize);

	for (int i = 0; i < NUM_OF_REC_IN_S/ssize; i++)
	{
		e.id   = permutation[i];
		e.manager  = rand() % (NUM_OF_REC_IN_R/rsize);
		e.fund = (rand() % 500)*10;
		e.status = rand() % 5;

		s = F->InsertRecord((char *)&e, sizeof(Project), rid);
		assert(rid.pageNo != 3);
		if (s != OK)
		{
			cerr << "Cannot insert record " << i << " into S\n";
			exit(1);
		}
	}

	delete F;
}

//----------------------------------------------------------
// Initializes the structure spec to indicate which attribute
// of S is the join attribute.  We also open the heapfile "S".
//----------------------------------------------------------

void CreateSpecForS (JoinSpec &spec)
{
	Status s;

	strcpy(spec.relName, "S");
	spec.numOfAttr = (NUM_OF_ATTR_IN_S);
	spec.recLen = sizeof(Project);
	spec.joinAttr = 0;
	spec.file = new HeapFile (spec.relName, s);
	if (s != OK)
	{
		cerr << "ERROR : cannot open HeapFile " << spec.relName << endl;
		exit(1);
	}
	spec.offset = spec.joinAttr*sizeof(int);
}

//------------------------------------------------------------------
// Print out the content of the result HeapFile, into a file called
// _name_.  Useful for debugging.
//------------------------------------------------------------------

void PrintResult (HeapFile *RS, char *name)
{
	Status s;
	FILE *f;
	Scan *scan = RS->OpenScan(s);
	if (s != OK)
	{
		cerr << "Cannot open scan on result HeapFile." << endl;
	}
	f = fopen(name, "w");
	if (f == NULL)
	{
		cerr << "Cannot open file " << name << " for writing.\n";
	}

	EmployeeProject e;
	int len;
	RecordID rid;

	while (scan->GetNext(rid, (char *)&e, len) != DONE)
	{
		fprintf(f, "%d %d %d\n", e.proj, e.projid, e.id);
	}
	fclose(f);
  delete scan;
}

void PrintR (HeapFile *R, char *name)
{
	Status s;
	FILE *f;
	Scan *scan = R->OpenScan(s);
	if (s != OK)
	{
		cerr << "Cannot open scan on HeapFile R "<< endl;
	}
	f = fopen(name, "w");
	if (f == NULL)
	{
		cerr << "Cannot open file " << name << " for writing.\n";
	}

	Employee e;
	int len;
	RecordID rid;

	while (scan->GetNext(rid, (char *)&e, len) != DONE)
	{
		fprintf(f, "%d %d %d %d %d %d\n", e.id, e.age, e.proj,e.rating, e.salary, e.dept);
	}
	fclose(f);
  delete scan;
}

void PrintS (HeapFile *S, char *name)
{
	Status s;
	FILE *f;
	Scan *scan = S->OpenScan(s);
	if (s != OK)
	{
		cerr << "Cannot open scan on HeapFile S "<< endl;
	}
	f = fopen(name, "w");
	if (f == NULL)
	{
		cerr << "Cannot open file " << name << " for writing.\n";
	}

	Project p;
	int len;
	RecordID rid;

	while (scan->GetNext(rid, (char *)&p, len) != DONE)
	{
		fprintf(f, "%d %d %d %d \n", p.id, p.fund, p.manager, p.status);
	}
	fclose(f);
  delete scan;
}
