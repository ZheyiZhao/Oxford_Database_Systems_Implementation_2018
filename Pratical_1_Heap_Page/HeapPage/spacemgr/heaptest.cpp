//*****************************************
//  Driver program for heapfiles
//****************************************

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/db.h"
#include "../include/heapfile.h"
#include "../include/scan.h"
#include "../include/heaptest.h"
#include "../include/bufmgr.h"


static const int namelen = 24;
struct Rec
{
    int ival;
    double fval;
    char name[namelen];
};

static const int reclen = sizeof(Rec);



HeapDriver::HeapDriver() : TestDriver( "hftest" )
{
    choice = 100;       // big enough for file to occupy >1 page
}

HeapDriver::~HeapDriver()
{}


Status HeapDriver::RunTests()
{
    return TestDriver::RunTests();
}

Status HeapDriver::RunAllTests()
{
    Status answer;
    minibase_globals = new SystemDefs(answer,"MINIBASE.DB", "MINIBASE.LOG",
		100,500,100,"Clock");
    if ( answer == OK )
        answer = TestDriver::RunAllTests();
	
	
    delete minibase_globals;
    return answer;
}

const char* HeapDriver::TestName()
{
    return "Heap File";
}


//********************************************

int HeapDriver::Test1()
{
    cout << "\n  Test 1: Insert and scan fixed-size records\n";
    Status status = OK;
    RecordID rid;
	
    cout << "  - Create a heap file\n";
    HeapFile f("file_1", status);
	
    if (status != OK)
        cerr << "*** Could not create heap file\n";
    else if ( MINIBASE_BM->GetNumOfUnpinnedBuffers() != MINIBASE_BM->GetNumOfBuffers() )
	{
        cerr << "*** The heap file has left pages pinned\n";
        status = FAIL;
	}
	
	
    if ( status == OK )
	{
        cout << "  - Add " << choice << " records to the file\n";
        for (int i =0; i<choice && status == OK; i++)
		{
            Rec rec = { i, i*2.5 };
            sprintf(rec.name, "record %i",i);
			
            status = f.InsertRecord((char *)&rec, reclen, rid);
			
            if (status != OK)
                cerr << "*** Error inserting record " << i << endl;
            else if ( MINIBASE_BM->GetNumOfUnpinnedBuffers() != MINIBASE_BM->GetNumOfBuffers() )
			{
                cerr << "*** Insertion left a page pinned\n";
                status = FAIL;
			}
		}
		
        if ( f.GetNumOfRecords() != choice )
		{
            status = FAIL;
            cerr << "*** File reports " << f.GetNumOfRecords() << " records, not "
				<< choice << endl;
		}
	}
	
	
	// In general, a sequential scan won't be in the same order as the
	// insertions.  However, we're inserting fixed-length records here, and
	// in this case the scan must return the insertion order.
    Scan* scan = 0;
    if ( status == OK )
	{
        cout << "  - Scan the records just inserted\n";
        scan = f.OpenScan(status);
		
        if (status != OK)
            cerr << "*** Error opening scan\n";
        else if ( MINIBASE_BM->GetNumOfUnpinnedBuffers() == MINIBASE_BM->GetNumOfBuffers() )
		{
            cerr << "*** The heap-file scan has not pinned the first page\n";
            status = FAIL;
		}
	}
    if ( status == OK )
	{
        int len, i = 0;
        Rec rec;
		
        while ( (status = scan->GetNext(rid, (char *)&rec, len)) == OK )
		{
            if ( len != reclen )
			{
                cerr << "*** Record " << i << " had unexpected length " << len
					<< endl;
                status = FAIL;
                break;
			}
            else if ( MINIBASE_BM->GetNumOfUnpinnedBuffers() == MINIBASE_BM->GetNumOfBuffers() && i != choice - 1)
			{
                cerr << "*** The heap-file scan has not left its page pinned\n";
                status = FAIL;
                break;
			}
            char name[ sizeof rec.name ];
            sprintf( name, "record %i", i );
            if( rec.ival != i  ||
                rec.fval != i*2.5  ||
                0 != strcmp( rec.name, name ) )
			{
                cerr << "*** Record " << i << " differs from what we inserted\n";
                status = FAIL;
                break;
			}
            ++i;
		}
		
        if ( status == DONE )
		{
            if ( MINIBASE_BM->GetNumOfUnpinnedBuffers() != MINIBASE_BM->GetNumOfBuffers() )
                cerr << "*** The heap-file scan has not unpinned its page after finishing\n";
            else if ( i == choice )
                status = OK;
            else
                cerr << "*** Scanned " << i << " records instead of "
				<< choice << endl;
		}
	}
	
    delete scan;
	
    if ( status == OK )
        cout << "  Test 1 completed successfully.\n";
    return (status == OK);
}


