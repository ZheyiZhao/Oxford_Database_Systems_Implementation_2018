#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <assert.h>
#include <unistd.h>

#define _getpid getpid

using namespace std;

#include "include/new_error.h"
#include "include/test.h"

TestDriver::TestDriver( const char* nameRoot )
{
    unsigned len = (unsigned int)strlen(nameRoot);
    char basename[200];
    char dbfname[200];
    char logfname[200];

    sprintf( basename, "%s%ld", nameRoot, _getpid() );
    sprintf( dbfname, "%s.minibase-db", basename );
    sprintf( logfname, "%s.minibase-log", basename );

    dbpath = strdup(dbfname);
    logpath = strdup(logfname);
}


TestDriver::~TestDriver()
{
    ::free(dbpath);
    ::free(logpath);
}


bool TestDriver::Test1()
{
    return true;
}

bool TestDriver::Test2()
{
    return true;
}

bool TestDriver::Test3()
{
    return true;
}

bool TestDriver::Test4()
{
    return true;
}

bool TestDriver::Test5()
{
    return true;
}

bool TestDriver::Test6()
{
    return true;
}


const char* TestDriver::TestName()
{
    return "*** unknown ***";   // A little reminder to subclassers.
}


void TestDriver::TestFailure( Status& status, Status expectedStatus,
                              const char* activity, int postedErrExpected )
{
    if ( status == OK )
      {
        status = FAIL;
        cerr << "*** " << activity << " did not return a failure status.\n";
      }
	/*
    else if ( status != expectedStatus )
        cerr << "*** " << activity << " correctly returned a failure status,\n"
             << "    but not the expected one.\n";
    else if ( postedErrExpected && !minibase_errors.error() )
        cerr << "*** " << activity << " correctly returned a failure status,\n"
             << "    but did not log the error.\n";
    else if ( !postedErrExpected && minibase_errors.error() )
        cerr << "*** " << activity << " correctly returned a failure status,\n"
             << "    but unexpectedly logged the error.\n";
	*/
    else
      {
        status = OK;
        cout << "    --> Failed as expected\n";
      }

    if ( status != OK )
        minibase_errors.show_errors();
    minibase_errors.clear_errors();
}



void TestDriver::RunTest( Status& status, TestDriver::testFunction test )
{
    /*
	minibase_errors.clear_errors();
    int result = test();
    if ( !result || minibase_errors.error() )
    {
        status = FAIL;
        if ( minibase_errors.error() )
            cerr << (result? "*** Unexpected error(s) logged, test failed:\n"
                     : "Errors logged:\n");
        minibase_errors.show_errors(cerr);
    }
	*/
}


Status TestDriver::RunTests()
{
    cout << "\nRunning " << TestName() << " tests...\n" << endl;


    // Kill anything that might be hanging around.

    char* newdbpath;
    char* newlogpath;
   
	//char remove_logcmd[50];
	//char remove_dbcmd[50];

    newdbpath = new char[ strlen(dbpath) + 20];
    newlogpath = new char[ strlen(logpath) + 20];
    strcpy(newdbpath,dbpath); 
    strcpy(newlogpath, logpath);

/*	sprintf(remove_logcmd, "del %s", logpath);
	sprintf(remove_dbcmd, "del %s", dbpath);
	
	system(remove_logcmd);
	system(remove_dbcmd);
*/
	sprintf(newdbpath, "%s", dbpath);
	sprintf(newlogpath, "%s", logpath);


    unlink( newdbpath );
    unlink( newlogpath );

    minibase_errors.clear_errors();


      // Run the tests.
    Status answer = RunAllTests();


      // Clean up.
	/*
    unlink( newdbpath );
    unlink( newlogpath );
	*/

    minibase_errors.clear_errors();

    cout << "\n..." << TestName() << " tests "
         << (answer == OK ? "completed successfully" : "failed")
         << ".\n\n";

    delete newdbpath; delete newlogpath;

    return answer;
}


Status TestDriver::RunAllTests()
{
    Status status = OK;
	int result;
	int i;
	const int inTxtLen = 32;
	char *inputTxt = new char[inTxtLen];

	//there are really 6 tests but the last one doesn't do anything so I'll only mention 1-5
	cout << "Input a space separated test sequence (ie. a list of numbers " << endl <<
		" in the range 1-5: 1 5 2 3) or hit ENTER to run all tests: ";

	cin.getline ( inputTxt, inTxtLen );
	if ( strlen(inputTxt) == 0 )
	{
		inputTxt = "12345";
	}	
	for ( i = 0; i < (int)strlen(inputTxt); i++)
	{
		switch ( inputTxt[i] )
		{
		case '1' : 
			minibase_errors.clear_errors();
			result = Test1();
			if ( !result || minibase_errors.error() )
			{
				status = FAIL;
				if ( minibase_errors.error() )
					cerr << (result? "*** Unexpected error(s) logged, test failed:\n"
					: "Errors logged:\n");
				minibase_errors.show_errors(cerr);
			}
			break;
		case '2' :
			minibase_errors.clear_errors();
			result = Test2();
			if ( !result || minibase_errors.error() )
			{
				status = FAIL;
				if ( minibase_errors.error() )
					cerr << (result? "*** Unexpected error(s) logged, test failed:\n"
					: "Errors logged:\n");
				minibase_errors.show_errors(cerr);
			}
			break;
		case '3' :
			minibase_errors.clear_errors();
			result = Test3();
			if ( !result || minibase_errors.error() )
			{
				status = FAIL;
				if ( minibase_errors.error() )
					cerr << (result? "*** Unexpected error(s) logged, test failed:\n"
					: "Errors logged:\n");
				minibase_errors.show_errors(cerr);
			}    
			break;
		case '4' :
			minibase_errors.clear_errors();
			result = Test4();
			if ( !result || minibase_errors.error() )
			{
				status = FAIL;
				if ( minibase_errors.error() )
					cerr << (result? "*** Unexpected error(s) logged, test failed:\n"
					: "Errors logged:\n");
				minibase_errors.show_errors(cerr);
			}
			break;
		case '5' :
			minibase_errors.clear_errors();
			result = Test5();
			if ( !result || minibase_errors.error() )
			{
				status = FAIL;
				if ( minibase_errors.error() )
					cerr << (result? "*** Unexpected error(s) logged, test failed:\n"
					: "Errors logged:\n");
				minibase_errors.show_errors(cerr);
			}
			break;
		case '6' :
			minibase_errors.clear_errors();
			result = Test6();
			if ( !result || minibase_errors.error() )
			{
				status = FAIL;
				if ( minibase_errors.error() )
					cerr << (result? "*** Unexpected error(s) logged, test failed:\n"
					: "Errors logged:\n");
				minibase_errors.show_errors(cerr);
			}

			minibase_errors.clear_errors();
			break;
		}
	}
    return status;
}
