
#ifndef _INDEX_SCAN_H
#define _INDEX_SCAN_H

#include "iterator.h"
#include "heapfile.h"
#include "scan.h"
#include "tuple_utils.h"
#include "projection.h"
#include "pred_eval.h"
#include "index.h"


class IndexScanIter : public Iterator {

  // Index Scan iterator will directly access the required tuple using
  // the provided key. It will also perform selections and projections.

  public:

  FldSpec *perm_mat;

  IndexScanIter(
        IndexType   index,      // Type of the index
        const char *relName,    // Name of the input relation
        const char *indName,    // Name of the input index
        AttrType    types[],    // Array of types in this relation
        short  *str_sizes,      // Array of string sizes
        int     noInFlds,       // Number of fields in input tuple
        int     noOutFlds,      // Number of fields in output tuple
        FldSpec   outFlds[],    // Fields to project
        CondExpr *selects[],    // Conditions to apply, first one is primary
        const int fldNum,
        const int indexOnly,
        Status&   s
  );

  Status GetNext(Tuple*& tuple);

 ~IndexScanIter();

  private:

//    IndexFileScan* StatHash_scan(CondExpr* selects[], IndexFile* indFile);
//    void* IndexScanIter::getValue(CondExpr* cd, AttrType type, int choice);

    IndexFile     *indFile;
    IndexFileScan *indScan;
    AttrType      *_types;
    CondExpr     **_selects;
    int            _noInFlds;
    int            _noOutFlds;
    HeapFile      *f;
    Tuple         *tuple1;
    Tuple         *Jtuple;
    int            t1_size;
    int            _fldNum;
    int            index_only;
};

class IndexUtils {

  public:

    IndexFileScan *StatHash_scan(CondExpr **selects, IndexFile *indFile);

  private:
    void *getValue(CondExpr* cd, AttrType type, int choice);
};

#endif // _INDEX_SCAN_H
