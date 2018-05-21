
#include "../include/bufmgr.h"
#include "../include/frame.h"
// db ops:

//Status DB::AllocatePage(PageID& start_page_num, int run_size_int)
//Status DB::DeallocatePage(PageID start_page_num, int run_size)
//Status DB::ReadPage(PageID pageno, Page* pageptr)
//Status DB::WritePage(PageID pageno, Page* pageptr)

//--------------------------------------------------------------------
// Constructor for BufMgr
//
// Input   : bufSize  - number of pages in the this buffer manager
// Output  : None
// PostCond: All frames are empty.
//--------------------------------------------------------------------

BufMgr::BufMgr( int bufSize )
{
    //Frame **frames; //pool of frames
    Frame** frames = new Frame*[numOfBuf]; // row
    for(int i = 0; i < numOfBuf; ++i)
        frames[i] = new int[bufSize]; // column
    
    for (int i = 0 ; i < numOfBuf; i++)
        for (j = 0; j < bufSize; j++){
            frames[i][j].data = NULL;
        }
    PageID lru_queue[bufSize * numOfBuf];
    head = 0;
    tail = 0;
}


//--------------------------------------------------------------------
// Destructor for BufMgr
//
// Input   : None
// Output  : None
//--------------------------------------------------------------------

BufMgr::~BufMgr()
{   
	// flush all dirty pages to disk 
	for (int i = 0 ; i < numOfBuf; i++)
        for (j = 0; j < bufSize; j++){
		if (frames[i][j].IsDirty())
			FlushPage(frames[i]->pid); // do something about return value?
	}
	//deallocate the buffer pool
    for (int i = 0 ; i < numOfBuf; i++)//Free each sub-array
        delete[] frames[i];
	delete[] frames; //Free the array of pointers
}

//--------------------------------------------------------------------
// BufMgr::PinPage
//
// Input    : pid     - page id of a particular page 
//            isEmpty - (optional, default to false) if true indicate
//                      that the page to be pinned is an empty page.
// Output   : page - a pointer to a page in the buffer pool. (NULL
//            if fail)
// Purpose  : Pin the page with page id = pid to the buffer.  
//            Read the page from disk unless isEmpty is true or unless
//            the page is already in the buffer.
// Condition: Either the page is already in the buffer, or there is at
//            least one frame available in the buffer pool for the 
//            page.
// PostCond : The page with page id = pid resides in the buffer and 
//            is pinned. The number of pin on the page increase by
//            one.
// Return   : OK if operation is successful.  FAIL otherwise.
//--------------------------------------------------------------------


Status BufMgr::PinPage(PageID pid, Page*& page, bool isEmpty)
{
    int row,col;
	int frameNum = FindFrame(pid);
	if (frameNum != INVALID_FRAME){/*in the buffer pool*/
        col = frameNum % bufSize;
        row = frameNum / bufSize;
        frames[row][col].pinCount += 1; // increase pin count
        page = &frames[row][col]; // point to this page
    }
		
    else if (frameNum == INVALID_FRAME && isEmpty == true)
        /* not in the buffer pool and this page is empty*/
    {
        //choose a victim frame
        Clock clock;
        int vicIndex = clock.PickVictim();
        //pin the frame to an empty page (set pid)
        col = vicIndex % bufSize;
        row = vicIndex / bufSize;
        // put the page into the empty frame
        Frame newFrame = new Frame;
        newFrame.pid = pid;
        newFrame.pinCount = 1;
        newFrame.data = NULL;
        newFrame.referenced = false;
        frames[row][col] = newFrame;
        
    }
    else if (frameNum == INVALID_FRAME && isEmpty == true)
        /* not in the buffer pool and this page is not empty: read from disk*/
    {
        //choose a victim frame
        /* CLOCK */
        /*Clock clock;
        int vicIndex = clock.PickVictim();
         */
        /* LRU */
        PageNo vicPid = lru_queue[head];
        head = head + 1;
        int vicPid = FindFrame(vicPid);
        col = vicIndex % bufSize;
        row = vicIndex / bufSize;
        //read from disk
        //Status DB::ReadPage(PageID pageno, Page* pageptr)
        DB db;
        Status status;
        Frame newFrame = new Frame;
        status= db.ReadPage(pid,&page);
        if (status == FAIL)
            return FAIL;
        //put in the frame
        newFrame.pid = -1;//empty pid?
        newFrame.pinCount = 1;
        newFrame.referenced = false;
        newFrame.dirty = 0;
        newFrame.data = page;
        frames[row][col] = newFrame;
    }
    else{ //unable to pin
        page = NULL;
        return FAIL;
    }
	
	return OK;
	
	
} 

