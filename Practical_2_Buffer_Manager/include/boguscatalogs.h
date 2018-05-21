//
// boguscatalogs.h
//
// C++ Minirel System Catalogs
//
// Adapted by Stephen Harris from boguscatalogs.h written 
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
#ifndef BOGUSCATALOGS_H
#define BOGUSCATALOGS_H

#include <fstream>
#include <string>
#include "query.h"

class DataBase {
public:
  int numberofrelations;
  Plan_RelationList relations;
  string filename;

  DataBase(string filename);
  void OutputCatalog(const char* databasename);
  
  ~DataBase();
  
private:
  Plan_Relation *ReadPlan_Relation(ifstream &infile);
  Attribute *ReadAttribute(ifstream &infile, Plan_Relation *relation);
  Plan_AccessMethod *ReadPlan_AccessMethod(ifstream &infile, Attribute *attribute);
}; 

#endif /* BOGUSCATALOGS_H */




