//------------------------------------
// catalog.h
//
// P. Feakins May 1995
//-------------------------------------

#ifndef CATALOG_H
#define CATALOG_H

#include <string.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <ostream>

#include "scan.h"
#include "heapfile.h"
#include "index.h"
#include "minirel.h"
#include "ext_sys_defs.h"
#include "tuple.h"
#include "new_error.h"
#include "shashfile.h"

#define strcasecmp _stricmp

// forward declarations
class Catalog;
class AttrCatalog;
class IndexCatalog;

#define RELCATNAME      "relcat"          // name of relation catalog
#define ATTRCATNAME     "attrcat"         // name of attribute catalog
#define INDEXCATNAME    "indcat"
#define RELNAME         "relname"         // name of indexed field in rel/attrcat
#define MAXNAME         32                // length of relName, attrName
#define MAXSTRINGLEN    255               // max. length of string attribute
#define NUMTUPLESFILE   100               // default statistic = no recs in file
#define NUMPAGESFILE    20                // default statistic = no pages in file
#define DISTINCTKEYS    20                // default statistic: no of distinct keys
#define INDEXPAGES      5                 // default statisitc no of index pages
#define MINSTRINGVAL    "A"               // default statisitic
#define MAXSTRINGVAL    "ZZZZZZZ"         // default statisitic
#define MINNUMVAL       0                 // default statistic
#define MAXNUMVAL       999999999         // default statisitic


// attrData struct for minimum and maximum attribute values
struct attrData
{
    char  strVal[10];
    int   intVal;
    float floatVal;
};


//   RelDesc struct: schema of relation catalog:
struct RelDesc
{
    char relName[MAXNAME];        // relation name
    int  attrCnt;                 // number of attributes
    int  indexCnt;                // number of indexed attrs
    int  numTuples;               // number of tuples in the relation
    int  numPages;                // number of pages in the file
};


// attrInfo struct used for creating relations
struct attrInfo
{
    char     attrName[MAXNAME];       // attribute name
    AttrType attrType;                // INTEGER, FLOAT, or STRING
    int      attrLen;                 // length
}; 


// AttrDesc struct : schema of attribute catalog:
struct AttrDesc
{
    char     relName[MAXNAME];            // relation name
    char     attrName[MAXNAME];           // attribute name
    int      attrOffset;                  // attribute offset
    int      attrPos;                     // attribute position 
    AttrType attrType;                    // attribute type
    int      attrLen;                     // attribute length
    int      indexCnt;                    // number of indexes
    attrData minVal;                      // min max key values
    attrData maxVal;
};


// IndexDesc Struct : schema for index catalog  
struct IndexDesc
{
    char       relName[MAXNAME];          // relation name
    char       attrName[MAXNAME];         // attribute name
    IndexType  accessType;                // access method 
    TupleOrder order;                     // order of keys   
    int        clustered;                 //  
    int        distinctKeys;              // no of distinct key values 
    int        indexPages;                // no of index pages 
};

class RelCatalog : public HeapFile
{
    friend class Catalog;
    friend class AttrCatalog;
    friend class IndexCatalog;

      // Helps runStats
    Status genStats(RelDesc &relRec, AttrDesc *&attrRecs);

public:
      // CONSTRUCTOR
    RelCatalog(Status &status);

      // GET RELATION DESCRIPTION FOR A RELATION
    Status getInfo(const char *relation, RelDesc& record);

      // DESTRUCTOR
    ~RelCatalog();

// private:
      // CREATE A NEW RELATION
    Status createRel(const char *relation, int attrCnt, attrInfo attrList[]);

      // DESTROY A RELATION
    Status destroyRel(const char *relation);

      // ADD AN INDEX TO A RELATION
    Status addIndex(const char *relation, const char *attrname,
                    IndexType accessType, int buckets = 1);

      // DROP AN INDEX FROM A RELATION
    Status dropIndex(const char *relation, const char *attrname,
                     IndexType accessType);

      // DUMPS A CATALOG TO A DISK FILE (FOR OPTIMIZER)
    Status dumpCatalog(const char *filename);

      // Collects stats from all the tables of the database.
    Status runStats(const char *filename);

      // OUTPUTS A RELATION TO DISK FOR OPTIMIZER
    Status dumpRelation(fstream &outFile, RelDesc &relRec, int tupleSize); 

      // OUTPUTS ATTRIBUTES TO DISK FOR OPTIMIZER (used by runstats)
    Status rsdumpRelAttributes (fstream &outFile,AttrDesc *&attrRecs,
            int attrCnt, const char *relName);