//--------------------------------------------------------------------
// BufMgr::UnpinPage
//
// Input    : pid     - page id of a particular page 
//            dirty   - indicate whether the page with page id = pid
//                      is dirty or not. (Optional, default to false)
// Output   : None
// Purpose  : Unpin the page with page id = pid in the buffer. Mark 
//            the page dirty if dirty is true.  
// Condition: The page is already in the buffer and is pinned.
// PostCond : The page is unpinned and the number of pin on the
//            page decrease by one. 
// Return   : OK if operation is successful.  FAIL otherwise.
//--------------------------------------------------------------------


Status BufMgr::UnpinPage(PageID pid, bool dirty)
{
	//find the frame
    int index = FindFrame(pid);
    col = index % bufSize;
    row = index / bufSize;
    /*not pinned*/
    if (frames[row][col].pinCount == 0)
        return FAIL;
    /*pinned*/
    else{
        frames[row][col].pinCount -= 1;
        if (dirty == 1)
            frames[row][col].dirty = 1;
        if (frames[row][col].pinCount == 0){
            /* CLOCK */
            //frames[row][col].referenced = 1; // turn on
            /* LRU */
            lru_queue[tail] = pid;
            tail += 1;
            
        }
        return OK;
    }
}

//--------------------------------------------------------------------
// BufMgr::NewPage
//
// Input    : howMany - (optional, default to 1) how many pages to 
//                      allocate.
// Output   : firstPid  - the page id of the first page (as output by
//                   DB::AllocatePage) allocated.
//            firstPage - a pointer to the page in memory.
// Purpose  : Allocate howMany number of pages, and pin the first page
//            into the buffer. 
// Condition: howMany > 0 and there is at least one free buffer space
//            to hold a page.
// PostCond : The page with page id = pid is pinned into the buffer.
// Return   : OK if operation is successful.  FAIL otherwise.
// Note     : You can call DB::AllocatePage() to allocate a page.  
//            You should call DB:DeallocatePage() to deallocate the
//            pages you allocate if you failed to pin the page in the
//            buffer.
//--------------------------------------------------------------------

// added: if there is free space in the buffer
bool BufMgr::FreeSpace (){
    for (int i = 0 ; i < numOfBuf; i++)
        for (j = 0; j < bufSize; j++){
            if (frames[i][j].data == NULL)
                return true;
        }
    return false;
}

Status BufMgr::NewPage (PageID& firstPid, Page*& firstPage, int howMany)
{
	// allocate a run of new pages (number : howMany)
    if (howMany <= 0)
        return FAIL;
    //as least one free buffer space
    if(FreeSpace == false)
        return FAIL;
    //Status DB::AllocatePage(PageID& start_page_num, int run_size_int)
    //Gives back the page number of the first page of the allocated run.
    DB db;
    db.AllocatePage(firstPid,howMany); // output:firstPid
    // pin the first page into the buffer
    // Status BufMgr::PinPage(PageID pid, Page*& page, bool isEmpty)
    STATUS status;
    // set firstPage
    status = PinPage(firstPid,firstPage,true); // first allocated: empty?
    if (status == OK){
        return OK;
    }
    else{ // fail to pin:deallocate
        //Status DB::DeallocatePage(PageID start_page_num, int run_size)
        db.DeallocatePage(firstPid,howMany);
        return FAIL;
        
    }
}

//--------------------------------------------------------------------
// BufMgr::FreePage
//
// Input    : pid     - page id of a particular page 
// Output   : None
// Purpose  : Free the memory allocated for the page with 
//            page id = pid  
// Condition: Either the page is already in the buffer and is pinned
//            no more than once, or the page is not in the buffer.
// PostCond : The page is unpinned, and the frame where it resides in
//            the buffer pool is freed.  Also the page is deallocated
//            from the database. 
// Return   : OK if operation is successful.  FAIL otherwise.
// Note     : You can call MINIBASE_DB->DeallocatePage(pid) to
//            deallocate a page.
//--------------------------------------------------------------------

//added: free a frame
Status BufMgr::FreeFrame(int row, int col){
    frames[row][col].pid = -1;
    frames[row][col].pincount = 0;
    frames[row][col].dirty = 0;
    frames[row][col].referenced = false;
    frames[row][col].data = NULL;
    return OK;
}

