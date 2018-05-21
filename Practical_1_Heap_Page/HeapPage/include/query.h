//
// query.h
//

/* Stephen Harris    4/1/94
 *
 * This file contains all of the headers for the classes contained in and
 * including the class State.  The classes in this file are the classes that
 * are shared between the parser interface and the optimizer.  These classes
 * along with the classes declared in planner.h are also shared between
 * the optimizer and the lower levels of the system.  Below is a list of
 * the classes and data types declared in this file.  See the declarations
 * themselves for more detailed descriptions.  Note that some classes have
 * the prefix "Plan_".  The reason for this is to remove name confilicts
 * with other parts of the system.
 *
 * Classes and Types (in order of declaration):
 *
 *      AttrType                 attribute type
 *
 *      AttrOperator             attribute operators (<, >, <=, >=, ==, !=)
 *
 *      LogicalOperator          logical operators (and, or, not)
 *
 *      TupleOrder               tuple orderings (ascending, descending, random)
 *
 *      PropertyNode             stores a miscellaneous property associated
 *                               with a relation
 *
 *      PropertyList             stores a list of PropertyNodes
 *
 *      AttrValue                stores either a CString, integer, or real
 *
 *      Index                    stores data about an index
 *
 *      Attribute                stores information that describes an
 *                               attribute associated with a relation
 *
 *      AttributeList            stores a list of attributes
 *
 *      Plan_AccessMethod        stores information about specific access
 *                               methods for retrieving a relation from disk
 *
 *      Plan_AccessMethodList    stores a list of Plan_AccessMethods
 *
 *      Plan_Relation            stores information about a relation
 *
 *      Plan_RelationList        stores a list of Plan_Relations
 *
 *      SelectOperand            stores either a pointer to an attribute or
 *                               an instance of AttrValue, used exclusively
 *                               by the classes associated with the Select
 *                               class
 *
 *      SelectElem               stores an operator of type AttrOperator and
 *                               pointers to two instances of SelectOperand,
 *                               also used exclusively by classes associated
 *                               with the Select class
 *
 *      SelectTerm               stores an operator of type LogicalOperator
 *                               and pointers to two instances of SelectExpr
 *
 *      SelectExpr               stores a pointer to either an instance of
 *                               SelectElem or SelectTerm
 *
 *      Select                   stores a pointer to an instance of SelectExpr
 *                               and contains member functions to retrieve
 *                               information associated with it
 *
 *      State                    stores a list of relations to be joined, a
 *                               list of selects to perform, and a list of
 *                               attributes to project on
 */

#ifndef QUERY_H
#define QUERY_H

// #define _STANDALONE_
//


#include <string>
#include "da_types.h"
#include "ndlist.h"

#ifndef _STANDALONE_
#include "minirel.h"
#endif

#ifndef _MAX
#define _MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef _MIN
#define _MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifdef _STANDALONE_
enum AttrType { attrString, attrInteger, attrReal, attrNull };
enum SelectType { selRange, selExact, selBoth, selUndefined };
enum AttrOperator { aopEQ, aopLT, aopGT, aopNE, aopLE,
                    aopGE, aopNOT, aopNOP, aopRANGE };
enum LogicalOperator { lopAND, lopOR, lopNOT };
enum TupleOrder { Ascending, Descending, Random };
#endif

// forward references
class Plan_Relation;
class Attribute;
class Index;
class Select;


/* PropertyNode stores a pointer and a name that describes what the pointer
 * is pointing to.  PropertyList is simply a list of PropertyNodes.  This
 * can be used to store properties of a relation that are currently
 * unforseen.  As a result, at this time (4/1/94) PropertyNode and
 * PropertyList are not used.
*/

class PropertyNode {

  public:
    string name;
    void *data;

    int operator==(const PropertyNode & pn)
      { return ( (name.compare(pn.name) == 0) && data == pn.data); };
};

typedef List<PropertyNode> PropertyList;


