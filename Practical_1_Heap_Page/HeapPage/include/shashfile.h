/*
 * StatHashFile.h - definition of class StatHashFile : public IndexFile
 *
 */

#ifndef _STAT_HASH_FILE_H
#define _STAT_HASH_FILE_H

/*
 * This is the main definition of the class StatHashFile, which derives from 
 * the abstract base class IndexFile.
 *
 * class StatHashFileScan is defined in shashfilescan.h; this class provides
 * a search/iterate interface to a StatHashFile.
 */

#include "index.h"
#include "key.h"

class StatHashFileScan;

     // The size of the hash table (prime)
#define   HTAB_SZE    (233)

class StatHashFile: public IndexFile {

  friend class StatHashFileScan;

  /*
   * Structure of a Static Hash index header page.
   */

  private:

    struct StatHashHeaderPage {
        unsigned long magic0; // magic number for sanity checking
    
        AttrType key_type;    // type of keys in tree
        int      keysize;     // max key length (specified at index creation)

        PageID htab[HTAB_SZE];  // The static hash table.
    };

  public:

    enum ErrorTypes { 

      // interface for the error protocol.

      _OK = 0,                // 0 is always OK
      CANT_FIND_HEADER,       // tried to open index but db said no header
      CANT_PIN_HEADER,        // buffer manager failed to pin header page
      CANT_ALLOC_HEADER,      // failed to allocate block for header page
      CANT_ADD_FILE_ENTRY,    // couldn't register new index file w/ db
      CANT_UNPIN_HEADER,      // can't unpin header page
      CANT_PIN_PAGE,          // can't pin some page
      CANT_UNPIN_PAGE,        // can't unpin some page
      INVALID_SCAN,           // attempt to use bad Scan object 
      DELETE_CURRENT_FAILED,  // failed to delete current rid in scan
      CANT_DELETE_FILE_ENTRY, // db failed to delete file entry
      CANT_FREE_PAGE,         // buffer manager failed to free a page
      KEY_TOO_LONG,           // the key given in insert is too long
      INSERT_FAILED,          // insert not successful
      DELETE_DATAENTRY_FAILED,// could not delete a data entry 
      DATA_ENTRY_NOT_FOUND,   // could not find data entry to delete
      CANT_GET_PAGE_NO,       // get_page_no on SHIndexPage failed
      CANT_ALLOCATE_NEW_PAGE, // bm::newPage failed
  
      NR_ERRORS               // and this is the number of them
    }; 
  
    static const char *Errors[NR_ERRORS];

    // an index with given filename should already exist; this opens it.
    StatHashFile(Status& status, const char *filename);
  
    // if index exists, open it; else create it.
    StatHashFile(Status& status, const char *filename,
                 const AttrType keytype, const int keysize);
    
    // closes index
   ~StatHashFile();
    
    // destroy entire index file
    Status DestroyFile();
    
    // insert recid with the key 'key'
    Status Insert(const void *key, const RecordID rid);
    
    // delete leaf entry recid given its key
    // (`rid' is IN the data entry; it is not the id of the data entry)
    Status Delete(const void *key, const RecordID rid);
    
    // create a scan with given keys
    // Cases:
    //      (1) lo_key = NULL, hi_key = NULL
    //              scan the whole index
    //      (2) lo_key = NULL, hi_key != NULL
    //              range scan from min to the hi_key
    //      (3) lo_key != NULL, hi_key = NULL
    //              range scan from the lo_key to max
    //      (4) lo_key != NULL, hi_key != NULL, lo_key = hi_key
    //              exact match ( might not be unique)
    //      (5) lo_key != NULL, hi_key != NULL, lo_key < hi_key
    //              range scan from lo_key to hi_key
    IndexFileScan *OpenScan(const void *lo_key = NULL,
                            const void *hi_key = NULL);

    int KeySize();
    

    void PrintHeader();            // print the header info
    void PrintPage(PageID id);     // print page with id

  private:

    StatHashHeaderPage *headerPage;   // (header page)
    PageID              headerPageId; // page number of header page
    char               *dbname;       // copied from arg of the ctor.

};

#endif // _STAT_HASH_FILE_H
