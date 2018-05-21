/* -*- C++ -*- */
/*
* bt.h - global declarations for the B+ Tree.
*
* Gideon Glass & Johannes Gehrke  951012  CS564  UW-Madison
*/ 

#ifndef BT_H
#define BT_H

/*
* This file contains, among other things, the interface to our
* key (void*) abstraction.  The BTLeafPage and BTIndexPage code
* know very little about the various key types.  In fact, only 
* a very small amount of work would be required to make those 
* classes share almost all of their code (e.g., by creating a class
* `BTPage' derived from SortedPage and from which BTLeafPage and 
* BTIndexPage themselves drive).  
*
* Essentially we provide a way for those classes to package and 
* unpackage <key,data> pairs in a space-efficient manner.  That is, the 
* packaged result contains the key followed immediately by the data value; no 
* padding bytes are used (not even for alignment). 
*
* Furthermore, the BT<*>Page classes need
* not know anything about the possible AttrType values, since all 
* their processing of <key,data> pairs is done by the functions 
* declared here.
*
* In addition to packaging/unpacking of <key,value> pairs, we 
* provide a keyCompare function for the (obvious) purpose of
* comparing keys of arbitrary type (as long as they are defined 
* here).
*/

#include "minirel.h"
#include "foo.h"

typedef enum 
{
	INDEX_NODE,
	LEAF_NODE		
} NodeType;


/*
* A bunch of macros for handling and returning errors.
*
* Note that some of these macros effect immediate returns from the current
* function; some return the specified error; the *_CTOR ones assign
* the error to an argument (these are for use in returnvalue-less 
* constructors).
*/

/*
 * MAX_KEY_SIZE1: Maximum key size in bytes 
 */

#define MAX_KEY_SIZE        220

#define ATTR_INT  attrInteger
#define ATTR_STRING attrString
#define ATTR_FOO	attrFoo
/*
 * Keytype union: used to discover the max keysize, and for minimal 
 * static type checking in key package/unpackage functions (see below).
 */



union KeyType
{
	int intKey;
	char charKey[MAX_KEY_SIZE];
	Foo  fooKey;

};
/*
* Datatype union: used to discover the max data size, and for minimal 
* static type checking in key package/unpackage functions (see below).
*/

union DataType
{
	PageID      pid;  // for index page entries
	RecordID    rid;  // for leaf page entries    
};


/*
* struct KeyDataEntry:
*
* The BT*Page code only knows about the following type (by name).
* It never refers to the listed members; rather, it simply instantiates
* an object so that it has enough storage to copy into and out of a record
* the necessary number of <key, data> bytes (for whatever key and data
* types it is interested in -- for key types, it doesn't care).  
*/

struct KeyDataEntry
{
	KeyType    key;
	DataType   data;
};

/*
* Finally, here is the interface to our <key,data> abstraction.
* 
* keyCompare simply compares keys (types must be the same); return 
* value is < 0, 0, or > 0.
*
* make_entry packages a key and a data value into a chunk of memory 
* large enough to hold it (the first parameter).  Note that the 
* resultant KeyDataEntry cannot be accessed by its members because
* the Datatype member may start after the actual beginning of the
* data value stored here.  The real length of the resulting <key,data>
* pair is returned in *pentry_len.
*
* get_key_data takes a KeyDataEntry chunk and its real length and 
* unpacks the <key,data> values from it; those are written to *targetkey
* and *targetdata, respectively.
*   - key1  < key2 : negative
*   - key1 == key2 : 0
*   - key1  > key2 : positive
*
* Finally, get_key_length and get_key_data_length determine the 
* storage required for given key and key+data. 
*/

int KeyCmp(const void *key1, const void *key2, AttrType t);
int GetKeyLength(const void *key, const AttrType keyType);
int GetKeyDataLength(const void *key, const AttrType keyType, 
					 const NodeType nodeType);
void MakeEntry (KeyDataEntry *target, AttrType keyType, const void *key,
                NodeType nodeType, DataType data,int *len);
void GetKeyData (void *key, DataType *data, KeyDataEntry *pair, int len, NodeType nodeType);

#define INSERT(page, key, type, data, rid) {\
	if ((page)->Insert((void *)key, type, data, rid) != OK) {\
		cerr << "Unable to insert in " << __FILE__ << ":" << __LINE__;return FAIL;}}
#define DELETE(page, key, type, rid) {\
	if ((page)->Delete(key, type, rid) != OK) {\
		cerr << "Unable to delete in " << __FILE__ << ":" << __LINE__;return FAIL;}}

#endif
