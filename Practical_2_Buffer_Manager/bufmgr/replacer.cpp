#include "../include/replacer.h"
//always find an available candidate?

// choose a page that has pin_count = 0 & referenced = off
			//if pin_count > 0
			// 	if referenced = on -> turn off
			// 	if referenced = off -> do nothing 
			// if pin_count = 0
			// 	if referenced = off -> chosen -> write back if modified
			// if current increases until repeats -> no available candidate

int Clock::PickVictim(){
	// assume ref = on when pin_count = 0
// CLOCK policy
    int index = -1;
	for (int current = 0;current < numOfBuf; current ++)// rows
        for(int j = 0; j < bufSize; j++){
            cur_pin_count = frames[current][j].pinCount;
            cur_ref = frames[current][j].referenced;
            cur_dirty = frames[current][j].dirty;
            
            if (cur_pin_count > 0){
                if (cur_ref == on){
                    frames[current][j].referenced = off; //turn off
                }
            }
            else{ //cur_pin_count == 0
                if (cur_ref == off){ // chosen
                    index = current * bufSize + j; // sequential index
                    if (cur_dirty == 1){ // check dirty
                        //writes page to disk:Status DB::WritePage(PageID pageno, Page* pageptr)
                        DB db;
                        PageID pid =frames[current][j].pid;
                        db.WritePage(pid,&frames[current][j]);
                    }
                }
            }
        }
    //if (index == -1)??
    
    
    
    return index;
}