//***************************************************

int HeapDriver::Test2()
{
    cout << "\n  Test 2: Delete fixed-size records\n";
    Status status = OK;
    Scan* scan = 0;
    RecordID rid;
	
    cout << "  - Open the same heap file as Test 1\n";
    HeapFile f("file_1", status);
	
    if (status != OK)
        cerr << "*** Error opening heap file\n";
	
	
    if ( status == OK )
	{
        cout << "  - Delete half the records\n";
        scan = f.OpenScan(status);
        if (status != OK)
            cerr << "*** Error opening scan\n";
	}
    if ( status == OK )
	{
        int len, i = 0;
        Rec rec;
		
        while ( (status = scan->GetNext(rid, (char *)&rec, len)) == OK )
		{
            if ( i & 1 )        // Delete the odd-numbered ones.
			{
                status = f.DeleteRecord( rid );
                if ( status != OK )
				{
                    cerr << "*** Error deleting record " << i << endl;
                    break;
				}
			}
            ++i;
		}
		
        if ( status == DONE )
            status = OK;
	}
	
    delete scan;
    scan = 0;
	
    if ( status == OK
		&& MINIBASE_BM->GetNumOfUnpinnedBuffers() != MINIBASE_BM->GetNumOfBuffers() )
	{
        cerr << "*** Deletion left a page pinned\n";
        status = FAIL;
	}
	
    if ( status == OK )
	{
        cout << "  - Scan the remaining records\n";
        scan = f.OpenScan(status);
        if (status != OK)
            cerr << "*** Error opening scan\n";
	}
    if ( status == OK )
	{
        int len, i = 0;
        Rec rec;
		
        while ( (status = scan->GetNext(rid, (char *)&rec, len)) == OK )
		{
            if( rec.ival != i  ||
                rec.fval != i*2.5 )
			{
                cerr << "*** Record " << i << " differs from what we "
                    "inserted\n";
                status = FAIL;
                break;
			}
            i += 2;     // Because we deleted the odd ones...
		}
		
        if ( status == DONE )
            status = OK;
	}
	
    delete scan;
	
    if ( status == OK )
        cout << "  Test 2 completed successfully.\n";
    return (status == OK);
}


//********************************************************

int HeapDriver::Test3()
{
    cout << "\n  Test 3: Update fixed-size records\n";
    Status status = OK;
    Scan* scan = 0;
    RecordID rid;
	
    cout << "  - Open the same heap file as tests 1 and 2\n";
    HeapFile f("file_1", status);
	
    if (status != OK)
        cerr << "*** Error opening heap file\n";
	
	
    if ( status == OK )
	{
        cout << "  - Change the records\n";
        scan = f.OpenScan(status);
        if (status != OK)
            cerr << "*** Error opening scan\n";
	}
    if ( status == OK )
	{
        int len, i = 0;
        Rec rec;
		
        while ( (status = scan->GetNext(rid, (char *)&rec, len)) == OK )
		{
            rec.fval = (float)7*i;     // We'll check that i==rec.ival below.
            status = f.UpdateRecord( rid, (char*)&rec, len );
            if ( status != OK )
			{
                cerr << "*** Error updating record " << i << endl;
                break;
			}
            i += 2;     // Recall, we deleted every other record above.
		}
		
        if ( status == DONE )
            status = OK;
	}
	
    delete scan;
    scan = 0;
	
    if ( status == OK
		&& MINIBASE_BM->GetNumOfUnpinnedBuffers() != MINIBASE_BM->GetNumOfBuffers() )
	{
        cerr << "*** Updating left pages pinned\n";
        status = FAIL;
	}
	
    if ( status == OK )
	{
        cout << "  - Check that the updates are really there\n";
        scan = f.OpenScan(status);
        if (status != OK)
            cerr << "*** Error opening scan\n";
	}
    if ( status == OK )
	{
        int len, i = 0;
        Rec rec, rec2;
		
        while ( (status = scan->GetNext(rid, (char *)&rec, len)) == OK )
		{
			// While we're at it, Test the getRecord method too.
            status = f.GetRecord( rid, (char*)&rec2, len );
            if ( status != OK )
			{
                cerr << "*** Error getting record " << i << endl;
                break;
			}
            if( rec.ival != i || rec.fval != i*7
                || rec2.ival != i || rec2.fval != i*7 )
			{
                cerr << "*** Record " << i << " differs from our "
                    "update\n";
                status = FAIL;
                break;
			}
            i += 2;     // Because we deleted the odd ones...
		}
		
        if ( status == DONE )
            status = OK;
	}
	
    delete scan;
	
    if ( status == OK )
        cout << "  Test 3 completed successfully.\n";
    return (status == OK);
}


