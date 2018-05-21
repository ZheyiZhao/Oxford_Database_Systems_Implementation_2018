//
// listbase.h  
//
// By Stephen Harris 1994.  Adapted from code by David M. Koscinski.
//

#ifndef listbase_h
#define listbase_h

#include "da_types.h"
#include "listnode.h"


template <class T>
class ListBase
{
protected:
        ListNode<T> *head;
        ListNode<T> *tail;
        ListNode<T> *curr;
        ushort count;

public:
        ListBase();
            ~ListBase() { UnLink(); }; // destructor

        T * Detach();                // unlinks current node and returns its address

        T * Detach(T * t);        // unlinks node pointed to by t and returns
                                // its address

        T * Detach(const T & t);// finds element in list that matches t then
                                // unlinks it from the list

        T * Delete();                // frees current node back to the heap and
                                // returns the new current pointer

        T * Delete(T * t);        // frees node pointed to by t back to the
                                // heap and returns the new current pointer

        T * Delete(const T & t); // finds element in list that matches t then
                                 // removes and frees it from the list, then
                                 // returns the new current pointer

        void Clean();
        void UnLink();

        T * GoHead();
        T * GoTail();
        T * GoNext();
        T * Goto(T * t);
        T * Goto(const T & t);

        int Size() const { return count; }
        int IsEmpty() const { return !count; }

        T * CurrPtr();
        T & Curr();
};


template <class T>
inline ListBase<T>::ListBase()
{
        head = tail = curr = NULL;
        count = 0;
}


template <class T>
T * ListBase<T>::Goto(T * t)
{
        for( GoHead(); ((CurrPtr() != t) && CurrPtr()); GoNext() );
        return CurrPtr();
}

template <class T>
T * ListBase<T>::Goto(const T & t)
{
        int done;
        for( GoHead(), done = 0; (CurrPtr() && !done); )
          if (!(done = (Curr() == t))) GoNext();
        
        return CurrPtr();
}

template <class T>
inline T * ListBase<T>::Detach(T * t)
{
        Goto(t);
        return Detach();
}

template <class T>
inline T * ListBase<T>::Detach(const T & t)
{
        Goto(t);
        return Detach();
}


template <class T>
T * ListBase<T>::Detach()
{
        T *result = CurrPtr();

        if( curr == NULL )
                return NULL;

#ifdef debug_messages
        if (!curr->destructiveInsert)
          cerr << "Warning: Detatching node that was inserted non-destructively\n";
#endif

        ListNode<T> *ln = NULL;

        // set ln to point to the node preceding curr
        if( curr != head )
                for( ln = head; ln->next != curr; ln = ln->next );

        if( curr == head && curr == tail )
        {
                delete curr;
                head = tail = curr = NULL;
        }
        else if( curr == tail )
        {
                tail = ln;
                tail->next = NULL;
                delete curr;
                curr = NULL;
        }
        else if( curr == head )
        {
                head = curr->next;
                delete curr;
                curr = head;
        }
        else
        {
                ln->next = curr->next;
                delete curr;
                curr = ln->next;
        }
        count--;
        return result;
}


template <class T>
inline T * ListBase<T>::Delete(T * t)
{
        Goto(t);
        return Delete();
}

template <class T>
inline T * ListBase<T>::Delete(const T & t)
{
        Goto(t);
        return Delete();
}

template <class T>
inline T * ListBase<T>::Delete()
{
#ifdef debug_messages
        if (curr != NULL)
          if (curr->destructiveInsert)
            cerr << "Warning: Deleting node inserted destructively\n";
        curr->destructiveInsert = TRUE;
#endif
        T *temp = Detach();
        if (temp) delete temp;
        return CurrPtr();
}


template <class T>
inline T * ListBase<T>::GoHead()
{
        curr = head;
        return CurrPtr();
}


template <class T>
inline T * ListBase<T>::GoTail()
{
        curr = tail;
        return CurrPtr();
}


template <class T>
inline T * ListBase<T>::GoNext()
{
        if( curr != NULL )
                curr = curr->next;
        return CurrPtr();
}


template <class T>
void ListBase<T>::Clean()
{
    if ( IsEmpty() ) return;
    
    for( GoHead(); CurrPtr(); Delete() );
}


template <class T>
void ListBase<T>::UnLink()
{
        for( GoHead(); CurrPtr(); Detach() );
}


template <class T>
inline T * ListBase<T>::CurrPtr()
{
        return curr ? curr->data : 0;
}


template <class T>
inline T & ListBase<T>::Curr()
{
        return *(curr->data);
}


#endif
