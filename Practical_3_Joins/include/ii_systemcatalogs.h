//
// ii_systemcatalogs.h
//
// C++ Minirel System Catalogs
//
// Adapted by Stephen Harris from ii_systemcatalogs.h written 
// for SHORE Persistent Object System Software.
//


/**********************************************************************
* SHORE Persistent Object System Software
* Copyright (c) 1991 Computer Sciences Department, University of
*                    Wisconsin -- Madison
* All Rights Reserved.
*
* Permission to use, copy, modify and distribute this software and its
* documentation is hereby granted, provided that both the copyright
* notice and this permission notice appear in all copies of the
* software, derivative works or modified versions, and any portions
* thereof, and that both notices appear in supporting documentation.
*
* THE COMPUTER SCIENCES DEPARTMENT OF THE UNIVERSITY OF WISCONSIN --
* MADISON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS" CONDITION.  
* THE DEPARTMENT DISCLAIMS ANY LIABILITY OF ANY KIND FOR ANY DAMAGES
* WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
*
* The SHORE Project Group requests users of this software to return 
* any improvements or extensions that they make to:
*
*   SHORE Project Group 
*     c/o David J. DeWitt and Michael J. Carey
*   Computer Sciences Department
*   University of Wisconsin -- Madison
*   Madison, WI 53706
*
*         or shore@cs.wisc.edu
*
* In addition, the SHORE Project Group requests that users grant the 
* Computer Sciences Department rights to redistribute these changes.
**********************************************************************/
#ifndef II_SYSTEMCATALOGS_H
#define II_SYSTEMCATALOGS_H

#include <string>
#include "boguscatalogs.h"
//#include <std.h>
// #include <dmk_str.h>

class ii_SystemCatalogs {
public:
    virtual ~ii_SystemCatalogs() = 0;

      // This method prints the current catalog in "bogus" format to the given
      // file.
    virtual void OutputCatalog( const char* filename ) = 0;

      // This method creates a new Plan_Relation object and returns it.  (Or it
      // returns a null pointer if there is no such relation.)  It is the
      // caller's responsibility to delete it.
    virtual Plan_Relation* GetPlan_Relation( const string& relName ) = 0;

      // Does the given relation exist?
    virtual bool RelationExists( const string& relName ) = 0;

      // This method creates a new Attribute object and returns it (or null).
      // It is the caller's responsibility to delete it.
    virtual Attribute* GetAttribute( const string& relName, const string& attrName ) = 0;

      // Prints the names of all the relations (in a specialized format) to
      // standard output.
    virtual void PrintRelations() = 0;

      // Print the kind of catalog object ("catalog" or "database"), and the
      // filename associated therewith.
    virtual void PrintFilename() = 0;
};

class ii_BogusSystemCatalogs : public ii_SystemCatalogs
{
    DataBase database;
  
public:
    ii_BogusSystemCatalogs(char* catname)
        : database(catname)
        {}

    ~ii_BogusSystemCatalogs()
        {}
  
    void OutputCatalog(const char* filename)
        {
            database.OutputCatalog( filename );
        }
  
    virtual Plan_Relation* GetPlan_Relation( const string& relName );
    virtual bool RelationExists( const string& relName );
    virtual Attribute* GetAttribute( const string& relName, const string& attrName );
    virtual void PrintRelations();
    virtual void PrintFilename();
};

  // This class uses the MINIBASE_CATALOG object to implement its methods.
class ii_MinibaseSystemCatalogs : public ii_SystemCatalogs
{
public:
    ii_MinibaseSystemCatalogs() {}
    virtual ~ii_MinibaseSystemCatalogs();
    virtual void OutputCatalog( const char* filename );
    virtual Plan_Relation* GetPlan_Relation( const string& relName );
    virtual bool RelationExists( const string& relName );
    virtual Attribute* GetAttribute( const string& relName, const string& attrName );
    virtual void PrintRelations();
    virtual void PrintFilename();
};

extern ii_SystemCatalogs* systemCatalogs;
#endif /* SYSTEMCATALOGS_H */
