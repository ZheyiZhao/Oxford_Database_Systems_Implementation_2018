//
// ndlist.h      nd is for non-destructive
//
// By Stephen Harris 1994.  Adapted from code by David M. Koscinski.
//

#ifndef ndlist_h
#define ndlist_h

#include "da_types.h"
#include "dlist.h"


template <class T>
class List: public DList<T>
{
private:
        int dConstructor;        // true if destructive copy constructor used
        T * SafeNew(const T & t);
public:
        List(int isDest = FALSE): DList<T>() { dConstructor = isDest; }
        List(const List<T> & l);     // copy constructor, makes complete copy
        List(const List<T> * l): DList<T>(l) { dConstructor = 1; }
        ~List() { if (dConstructor) this->UnLink(); else this->Clean(); }

        // non-destructive
        void operator=(const List<T> & l);
        // destructive
        void operator=(const List<T> * l);

        // In all of the concatenation and subtraction operators the 
        // arguments are left unchanged by the functions, however, in 
        // the non-destructive ones data is copied for concatenation and
        // "freed" for subtraction.  For the destructive ones data is 
        // neither copied nor "freed".  Keep in mind that non-destructive
        // and destructive subtraction are misnomers to an extent.  The
        // main difference is that the destructive version works 
        // consistently on destructive lists and the non-destructive version
        // works consistently on non-destructive lists.

        // non-destructive concatenation
        void operator+(const List<T> & l);
        // destructive concatenation
        void operator+(const List<T> * l);

        // non-destructive subtraction
        void operator-(const List<T> & l);
        // destructive subtraction
        void operator-(const List<T> * l);

        T * Insert(const T & t);                   // t is copied
        T * Insert(T * t) { return DList<T>::Insert(t); } ;
        T * InsertBeforeCurr(const T & t);         // t is copied
        T * InsertBeforeCurr(T * t) { return DList<T>::InsertBeforeCurr(t); } ;
           T * InsertHead(const T & t);            // t is copied
           T * InsertHead(T * t) { return DList<T>::InsertHead(t); } ;
           T * InsertTail(const T & t);                // t is copied
        T * InsertTail(T * t) { return DList<T>::InsertTail(t); } ;
};


template <class T>
inline T * List<T>::SafeNew(const T & t)
{
  T *temp = new T(t);
  if (!temp) FatalError(ErrMEM);
  return temp;
}


// copy constructor
template <class T>
List<T>::List(const List & l)
{
        dConstructor = 0;          // destructive constructor not used

        this->head = this->tail = this->curr = NULL;
        this->count = 0;

        ListNode<T> *n;

        for( n=l.head; n != NULL; n=n->next )
                Insert(*n->data);

        this->curr = this->head;
}

template <class T>
void List<T>::operator=(const List & l)
{
        if (dConstructor)
          this->UnLink();
        else
          this->Clean();

        dConstructor = 0;

        ListNode<T> *n;

        for( n=l.head; n != NULL; n=n->next )
                Insert(*n->data);

        this->curr = this->head;
}

template <class T>
void List<T>::operator=(const List * l)
{
        if (dConstructor)
          this->UnLink();
        else
          this->Clean();

        dConstructor = 1;

        ListNode<T> *n;

        for( n=l->head; n != NULL; n=n->next )
                Insert(n->data);

        this->curr = this->head;
}

// non-destructive concatenation
template <class T>
void List<T>::operator+(const List & l)
{
        ListNode<T> *n;

        for (n = l.head; n; n = n->next)
          InsertTail(*n->data);
}


// destructive concatenation
template <class T>
void List<T>::operator+(const List * l)
{
        ListNode<T> *n;

        for (n = l->head; n; n = n->next)
          InsertTail(n->data);
}


// non-destructive subtraction
template <class T>
void List<T>::operator-(const List & l)
{
        ListNode<T> *n;

        for (n = l.head; n; n = n->next)
          Delete(*n->data);
}



// destructive subtraction
template <class T>
void List<T>::operator-(const List * l)
{
        ListNode<T> *n;

        for (n = l->head; n; n = n->next)
          Detach(n->data);
}


// make copy and insert after curr
template <class T>
inline T * List<T>::Insert(const T & t)
{
#ifdef debug_messages
        T *result = DList<T>::Insert(SafeNew(t));
        curr->destructiveInsert = FALSE;
        return result;
#else
        return DList<T>::Insert(SafeNew(t));
#endif
}


// make copy insert before curr node
template <class T>
inline T * List<T>::InsertBeforeCurr(const T & t)
{
#ifdef debug_messages
        T *result = DList<T>::InsertBeforeCurr(SafeNew(t));
        curr->destructiveInsert = FALSE;
        return result;
#else
        return DList<T>::InsertBeforeCurr(SafeNew(t));
#endif
}


// make copy and insert at head
template <class T>
inline T * List<T>::InsertHead(const T & t)
{
#ifdef debug_messages
        T *result = DList<T>::InsertHead(SafeNew(t));
        curr->destructiveInsert = FALSE;
        return result;
#else
        return DList<T>::InsertHead(SafeNew(t));
#endif
}

// make copy and insert at tail
template <class T>
inline T * List<T>::InsertTail(const T & t)
{
#ifdef debug_messages
        T *result = DList<T>::InsertTail(SafeNew(t));
        curr->destructiveInsert = FALSE;
        return result;
#else
        return DList<T>::InsertTail(SafeNew(t));
#endif
}


#endif