//*****************************************************************

int HeapDriver::Test4()
{
    cout << "\n  Test 4: Temporary heap files and variable-length records\n";
    Status status = OK;
    Scan* scan = 0;
    RecordID rid;
	
    cout << "  - Create a temporary heap file\n";
    HeapFile f( 0, status );
	
    if (status != OK)
        cerr << "*** Error creating temp file\n";
	
	
    if ( status == OK )
        cout << "  - Add variable-sized records\n";
	
    unsigned recSize = MINIBASE_PAGESIZE / 2;
	/* We use half a page as the starting size so that a single page won't
	hold more than one record.  We add a series of records at this size,
	then halve the size and add some more, and so on.  We store the index
	number of each record on the record itself. */
    unsigned numRecs = 0;
    char record[MINIBASE_PAGESIZE] = "";
    for ( ; recSize >= (sizeof numRecs + sizeof recSize) && status == OK;
	recSize /= 2 )
        for ( unsigned i=0; i < 10 && status == OK; ++i, ++numRecs )
	{
		memcpy( record, &numRecs, sizeof numRecs );
		memcpy( record+(sizeof numRecs), &recSize, sizeof recSize );
		status = f.InsertRecord( record, recSize, rid );
		if ( status != OK )
			cerr << "*** Failed inserting record " << numRecs
			<< ", of size " << recSize << endl;
	}
	
	
    int *seen = new int[numRecs];
	for (int i = 0; i < (int)numRecs; i++)
	{
		seen[i] = FALSE;
	}

    memset( seen, 0, sizeof seen );
    if ( status == OK )
	{
        cout << "  - Check that all the records were inserted\n";
        scan = f.OpenScan(status);
        if (status != OK)
            cerr << "*** Error opening scan\n";
	}
    if ( status == OK )
	{
        int len;
        while ( (status = scan->GetNext(rid, record, len)) == OK )
		{
            unsigned recNum;
            memcpy( &recNum, record, sizeof recNum );
            if ( seen[recNum])
			{
                cerr << "*** Found record " << recNum << " twice!\n";
                status = FAIL;
                break;
			}
            seen[recNum] = TRUE;
			
            memcpy( &recSize, record + sizeof recNum, sizeof recSize );
            if ( recSize != (unsigned)len )
			{
                cerr << "*** Record size mismatch: stored " << recSize
					<< ", but retrieved " << len << endl;
                status = FAIL;
                break;
			}
			
            --numRecs;
		}
		
        if ( status == DONE )
            if ( numRecs )
			cerr << "*** Scan missed " << numRecs << " records\n";
		else
			status = OK;
	}
	
    delete scan;
	
    if ( status == OK )
        cout << "  Test 4 completed successfully.\n";
    return (status == OK);
}



int HeapDriver::Test5()
{
    cout << "\n  Test 5: Test some error conditions\n";
    Status status = OK;
    Scan* scan = 0;
    RecordID rid;
	
    HeapFile f("file_1", status);
    if (status != OK)
        cerr << "*** Error opening heap file\n";
	
    if ( status == OK )
	{
        cout << "  - Try to change the size of a record\n";
        scan = f.OpenScan(status);
        if (status != OK)
            cerr << "*** Error opening scan\n";
	}
    if ( status == OK )
	{
        int len;
        Rec rec;
        status = scan->GetNext(rid, (char *)&rec, len);
        if ( status != OK )
            cerr << "*** Error reading first record\n";
        else
		{
            status = f.UpdateRecord( rid, (char*)&rec, len-1 );
            TestFailure( status, HEAPFILE, "Shortening a record" );
            if ( status == OK )
			{
                status = f.UpdateRecord( rid, (char*)&rec, len+1 );
                TestFailure( status, HEAPFILE, "Lengthening a record" );
			}
		}
	}
	
    delete scan;
	
	
    if ( status == OK )
	{
        cout << "  - Try to insert a record that's too long\n";
        char record[MINIBASE_PAGESIZE] = "";
        status = f.InsertRecord( record, MINIBASE_PAGESIZE, rid );
        TestFailure( status, HEAPFILE, "Inserting a too-long record" );
	}
	
	
	
    if ( status == OK )
        cout << "  Test 5 completed successfully.\n";
    return (status == OK);
}


int HeapDriver::Test6()
{
    return TRUE;
}