/* The AttrValue class stores the value of an attribute; either a character
 * CString, an integer, or a real.  The class Select (declared in this file)
 * uses instances of AttrValue in its representation of select predicates.
 * The Attribute class (also declared in this file) also uses AttrValue to
 * to store minimum and maximum values.
 *
 * For example:     Sailor.Name = "Jones"        "Jones" would be
 *                                             stored using AttrValue
 *
 * Members are:
 *
 *      type                     stores the type of the value being stored
 *
 *      s                        stores a CString if type is attrCString
 *
 *      i                        stores an integer if type is attrInteger
 *
 *      r                        stores a real if type is attrReal
 *
 *      =                        overloaded assignment operator
 *
 *      GetCString                returns s
 *
 *      GetInteger               returns i
 *
 *      GetReal                  returns r
 *
 *      Type                     returns type
 *
 *      ==, !=, <=, >, >=, <     comparison operators
 *
 *      <<                       ostream operator
 *
 *      MakeCString               returns a CString representation of AttrValue;
 *                               used for display purposes
 *
 *      Read                     for reading an AttrValue from an istream
 */
class AttrValue {

  private:
    AttrType type;
    string s;    // left out of the union because the compiler didn't like it
    union {
        long i;
        double r;
    };

  public:
    AttrValue();
    AttrValue(const AttrValue & av);

    // assignment methods
    string operator =(const string & sVal);
    long operator =(long iVal);
    int operator =(int iVal);

    double operator =(double rVal);
    AttrValue operator =(const AttrValue & av);

    // retrieval methods
    string GetString();
    long GetInteger();
    double GetReal() ;


    AttrType Type() const { return type; };

    // comparison operators
    bool operator ==(const AttrValue & av);
    bool operator !=(const AttrValue & av) {return !(*this == av); };
    bool operator <=(const AttrValue & av);
    bool operator >(const AttrValue & av) {return !(*this <= av); };
    bool operator >=(const AttrValue & av);
    bool operator <(const AttrValue & av) {return !(*this >= av); };

    friend ostream & operator << (ostream & s, const AttrValue & av);
    string MakeString();

    void Read(istream & s, AttrType type);
};




/* The Attribute class stores information that describes an attribute.
 * It contains :
 *
 *      name                     the name of the attribute
 *
 *      attrID                   attribute ID
 *
 *      relation                 a pointer to the Plan_Relation that it
 *                               belongs to
 *
 *      type                     the type of the attribute
 *
 *      size                     the size of the field
 *
 *      offset                   the offset from the start of a tuple
 *
 *      Name()                   returns name
 *
 *      FullName()               returns the name of the relation followed
 *                               by a dot, followed by the name of the
 *                               attribute
 *
 *      AttrID()                 returns the attribute ID
 *
 *      *RelationPtr()           returns the member variable called relation
 *
 *      Type()                   returns the type of the attribute
 *
 *      Size()                   returns the size member variable
 *
 *      Offset()                 returns the offset member variable
 *
 *      additionalProperties     a property list for storing unforseen
 *                               properties, probably not used
 *
 *      FindIndex                returns a pointer to an index on the
 *                               attribute if there is one, NULL otherwise
 *
 *      minVal, maxVal           minimum and maximum values
 *
 *      ==, !=                   comparison operators (comparison based
 *                               on name)
 */
class Attribute {

  public:
    friend void AlterAttribute();

    enum AggregateType { NONE, COUNT, AVG, MAX, MIN, SUM };

  protected:
    string name;
    int attrId;

    Plan_Relation *relation;
    AttrType type;

    int size;
    int offset;
    AggregateType aggregate;
    bool aggDistinct;

  public:
    AttrValue minVal;
    AttrValue maxVal;

/*
    PropertyList additionalProperties;
 */

    // methods
  public:
    Attribute() {}
    Attribute(const string& name) : name(name), attrId(0),
            relation(NULL), type(attrNull), size(0), offset(0), aggregate(NONE),
            aggDistinct(false) {}
         // Used during the read of multikey
         // access methods; rest to be filled out later;

    Attribute(const Attribute & a);   // copy constructor
    Attribute(const string & iName, Plan_Relation *iRelation, AttrType iType,
              int iSize, int iOffset, int iAttrID);
   ~Attribute() {
        // additionalProperties.Clean();
    }

    string Name();
    int AttrID();