      // OUTPUTS ATTRIBUTES TO DISK FOR OPTIMIZER
    Status dumpRelAttributes (fstream &outFile, AttrDesc *&attrRecs,
            int attrCnt);

      // OUTPUTS ACCESS METHODS TO DISK FOR OPTIMIZER
    Status dumpRelIndex(fstream &outFile,IndexDesc *&indexRecs,
                        int indexCnt, int attrsize);

      // ADD INFORMATION ON A RELATION TO  CATALOG
    Status addInfo(RelDesc record);

      // REMOVE INFORMATION ON A RELATION FROM CATALOG
    Status removeInfo(const char *relation);

      // Converts AttrDesc to tuple.
    Status make_tuple(Tuple *tuple, RelDesc record);

    Status read_tuple(Tuple *tuple, RelDesc& record);



    Tuple *tuple;
    short *str_sizes;
    AttrType *attrs;

};



class AttrCatalog : public HeapFile
{
    friend class Catalog;
    friend class RelCatalog;
    friend class IndexCatalog;

public:
      // OPEN ATTRIBUTE CATALOG
    AttrCatalog(Status &status);

      // GET ATTRIBUTE DESCRIPTION
    Status getInfo(const char *relation, const char *attrName,
                   AttrDesc &record);

      // GET ALL ATTRIBUTES OF A RELATION
    Status getRelInfo(const char *relation, int &attrCnt, AttrDesc *&Attrs);

      // RETURNS ATTRTYPE AND STRINGSIZE ARRAYS FOR CONSTRUCTING TUPLES
    Status getTupleStructure(const char *relation, int &attrCnt,
                    AttrType *&typeArray, short *&sizeArray);
  
      // CLOSE ATTRIBUTE CATALOG
    ~AttrCatalog();
  
private:
      // ADD ATTRIBUTE ENTRY TO CATALOG
    Status addInfo(AttrDesc record);

      // REMOVE AN ATTRIBUTE ENTRY FROM CATALOG
    Status removeInfo(const char *relation, const char *attrName);

      // REMOVE ALL ATTRIBUTE ENTRIES FOR A RELATION
    Status dropRelation(const char *relation);

      // ADD AN INDEX TO A RELATION
    Status addIndex(const char *relation, const char *attrname,
                    IndexType accessType);

      // Converts AttrDesc to tuple. 
    Status make_tuple(Tuple *tuple, AttrDesc record);

    Status read_tuple(Tuple *tuple, AttrDesc& record);



    Tuple *tuple;
    short *str_sizes;
    AttrType *attrs;
    int max;
};


class IndexCatalog : public HeapFile
{
    friend class Catalog;
    friend class RelCatalog;
    friend class AttrCatalog;

public:
      // OPEN INDEX CATALOG
    IndexCatalog(Status &status);

      // GET ALL INDEXES FOR A RELATION
    Status getRelInfo(const char *relation, int &indexCnt, IndexDesc *&indexes);

      // RETURN INFO ON AN INDEX
    Status getInfo(const char *relation, const char *attrName,
                   IndexType accessType, IndexDesc &record);

      // GET ALL INDEXES INLUDING A SPECIFIED ATTRIBUTE
    Status getAttrIndexes(const char *relation,
                const char *attrName, int &indexCnt, IndexDesc *&indexes);

      // CREATES A FILE NAME FOR AN INDEX 
    Status buildIndexName(const char *relation, const char *attrName,
                          IndexType accessType, char *&indexName);

      // DESTRUCTOR
    ~IndexCatalog();

private:
      // ADD INDEX ENTRY TO CATALOG
    Status addInfo(IndexDesc record);

      // REMOVE INDEX ENTRY FROM CATALOG
    Status removeInfo(const char *relation, const char *attrName,
                      IndexType accessType);

      // ADD INDEX TO A RELATION
    Status addIndex(const char *relation, const char *attrName,
                    IndexType accessType, int buckets = 1);

      // DROP INDEX FROM A RELATION
    Status dropIndex(const char *relation, const char *attrName,
                     IndexType accessType);

      // DROP ALL INDEXES FOR A RELATION
    Status dropRelation(const char *relation);


    Status make_tuple(Tuple *tuple, IndexDesc record);

    Status read_tuple(Tuple *tuple, IndexDesc& record);



    Tuple *tuple;
    short *str_sizes;
    AttrType *attrs;

};

#endif
