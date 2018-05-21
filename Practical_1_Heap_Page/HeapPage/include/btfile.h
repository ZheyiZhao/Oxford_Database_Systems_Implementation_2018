#ifndef _BTFILE_H
#define _BTFILE_H

#include "btindex.h"
#include "btleaf.h"
#include "index.h"
#include "btfilescan.h"
#include "bt.h"

enum PrintOption
{ SINGLE,
  RECURSIVE
};

class BTreeFile: public IndexFile {
	
public:
	
	friend class BTreeFileScan;

    BTreeFile(Status& status, const char *filename);
    BTreeFile(Status& status, const char *filename, const AttrType keytype, const int keysize);
	~BTreeFile();
	
    Status DestroyFile();
	
    Status Insert(const void *key, const RecordID rid); 
    Status Delete(const void *key, const RecordID rid);
    
	IndexFileScan *OpenScan(const void *lowKey = NULL, const void *highKey = NULL);

	Status Search(const void *key,  PageID& foundPid);

	IndexFileScan *OpenSearchScan(const void *lowKey = NULL, const void *highKey = NULL);

	Status PrintTree (PageID pageID, PrintOption option);
	Status PrintWhole ();
	Status DumpStatistics();

private:
	
    struct BTreeHeaderPage 
	{
        PageID root;       
        AttrType keyType; 
        int keySize;              
    };

    BTreeHeaderPage *header;   // (header page)
    PageID           headerID; // page number of header page
    char            *dbname;       // copied from arg of the ctor.
	
	int				totalDataPages;
	int				totalIndexPages;
	float				maxDataFillFactor;
	float				minDataFillFactor;
	float				maxIndexFillFactor;
	float				minIndexFillFactor;
	float				totalFillData; // sum of each data nodes' usedspace/fullpagespace
	float				totalFillIndex;
	int				totalNumIndex; // total num of Index Entries
	int				totalNumData;
	int				hight; // hight of Tree
    Status UpdateHeader (PageID newRoot);
	
	// Insertion
	Status DumpStatistics(PageID);
	Status _DumpStatistics(PageID);
	Status __DumpStatistics(PageID);
    Status _Insert (const void *key, const RecordID, KeyDataEntry **, int *, PageID);
	Status _InsertIntoIndex (const void *, const RecordID, KeyDataEntry **, int *, PageID , BTIndexPage *);
	Status _InsertIntoLeaf (const void *, const RecordID, KeyDataEntry **, int *, PageID , BTLeafPage *);
	Status CreateNewRootAndInsert(const void *key, const RecordID rid);
	Status SplitRoot(KeyDataEntry *entry, int len);

    Status FullDelete(const void *key, const RecordID rid);
    Status NaiveDelete(const void *key, const RecordID rid);
	
	Status _DeleteFromLeaf (const void *key, const RecordID rid, void *&oldChildEntry, 
					 PageID currPid, PageID parentID, BTLeafPage *page);
	Status _DeleteFromIndex (const void *key, const RecordID rid, void *&oldChildEntry, 
					 PageID currPid, PageID parentID, BTIndexPage *page);

	Status _Search( const void *key,  PageID, PageID&);
	Status _SearchIndex (const void *key,  PageID currIndexID, BTIndexPage *currIndex, PageID& foundID);
	Status _PrintTree ( PageID pageID, AttrType attrType);

	Status FindStart (const void *lowkey, BTLeafPage **result, RecordID *prid, PageID foundID);
    Status _Delete (const void *key,
		const RecordID     rid,
		void        *&oldChildEntry,
		PageID        currentPageId,
		PageID        parentPageId);
	Status DeleteFromRoot (BTLeafPage *page, void *&oldEntry);

    Status FindRunStart (const void *lo_key, BTLeafPage **ppage, RecordID *prid);
    Status _DestroyFile (PageID pageno);
};


#endif // _BTFILE_H