    string FullName();
    Plan_Relation *RelationPtr();
    void SetRelationPtr(Plan_Relation *iRelation);
    AttrType Type();
    int Size();
    int Offset();

    Index *FindIndex();

    bool operator == (const Attribute & a);
    bool operator != (const Attribute & a);

    bool AggDistinct();
    AggregateType MakeAgg( AggregateType type, bool iaggDistinct );
    AggregateType GetAgg();

};

typedef List<Attribute> AttributeList;

struct OrderAttr
{
    Attribute* attr;
    TupleOrder tupleOrder;
    bool operator==(const OrderAttr &a)
        {return (attr == a.attr) && (tupleOrder == a.tupleOrder); } ;
    bool operator!=(const OrderAttr &a) { return ! ( *this == a ); };
};

typedef List<OrderAttr> OrderByList;

/* The Plan_AccessMethod class stores information associated with a particular
 * method for retrieving a relation from disk.  Plan_AccessMethodList maintains
 * a list of Plan_AccessMethods. Each Plan_Relation has a list of access methods
 * associated with it.
 *
 * The Plan_AccessMethod class contains:
 *
 *      nodeID                   arbitrary ID assigned to the access method
 *                               used only for display purposes
 *
 *      name                     the name (description) of the access method
 *
 *      cost                     the cost of retrieving the relation it
 *                               belongs to
 *
 *      tupleOrder               the order that the tuples are retrieved in
 *                               (ascending, descending, or random)
 *
 *      attribute                a pointer to the attribute
 *
 *      index                    a pointer to a description of an index if
 *                               the access method is an index, NULL otherwise
 *
 *      Plan_AccessMethod()      default constructor
 *
 *      Plan_AccessMethod(const CString & iName, const double & iCost,
 *                        TupleOrder iTupleOrder, Attribute *iAttribute,
 *                        const Index *iIndex)
 *                                                 constructor for setting all
 *                                                 of the member variables
 *
 *      Plan_AccessMethod(const Plan_AccessMethod & am)
 *                               copy constructor
 *
 *      SetCost(const double & iCost)  sets the cost variable
 *
 *      NodeID()                 returns nodeID
 *
 *      Name()                   returns name
 *
 *      Cost()                   returns cost
 *
 *      Order()                  returns tupleOrder
 *      AttributePtr()           returns attribute (the attribute that the
 *                               access method applies to
 *
 *      IndexPtr()               returns index
 *
 *      =                        assignment operator
 *
 *      ==                       comparison operator
 *
 *      SetOrder(TupleOrder iTupleOrder)  sets the tupleOrder vaiable
 *
 *      Show()                   outputs a text representation of the access
 *                               method
 */
class Plan_AccessMethod {

protected:
  int nodeID;   // for display purposes only

  int numLeafPages;
  string name;
  double cost;
  TupleOrder tupleOrder;
  Attribute *attribute;
  Index *index;

public:
  Plan_AccessMethod();
  Plan_AccessMethod(const string & iName,
                    const int & numLeafPages,
                    TupleOrder iTupleOrder,
                    Attribute *iAttribute,
                    const Index *iIndex);

  Plan_AccessMethod(const Plan_AccessMethod & am);
 ~Plan_AccessMethod();

  int NumLeafPages() { return numLeafPages; };

  void SetCost(const double & iCost);
  int NodeID();
  string Name();
  string FullName();

  double Cost();
  double CalculateCost(double selectivity = 1, bool indexOnly = false);
  TupleOrder Order();
  Attribute *AttributePtr();

  string GetDescription(bool indexOnly = false);

  void SetAttributePtr(Attribute *iAttribute);
  Index *IndexPtr();
  SelectType GetSelectType();
  void SetOrder(TupleOrder iTupleOrder);
  void operator=(const Plan_AccessMethod & am);
  bool operator==(const Plan_AccessMethod & am);
  void Show();
};

typedef List<Plan_AccessMethod> Plan_AccessMethodList;


//forward reference
class DataBase;



