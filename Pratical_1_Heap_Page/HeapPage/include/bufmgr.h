
#ifndef _BUF_H
#define _BUF_H

#include "db.h"
#include "page.h"
#include "frame.h"
#include "replacer.h"
#include "hash.h"

class BufMgr
{
	private:

		/*
		 * hashTable to give hash access to frames. No obligation to use it, however if you want to use it you need
		 * define and implement the HashTable.cpp file
		 */
		HashTable *hashTable;
		Frame **frames; //pool of frames

		/*
		 * This is the component responsible of implementing the buffer replacement policy. You need to create the Replacer.cpp
		 * file and define there your chose replacement policy
		 */
		Replacer *replacer;
		int   numOfBuf; // number of buffers

		int FindFrame( PageID pid );
		long totalCall; //total number of times that upper layers try to pin a page
		long totalHit; //total number of times that upper layers try to pin a page and the page is already in the buffer
		long numDirtyPageWrites; //total number of times that a page has been modified and written back to disk

	public:

		BufMgr( int bufsize );
		~BufMgr();
		Status PinPage( PageID pid, Page*& page, Bool emptyPage=FALSE );
		Status UnpinPage( PageID pid, Bool dirty=FALSE );
		Status NewPage( PageID& pid, Page*& firstpage,int howmany=1 );
		Status FreePage( PageID pid );
		Status FlushPage( PageID pid );
		Status FlushAllPages();
		Status  GetStat(long& pinNo, long& missNo) { pinNo = totalCall; missNo = totalCall-totalHit; return OK;}

		unsigned int GetNumOfUnpinnedFrames();

		unsigned int GetNumOfBuffers();
		unsigned int GetNumOfUnpinnedBuffers();

		void   PrintStat();
		void   ResetStat() { totalHit = 0; totalCall = 0; numDirtyPageWrites = 0;}
};


#endif // _BUF_H