Status BufMgr::FreePage(PageID pid)
{
    //Status DB::DeallocatePage(PageID start_page_num, int run_size)
    int index = FindFrame(pid);
    if (index == INVALID_FRAME) // page not in the buffer
        return FAIL;
    else{// page in the buffer
        col = index % bufSize;
        row = index / bufSize;
        PageID start_page_num = frames[row][col].pid;
        if (frames[row][col].pinCount <= 1){
            //is pinned no more than once
            Status status;
            DB db;
            //deallocate the page from the database
            status = db.DeallocatePage(start_page_num);//default run_size = 1
            if (status == FAIL)
                return FAIL;
            //free the frame
            FreeFrame(row,col);
        }
        else { // pinned more than once
            return FAIL; //pinCount > 1
        }
        
           }
	return OK;
}


//--------------------------------------------------------------------
// BufMgr::FlushPage
//
// Input    : pid  - page id of a particular page 
// Output   : None
// Purpose  : Flush the page with the given pid to disk.
// Condition: The page with page id = pid must be in the buffer,
//            and is not pinned. pid cannot be INVALID_PAGE.
// PostCond : The page with page id = pid is written to disk if it's dirty. 
//            The frame where the page resides is empty.
// Return   : OK if operation is successful.  FAIL otherwise.
//--------------------------------------------------------------------

//writes page to disk:Status DB::WritePage(PageID pageno, Page* pageptr)

Status BufMgr::FlushPage(PageID pid)
{
    int index = FindFrame(pid);
    if (index == INVALID_FRAME) // page not in the buffer: fail
        return FAIL;
    else{// page in the buffer: ok
        col = index % bufSize;
        row = index / bufSize;
        PageID pageNum = frames[row][col].pid;
        if (frames[row][col].pinCount == 0){
            //not pinned :ok
            if (frames[row][col].IsDirty()){ // dirty: write back
                Status status;
                DB db;
                status = db.WritePage(pageNum,&frames[row][col]);//pointer to page
                if (status == FAIL)
                    return FAIL;
            }
        }
        else{ // pinned : fail
            return FAIL;
        }
        FreeFrame(row,col);
        return OK;
} 

//--------------------------------------------------------------------
// BufMgr::FlushAllPages
//
// Input    : None
// Output   : None
// Purpose  : Flush all pages in this buffer pool to disk.
// Condition: All pages in the buffer pool must not be pinned.
// PostCond : All dirty pages in the buffer pool are written to 
//            disk (even if some pages are pinned). All frames are empty.
// Return   : OK if operation is successful.  FAIL otherwise.
//--------------------------------------------------------------------

Status BufMgr::FlushAllPages()
{
    for (int i = 0 ; i < numOfBuf; i++)
        for (j = 0; j < bufSize; j++){
            if (FlushPage(frames[i][j] == FAIL))
                retrun FAIL;
        }
    return OK;
}


//--------------------------------------------------------------------
// BufMgr::GetNumOfUnpinnedFrames
//
// Input    : None
// Output   : None
// Purpose  : Find out how many unpinned locations are in the buffer
//            pool.
// Condition: None
// PostCond : None
// Return   : The number of unpinned buffers in the buffer pool.
//--------------------------------------------------------------------

unsigned int BufMgr::GetNumOfUnpinnedFrames()
{
    unsigned int cnt = 0;
    for (int i = 0 ; i < numOfBuf; i++)
        for (j = 0; j < bufSize; j++){
            if (frames[i][j].pinCount == 0) //unpinned
                cnt += 1;
        }
    return cnt;
}

void  BufMgr::PrintStat() {
	cout<<"**Buffer Manager Statistics**"<<endl;
	cout<<"Number of Dirty Pages Written to Disk: "<<numDirtyPageWrites<<endl;
	cout<<"Number of Pin Page Requests: "<<totalCall<<endl;
	cout<<"Number of Pin Page Request Misses "<<totalCall-totalHit<<endl;
}

//--------------------------------------------------------------------
// BufMgr::FindFrame
//
// Input    : pid - a page id 
// Output   : None
// Purpose  : Look for the page in the buffer pool, return the frame
//            number if found.
// PreCond  : None
// PostCond : None
// Return   : the frame number if found. INVALID_FRAME otherwise.
//--------------------------------------------------------------------

int BufMgr::FindFrame( PageID pid )
{
	//if in the buffer pool
    for (int i = 0 ; i < numOfBuf; i++)
        for (j = 0; j < bufSize; j++)
            if (frames[i][j].pid == pid){
                return i * numOfBuf + j; // frame number
            }
        }
	//else
    return INVALID_FRAME;
}