/* The Plan_Relation class maintains information about a particular relation.
 * The Plan_Relation class contains:
 *
 *      name                     the name of the relation
 *
 *      db                       a pointer to the data base that the
 *                               relation belongs to
 *
 *      numAttrs                 the number of attributes in the relation
 *
 *      cardinality              the approximate number of tuples
 *
 *      tupleSize                size of a tuple
 *
 *      numPages                 the number of pages that the relation
 *                               occupies
 *
 *      accessMethodList         a list of available access methods
 *
 *      attributeList            a list of the attributes belonging to
 *                               the relation
 *
 *      additionalPropertyList   a list of additional properties, probably
 *                               empty
 *
 *      Plan_Relation()          default constructor
 *
 *      Plan_Relation(const CString & iName, DataBase *iDb, ulong iCardinality,
 *                    int iTupleSize, int iNumPages)
 *                                                   constructor for setting
 *                                                   initial values to member
 *                                                   variables
 *
 *      Plan_Relation(const Plan_Relation & r)   copy constructor
 *
 *      Name()                   returns name
 *
 *      DbPtr()                  returns db
 *
 *      Cardinality()            returns cardinality
 *
 *      TupleSize()              returns tupleSize
 *
 *      NumPages()               returns numPages
 *
 *      ==                       comparison operator based on name
 */
class Plan_Relation {

protected:
  friend void AlterRelation(); // in main.C
  friend void AlterAttribute(); // in main.C

  string    name;
  string    originalName; // this name is the "real" one; when read in
  DataBase *db;
  ulong     cardinality;
  int       tupleSize;
  int       numPages;

// (SCH) Relation *relation;

public:
// (SCH) Relation *GetRelation() { return relation; }
  Plan_AccessMethodList accessMethodList;
  AttributeList         attributeList;
  PropertyList          additionalPropertyList;

// methods
public:
  Plan_Relation();
  Plan_Relation(const string & iName, DataBase *iDb, ulong iCardinality,
          int iTupleSize, int iNumPages/* (SCH) , Relation *rel*/);
  Plan_Relation(const Plan_Relation & r);

  string Name() { return name; };
  string OriginalName() { return originalName;}   // this is the "real" one
  void SetName(const string & newName) { name = newName; };
  DataBase *DbPtr() { return db; };
  ulong Cardinality() { return cardinality; };
  int TupleSize() { return tupleSize; };
  int NumPages() { return numPages; };
  void print() { cout << "<Plan Relation " <<  name; };

  bool operator==(const Plan_Relation & r) {
        return (originalName == r.originalName);
  }

  bool operator!=(const Plan_Relation & r) {
      return (originalName != r.originalName);
  }
};

typedef List<Plan_Relation> Plan_RelationList;



/* The SelectOperand class stores either a value (remember that AttrValue
 * stores either a CString, an integer, or a float) or a pointer to an
 * attribute.  For example in the following expression:
 *
 *       Employee.salary < 20000
 *
 * One instance of SelectOperand would store a pointer to Employee.salary,
 * and another instance would store an instance of AttrValue that stores
 * 20000.  An instance of SelectElem (described below) would represent
 * the whole expression Employee.salary < 20000.  The members are:
 *
 *
 *      literal                  TRUE if av stores a value (a literal),
 *                               FALSE if Attribute *a points to an attribute
 *
 *      av                       stores a value of a literal
 *
 *      a                        stores a pointer to an Attribute
 *
 *      SelectOperand()          default constructor
 *
 *      SelectOperand(const SelectOperand & so)  copy constructor
 *
 *
 *      IsLiteral()              returns literal
 *
 *      GetLiteral()             returns av
 *
 *      AttributePtr()           returns a
 *
 *      ==                       comparison operator
 *
 *      =                        assignment operators
 *
 *      MakeCString               returns a CString representation of the
 *                               select operand
 */
class SelectOperand {

protected:
  int        literal;

  AttrValue  av;
  Attribute *a;

public:
  SelectOperand(): av() { a = NULL; }

  SelectOperand(const SelectOperand & so) {
       literal = so.literal;
       if(literal) {
           av = so.av;
       } else {
           a = so.a;
       }
  }

  int operator==(const Attribute & attrib) {
                        if (!literal && a)
                            return (*a == attrib);
                        else
                            return FALSE;
  }

