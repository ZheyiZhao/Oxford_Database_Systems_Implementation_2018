/*
 * key.h - global declarations for the Static Hash
 *
 */ 

#ifndef _KEY_H
#define _KEY_H

/*
 * Essentially we provide a way for those classes to package and 
 * unpackage <key,data> pairs in a space-efficient manner.  That is, the 
 * packaged result contains the key followed immediately by the data value;
 * no padding bytes are used (not even for alignment). 
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


/*
 * MAX_KEY_SIZE1: Maximum key size in bytes (for sizing 
 * (MAX_KEY_SIZE seems to be defined somewhere else ??)
 * temporary buffers on the stack).
 */

#define MAX_KEY_SIZE1        20


/*
 * Keytype union: used to discover the max keysize, and for minimal 
 * static type checking in key package/unpackage functions (see below).
 */

union Keytype
{
  int  intkey;
  char charkey[MAX_KEY_SIZE1];
};


/*
 * Datatype union: used to discover the max data size, and for minimal 
 * static type checking in key package/unpackage functions (see below).
 */

union Datatype
{
  PageID      pageNo;
  RecordID         rid;
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
  RecordID        rid;
  Keytype    key;
};

/*
 * Finally, here is the interface to our <key,rid> abstraction.
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
 * unpacks the <key,rid> values from it; those are written to *targetkey
 * and *targetdata, respectively.
 *   - key1  < key2 : negative
 *   - key1 == key2 : 0
 *   - key1  > key2 : positive
 *
 */

int keyCompare(const void *key1, const void *key2, AttrType t);
                   
/*
 * make_entry: write a <key,rid> pair to a blob of memory (*target) big
 * enough to hold it.
 *
 */

void make_entry(KeyDataEntry *target,
                AttrType key_type,
                const void *key, const RecordID& rid);

/*
 * get_key_info: unpack a <key,rid> pair into pointers to respective parts.
 */

void get_key_info(KeyDataEntry *dst, KeyDataEntry *src,
                                     const AttrType key_type);

int get_key_length(const void *key, const AttrType key_type);

int hash_key_data(const void *key, const AttrType key_type);
                   
#endif  // _KEY_H
