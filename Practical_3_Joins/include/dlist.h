//
// dlist.h      d is for destructive
//
// By Stephen Harris 1994.  Adapted from code by David M. Koscinski.
//
// $Id: dlist.h,v 1.1 1997/01/02 12:45:24 flisakow Exp $

#ifndef dlist_h
#define dlist_h

#include "da_types.h"
#include "listbase.h"

template <class T>
class DList: public ListBase<T>
{
public:
        DList(): ListBase<T>() { /* empty */ };
        DList(const DList<T> & l);      // copy constructor; copies only
                                        // the links, the nodes of l are
                                        // the same as for the
                                        // resulting list

        DList(const DList<T> * l);        // same as the copy constructor;
                                        // provided to be consistent with
                                        // class NDList

        ~DList() { this->UnLink(); };

        void operator=(const DList<T> * l);

        // Concatenates argument to end of list.  Does not copy data in
        // argument, only pointers.
        void operator+(const DList<T> * l);

        // removes items from list that are also in l.  Does not perform
        // delete on any elements.
        void operator-(const DList<T> * l);

        T * Insert(T * t);                  // t is not copied i.e. -
                                        // its address is the same
                                        // in the list as before inserted
        T * InsertBeforeCurr(T * t);
        T * InsertHead(T * t);
        T * InsertTail(T * t);
};


template <class T>
inline DList<T>::DList(const DList & l)
{
        this->head = this->tail = this->curr = NULL;
        this->count = 0;

        ListNode<T> *n;

        for( n=l.head; n; n=n->next )
                Insert(n->data);

        this->curr = this->head;
}



template <class T>
inline DList<T>::DList(const DList * l)
{
        this->head = this->tail = this->curr = NULL;
        this->count = 0;

        ListNode<T> *n;

        for( n=l->head; n; n=n->next )
                Insert(n->data);

        this->curr = this->head;
}




template <class T>
inline void DList<T>::operator=(const DList * l)
{
        this->UnLink();

        ListNode<T> *n;

        for( n=l->head; n; n=n->next )
                Insert(n->data);

        this->curr = this->head;
}


// concatenation
template <class T>
inline void DList<T>::operator+(const DList * l)
{
        ListNode<T> *n;
         
        for (n = l->head; n; n = n->next)
          InsertTail(n->data);
}

// subtraction
template <class T>
inline void DList<T>::operator-(const DList * l)
{
  ListNode<T> *n;

  for (n = l->head; n; n = n->next) 
    Detach(n->data);
}

// insert after curr node
template <class T>
inline T * DList<T>::Insert(T * t)
{
        ListNode<T> *temp = new ListNode<T>(t);
        if (!temp) FatalError(ErrMEM);

#ifdef debug_messages
        temp->destructiveInsert = TRUE;
#endif

        if( this->count == 0 )
        {
                this->head = temp;
                this->tail = temp;
                temp->next = NULL;
        }
        else if( this->curr == NULL )
        {
                this->tail->next = temp;
                this->tail = temp;
                temp->next = NULL;
        }
        else
        {
                temp->next = this->curr->next;
                this->curr->next = temp;
                if( this->tail == this->curr )
                        this->tail = temp;
        }
        this->curr = temp;
        this->count++;

        return this->CurrPtr();
}

// insert before curr node
template <class T>
inline T * DList<T>::InsertBeforeCurr(T * t)
{
        ListNode<T> *ln = this->curr;

        if (this->curr == this->head || this->count == 0) 
                return InsertHead(t);
        else {        // set curr to preceding node and insert after that one
                for( this->curr = this->head; this->curr->next != ln; this->curr = this->curr->next );
                return Insert(t);
        }
}

// insert at head
template <class T>
inline T * DList<T>::InsertHead(T * t)
{
        ListNode<T> *temp = new ListNode<T>(t);
        if (!temp) FatalError(ErrMEM);

#ifdef debug_messages
        temp->destructiveInsert = TRUE;
#endif

        temp->next = this->head;
        if( this->tail == NULL )
                this->tail = temp;
        this->head = temp;

        this->curr = temp;
        this->count++;

        return this->CurrPtr();
}


// insert at tail
template <class T>
inline T * DList<T>::InsertTail(T * t)
{
        ListNode<T> *temp = new ListNode<T>(t);
        if (!temp) FatalError(ErrMEM);

#ifdef debug_messages
        temp->destructiveInsert = TRUE;
#endif

        if( this->head == NULL )
                this->head = temp;
        else
                this->tail->next = temp;

        this->tail = temp;
        temp->next = NULL;

        this->curr = temp;
        this->count++;

        return this->CurrPtr();
}

#endif