  int operator==(const SelectOperand & so);
  int operator!=(const SelectOperand & so) {
        return !(*this == so);
  }

  int IsLiteral() { return literal; };

  AttrValue GetLiteral() { return av; };
  Attribute *AttributePtr() { return a; };

  void operator =(Attribute *aInit) { literal = FALSE; a = aInit; };
  void operator =(const AttrValue &avInit) { literal = TRUE; av = avInit; };

  void Show();  // for debugging

  string MakeString();
};


/* The SelectElem class stores an operator (AttrOperator declared earlier
 * in this file; less than, greater than, equal to, ...), and pointers to
 * SelectOperands (see above).  For unary operators (i.e. - not, nop) arg2 is
 * NULL, otherwise arg1 and arg2 point to the operands of op.  An instance
 * of SelectElem can represent the entire expression:
 *
 *     Employee.salary < 20000
 *
 * Members of SelectElem are:
 *
 *      op                       operator to be applied to the argument(s),
 *                               (aopEQ, aopLT, aopGT, aopNE, aopLE, aopGE,
 *                               aopNOT, or aopNOP)
 *
 *      arg1, arg2               pointers to SelectOperands that are the
 *                               arguments
 *
 *      SelectElem()             default constructor
 *
 *      SelectElem(AttrOperator iOp, SelectOperand *iArg1, SelectOperand *iArg2)
 *                               constructor for setting inital values for
 *                               member variables
 *      SelectElem(const SelectElem & se) copy constructor
 *
 *      GetOperator()            returns op
 *
 *      Arg1()                   returns arg1
 *
 *      Arg2()                   returns arg2
 *
 *      GetSelectivity()         returns a ratio between 0 and 1 that is
 *                               an estimate of the ratio of tuples that
 *                               would match the condition represented
 *                               by the SelectElem class
 *

 *      MakeCString               returns a CString representation of
 *                               the class
 */
class SelectElem {

  public:
    enum Operator { EQUAL, NOTEQUAL, LESSTHAN, LESSEQUAL, OPTRUE, OPFALSE };
  protected:
    Operator op, op2;
    SelectOperand *arg1, *arg2, *arg3;

  public:

    SelectElem() {
        arg1 = arg2 = arg3 = NULL; op = op2 = SelectElem::OPTRUE;
    }
    SelectElem(AttrOperator iOp, SelectOperand *iArg1, SelectOperand *iArg2);

    SelectElem(const SelectElem& se);

    SelectType GetType() { if (   (op == LESSTHAN)  || (op == LESSEQUAL)
                               || (op2 == LESSTHAN) || (op2 == LESSEQUAL) )
                               return selRange;
                           else if (op == EQUAL)
                               return selExact;
                           else
                               return selUndefined;
    }

    bool TryToInclude( SelectElem& toInclude);
    bool Contains(Attribute *a) { return a && ((arg1 && (*arg1) == (*a))
                                          || (arg2 && (*arg2) == (*a))
                                          || (arg3 && (*arg3) == (*a)));
                              }

    AttrOperator GetOperator1();
    AttrOperator GetOperator2();
    //  AttrOperator GetOperator() { if (op) return op; else return op2; }
    SelectOperand *Arg1() { return arg1; }
    SelectOperand *Arg2() { return arg2; }
    SelectOperand *Arg3() { return arg3; }

    // the following three functions are a kludge to get it to work
    // with the spring 1995 764 project.  eventually select elem will
    // support full ranges (it already does, but it has been removed
    // because minirel does not.

    AttrOperator OldGetOperator();
    SelectOperand *OldArg1();
    SelectOperand *OldArg2();

//    const int SELRATIO = 10; // Selectivity Ratio defaults to 10
	int SELRATIO;
    double GetSelectivity();

   ~SelectElem() {
        if (arg1) delete arg1;
        if (arg2) delete arg2;
        if (arg3) delete arg3;
    }

    void Show();  // for debugging

    string MakeCString();
};


// forward reference
class SelectExpr;


