#ifndef TUPLE_HDR
#define TUPLE_HDR

#include <assert.h>
#include <memory.h>
#include <string.h>
#include "new_error.h"
#include "minirel.h"

// create an enum for tuple-processing errors
enum tupleErrCodes {

    TUPLE_TOOBIG_ERR,
    TUPLE_TYPE_ERR,
};

class Tuple {

    // Tuples (or Records) will be typeless, i.e. we could not say what is
    // in tuple just by looking at it.
    // This means that whenever the tuples are passed around, additional
    // type information should be given separately.
    // Tuple is able to return stored data provided we ask for 
    // specific type.
    // To create the Tuple you WILL NOT use the constructor but
    // something like this:
    //
    //        Tuple* tuple = (Tuple*) new char[Tuple::max_size];
    //
    // Where int Tuple::max_size is defined GLOBALLY.
    // For error messages see the file TupleError.h
    //
    // -------------------- WARNING ------------------------
    // This class is written using low level pointer arithmetic and
    // if not used correctly may cause lot of trouble.

  public:

    Tuple(){cerr << "Tuple constructor should never be called! \n";};

    int getFld(int fldNo, int& val){   // Convert this field into integer        
            assert((fldNo > 0) && (fldNo <= fldCnt));
            void *adr =  ((char*)this + fldOffset[fldNo - 1]);
            memcpy( &val, adr, sizeof(int));
            return val;
    }
    
    float getFld(int fldNo, float& val){ // Convert this field into float        
            assert((fldNo > 0) && (fldNo <= fldCnt));
            void *adr = ((char*)this + fldOffset[fldNo - 1]);
            memcpy( &val, adr, sizeof(float));
            return val;
    }

    char* getFld(int fldNo, char*& val){ //Convert this field into string        
            assert((fldNo > 0) && (fldNo <= fldCnt));
            val = (char*)this + fldOffset[fldNo - 1];
            return val;
    }

    char* getFld(int fldNo){    // Just return a char* pointer to the data
            assert((fldNo > 0) && (fldNo <= fldCnt));
            return (char*)this + fldOffset[fldNo - 1];
    }

    Tuple *setFld(int fldNo, int val){ // Set this field to integer value
            assert((fldNo > 0) && (fldNo <= fldCnt));
            char* adr = (char*) this + fldOffset[fldNo - 1];
            memcpy(adr,&val,sizeof(int));
            return this;
    }

    Tuple *setFld(int fldNo, float val){ // Set this field to float value
            assert((fldNo > 0) && (fldNo <= fldCnt));
            char* adr = (char*) this + fldOffset[fldNo - 1];
            memcpy(adr,&val,sizeof(float));
            return this;
    }

    Tuple *setFld(int fldNo, char *val){ // Set this field to string value
            assert((fldNo > 0) && (fldNo <= fldCnt));
            char *adr = (char*) this + fldOffset[fldNo - 1];
            assert( strlen(val)+1 <= (unsigned int) max_size );
            strcpy(adr, val);
            return this;
    }

    static int max_size;        // Maximum size of any tuple 

    Status setHdr(short numFlds, AttrType types[], short strSizes[] = 0); 
    
    // setHdr will set the header of this tuple. If there is enough
    // space available returned status will be OK. 
    // Array strSizes contains the sizes of the string (excluding trailing \0)
    // Array types contains the types that will be in this tuple.

    short size(){                // Returns size of this tuple in bytes
            return fldOffset[fldCnt];
    }

    short noOfFlds(){        // Returns number of fields in this tuple
            return fldCnt;
    }
    
    void print(AttrType type[], ostream& out = cout); 

            // Print this tuple to stream out, by default cout; 

  private:

    short fldCnt;                      // Number of fields in this tuple
    short fldOffset[1];                // Array of offsets of the fields

    short pad(short offset, AttrType type);

    // Padding must be used when storing different types.

};

#endif
