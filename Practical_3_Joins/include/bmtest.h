// -*- C++ -*-
#ifndef _BMTESTER_H_
#define _BMTESTER_H_

#include "test.h"


class BMTester : public TestDriver
{

	public:

		// This constructs the tester.  You then test it by 
		// calling RunTests().
		BMTester();
		~BMTester();

		Status RunTests();

	private:
		int Test1();
		int Test2();
		int Test3();
		int Test4();
		int Test5();
		int Test6();
		const char* TestName();
		void RunTest( Status& status, testFunction test );
		Status RunAllTests();
};


#endif