/* The SelectTerm class stores part of a select predicate that can be
 * expressed in the form:
 *
 *          expression1 op expression2
 *
 * where op is either lopAND, lopOR, or lopNOT; repressenting AND, OR, NOT
 * respectively.  And expression1 and 2 are of the type SelectExpr described
 * below.  In the event of lopNOT sel2 is NULL.
 *
 * GetSelectivity and MakeCString do the same as the members of SelectElem
 * and SelectExpr by the same name.
 */
class SelectTerm {

protected:
  LogicalOperator   op;
  SelectExpr       *sel1,
                   *sel2;

public:

  SelectTerm() { sel1 = sel2 = NULL; } ;
  SelectTerm(LogicalOperator iOp, SelectExpr *iSel1, SelectExpr *iSel2) {
      op = iOp;
      sel1 = iSel1;
      sel2 = iSel2;
  }

  SelectTerm(const SelectTerm & st);

  bool Contains(Attribute *a);

  LogicalOperator GetOperator() { return op; };
  SelectExpr *Sel1() { return sel1; };
  SelectExpr *Sel2() { return sel2; };

  double GetSelectivity();

 ~SelectTerm();

  void Show();  // for debugging

  string MakeString();
};


/* The SelectExpr class stores either a pointer to a SelectElem or a
 * SelectTerm (both declared earlier).  If isElem is true then elem points
 * to an instance of SelectElem, otherwise term points to an instance of
 * SelectTerm.
 *
 * GetSelectivity and MakeCString do the same as the members of SelectElem
 * and SelectTerm by the same name.
 */
class SelectExpr {

protected:
  int isElem;
  union {
    SelectElem *elem;
    SelectTerm *term;
  };

public:

  SelectExpr() {}
  SelectExpr(const SelectExpr & se);

  void operator =(SelectElem *iElem) { elem = iElem; isElem = TRUE; }
  void operator =(SelectTerm *iTerm) { term = iTerm; isElem = FALSE; }

  int IsElement() { return isElem; }
  SelectElem *GetElem() { return elem; }
  SelectTerm *GetTerm() { return term; }

  bool Contains(Attribute *a) {
      if (isElem && elem)
          return elem->Contains(a);
      else if (term)
          return term->Contains(a);
      else
          return false;
  }

  double GetSelectivity();

 ~SelectExpr() {
    if (isElem && elem)
        delete elem;
    else if (term)
        delete term;
  }

  void Show();   // currently (4/21/94) for debugging purposes

  string MakeString();
};


/* The Select class maintains and provides information for a single select
 * predicate.  It stores a pointer to a SelectExpr (defined above).  The
 * SelectExpr represents the select predicate, the remaining members of
 * Select are member functions.
 *
 * The members of Select are:
 *
 *      GetAttribute             returns a pointer to an attribute that
 *                               is contained the SelectOperand pointed
 *                               to by its argument, and NULL if it does
 *                               not contain a pointer to an attribute.
 *                               GetAttribute is called only by
 *                               RGetAttributeList
 *
 *      RGetAttributeList        a recursive function that compiles a list
 *                               of attributes contained in the expression
 *                               pointed to by expr; any attributes in the
 *                               expression are added to the list pointed
 *                               to by its parameter al
 *
 *      GetAttributes            returns a pointer to a list of attributes
 *                               that are contained in the expression
 *                               pointed to by expr
 *
 *      CanDoSelect(Plan_Relation *r) returns TRUE if the select represented
 *                               by the expression pointed to by expr can
 *                               be applied to the relation pointed to by
 *                               r, and FALSE otherwise
 *
 *      CanDoSelect(Plan_RelationList *rl)
 *                               returns true if the select represented by
 *                               the expression pointed to by expr can be
 *                               applied to a relation that is the result
 *                               of joining all of the relations in the
 *                               Plan_RelationList pointed to by rl
 *
 *      MakeCString               returns a CString representation of the
 *                               Select
 */
class Select {

protected:
  SelectExpr *expr;

  Attribute *GetAttribute(SelectOperand *so);
  void RGetAttributeList(SelectExpr *expr, AttributeList & al);

public:
  Select(SelectExpr *iExpr = NULL) { expr = iExpr; }
  Select(const Select & se) { if (se.expr) expr = new SelectExpr(*se.expr); }
 ~Select() { if (expr) delete expr; }

