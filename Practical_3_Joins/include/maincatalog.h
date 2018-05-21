//-------------------------------------
//maincatalog.h
// 
// P. Feakins May 1995
//------------------------------------

#ifndef MAINCATALOG_H
#define MAINCATALOG_H


#include "catalog.h"
#include "new_error.h"


int menu();

class Catalog
{
    friend class RelCatalog;
    friend class AttrCatalog;
    friend class IndexCatalog;   

  public:

      // open relation catalog (invokes constructors for each)
    Catalog(Status &status);
 
      // get rid of catalog (invokes destructors for each catalog)
    ~Catalog();

      // get catalog entry for a relation
    Status getRelationInfo(const char* relation, RelDesc& record);

      // create a new relation
    Status createRel(const char* relation, int attrCnt, attrInfo attrList[]);

      // destroy a relation
    Status destroyRel(const char* relation);

      // add a index to a relation
    Status addIndex(const char *relation, const char *attrname,
                    IndexType accessType, int buckets);

      // drop an index from a relation
    Status dropIndex(const char *relation, const char *attrname,
                     IndexType accessType);

      // get a catalog entry for an attribute
    Status getAttributeInfo(const char *relation, const char *attrName,
                            AttrDesc &record);

      // get catalog entries for all attributes of a relation
    Status getRelAttributes(const char *relation, int& attrCnt,
                            AttrDesc*& attrs);
  
      // get catalog entries for all indexes for a relation
    Status getRelIndexes(const char* relation, int &indexCnt,
                         IndexDesc *&indexes);

      // get catalog entries for all indexes for an attribute 
    Status getAttrIndexes(const char* relation, const char *attrName,
                          int &indexCnt, IndexDesc *&indexes);
 
      // get catalog entry on an index
    Status getIndexInfo(const char* relation, const char* attrName,
                        IndexType accessType, IndexDesc &record);

      // dump catalogs to a disk file for optimizer
    Status dumpCatalog(const char* filename);

      // Runs stats on the database...
    Status runStats(const char* filename);

    Status listRelations();

    Status initialize();

    IndexCatalog* getIndCat()
        { return indCat; }
    RelCatalog* getRelCat()
        { return relCat; }
    AttrCatalog* getAttrCat()
        { return attrCat; }


      // Error codes for catalog
    enum {
        MISSING_PARAM,
        NO_MEM,
        ATTR_NOT_FOUND,
        REL_NOT_FOUND,
        INDEX_EXISTS,
        REL_NAME_TOO_LONG,
        REL_EXISTS,
        DUP_ATTRS,
        TUPLE_TOO_BIG,
        ATTR_NAME_TOO_LONG,
        INDEX_NOT_FOUND,
        INDEX_NAME_TOO_LONG,
        INPUT_OPEN,
        BAD_RECORD_SIZE,
        BAD_ATTR_COUNT,
        BAD_TYPE,
        NO_CATALOG,
    };

  private:
    IndexCatalog *indCat;
    RelCatalog   *relCat;
    AttrCatalog  *attrCat;
};

#endif
