#include "../include/frame.h"
/* constructor */
Frame :: Frame(){
    this->pid = -1;
	this->pin_count = 0;
	this->dirty = false;
    this->data = NULL;
    this->referenced = 0; // off
}
/* deconstructor */ //??
Frame :: ~Frame(){
    this->pid = -1;
	this->pin_count = 0;
	this->dirty = false;
    this->data = NULL;
    this->referenced = 0; // off


}
void Frame :: Pin(){ 
	this->pin_count += 1;
}
void Frame :: Unpin(){
	this->pin_count -= 1;
}
void Frame :: EmptyIt(){//?? empty the frame
    this->pid = -1;
    this->pin_count = 0;
    this->dirty = false;
    this->data = NULL;
    this->referenced = 0; // off

}
void Frame :: DirtyIt(){//?? set dirty = 1
    this->dirty = 1;
}
void Frame :: SetPageID(PageID pid){
	this->pid = pid;
}
Bool Frame :: IsDirty(){
	return this->dirty;
}
Bool Frame :: IsValid(){//??
}
Status Frame :: Write(){ // write back to disk??
	
}
Status Frame :: Read(PageID pid){ // if this frame is the target frame??
	if (this->pid == pid)
		return OK;
	else 
		return FAIL;
}
Status Frame :: Free(){ // empty a frame?
	if (this->pin_count == 0 && this->dirty == false){
		this->data = NULL;
		return OK;
	}
	else if(this->pin_count == 0){ // dirty
		this->Write(); // write back to disk 
		this->data = NULL;
		return OK;
	}
	else if(this->pin_count != 0)
		return FAIL;
		
}
Bool Frame :: NotPinned(){
	if (pin_count == 0)
		return true;
	else 
		return false;
}
Bool Frame :: HasPageID(PageID pid){
	if (this->pid == pid)
		return true;
	else 
		return false;
}
PageID Frame ::  GetPageID(){
	return this->pid;
}
Page Frame :: *GetPage(){
	return *this->data;// return  value??
} 
void Frame :: UnsetReferenced(){//unset??
	this->referenced = false;
}
Bool Frame ::  IsReferenced(){
	return referenced;
}
Bool Frame :: IsVictim(){ // was picked or qualified?
	if (pin_count == 0 && referenced == false)
		return true;
	else 
		return false;
}