  SelectExpr *GetExpr() { return expr; }

  SelectType GetType();
  int TypeCompatible(SelectType st);

  AttributeList *GetAttributeList();
  void GetAttributeList(AttributeList & al) { RGetAttributeList(expr, al); }
  bool Contains(Attribute *a);

  int CanDoSelect(const Plan_Relation *r);
  int CanDoSelect(const Plan_RelationList & rl);
  double GetSelectivity();

  int operator==(const Select & s) { return (expr == s.expr); }

  // currently (4/21/94) for debugging purposes
  void Show() { if (expr) expr->Show(); }

  string MakeString() { if (expr) return expr->MakeString(); else return ""; }
};

typedef List<Select> SelectList;

/* The Index class stores the description of an index.  The members are:
 *
 *      clustered                TRUE if it is a clustered index, FALSE
 *                               otherwise
 *
 *      distinctKeys             stores the number of distinct keys
 *
 *      numPages                 the number of pages that the index occupies
 *
 *      selectType               the type of select predicates that can use
 *                               this index; either range, exact match, both,
 *                               or undefined
 *
 *      relation                 the relation the index is on
 *
 *      attributes               a list of attributes the index is on
 *
 *      filename                     file name for index
 *
 *      Index(int iClustered,          Constructor for initializing clustered,
 *            int iDistinctKeys,       distinctKeys, numPages, and selectType
 *            int iNumPages,
 *            SelectType iSelectType)
 *
 *      Index(const Index & i)   copy constructor
 *
 *      IsClustered()            returns clustered
 *
 *      DistinctKeys()           returns distinctKeys
 *
 *      NumPages()               return numPages
 *
 *      SelectType()             returns selectType
 *
 *      Relation()               returns relation index is on
 *
 *      Attributes()             returns attribute list index is on
 *
 *      Filename()               returns file name of index
 *
 */
class Index {

protected:
  bool           clustered;
  int            distinctKeys;
  int            numPages;
  SelectType     selectType;
  string         filename;
  Plan_Relation *relation;
  AttributeList  attributes;

public:
  Index(int iClustered,
        int iDistinctKeys,
        int iNumPages,
        SelectType iSelectType,
        const AttributeList& iAttributes,
        string iFilename);

  Index(const Index & i);

  enum Types { BPlusTree, Hash };

  string Filename();
  Plan_Relation* Relation();

  AttributeList& Attributes();

  bool CanUse(Select* sel);
  double MostSelective(const SelectList* in, SelectList& out);
  bool IsClustered();
  int DistinctKeys();
  int NumPages();
  SelectType GetSelectType();
};


/* The State class declared below is the real nuts and bolts of this file.
 * All of the preceeding classes are all defined to be a part of the
 * the State class.  The State class stores a complete description of
 * a query.  The parser interface converts a parse tree produced by the
 * parser using information from the catalogs to an instance of the State
 * class that represents the query.
 *
 *           ----------              -------------              -----------
 *           |        |              |           |              |
 *   SQL --> | parser | -- parse --> | parser    | -- state --> | optimizer
 *           |        |    tree      | interface |              |
 *           ----------              -------------              -----------
 *                                         /\
 *                                          |
 *                                          |
 *                                         \/
 *                                    ------------
 *                                    |          |
 *                                    | catalogs |
 *                                    |          |
 *                                    ------------
 *
 * An instance of State represents a query in terms of the relations to be
 * joined, the attributes to project on, and the select predicates to be
 * performed. Consequentially it consists of a Plan_RelationList, AttributeList,
 * and SelectList; all of which are declared in this file.
 */
class State {

public:
  Plan_RelationList joins;  // relations to be joined
  AttributeList     project;
  AttributeList     neededAttributes;

  SelectList    selects;
  AttributeList groupby;
  SelectList    having;

  Plan_AccessMethodList omits;
  OrderByList order;
  bool distinct;
  bool aggDistinct;
  bool containsAggregates;

  State(): joins(), project(), neededAttributes(),
           selects(), groupby(), having(), omits(), order() {
      distinct = false;
      containsAggregates = false;
      aggDistinct = false;
  }

  State(const State & state);
 ~State() { };
};


#endif
