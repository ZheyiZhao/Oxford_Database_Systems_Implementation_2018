// -*- C++ -*-
#ifndef _EXT_SYS_DEFS_H
#define _EXT_SYS_DEFS_H
/////////////////////////////////////////////////////////////////
//
// filename : ext_sys_defs.h    Luke Blanshard, April 12, 1996
//
// This extends the basic system globals to include the catalog
// objects.
//
//
/////////////////////////////////////////////////////////////////

#include "system_defs.h"

#define MINIBASE_CATALOGPTR     (minibase_globals->GlobalCatalogPtr)
#define MINIBASE_ATTRCAT        (MINIBASE_CATALOGPTR->getAttrCat())
#define MINIBASE_RELCAT         (MINIBASE_CATALOGPTR->getRelCat())
#define MINIBASE_INDCAT         (MINIBASE_CATALOGPTR->getIndCat())


class ExtendedSystemDefs : public SystemDefs {
  /* This class actually allocates the global catalog pointer.  Use this class
     rather than SystemDefs when you need to use the catalog system. */

  public:
    ExtendedSystemDefs( Status& status, const char* dbname, unsigned dbpages =0,
                        unsigned bufpoolsize =0,
                        const char* replacement_policy =0 );

    ExtendedSystemDefs( Status& status, const char* dbname, const char* logname,
                        unsigned dbpages, unsigned maxlogsize,
                        unsigned bufpoolsize =0,
                        const char* replacement_policy =0 );


    virtual ~ExtendedSystemDefs();

  private:
    void init( Status& status, bool initCatalog );
};


#endif // _SYSTEM_DEFS_H
