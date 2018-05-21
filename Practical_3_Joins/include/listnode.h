//
// listnode.h
//
// By Stephen Harris 1994.  Adapted from code by David M. Koscinski.
//

#ifndef listnode_h
#define listnode_h

// comment out the following line to dissable debugging messages
// #define debug_messages


template <class T>
class ListNode
{
public:
        ListNode *next;
        T *data;
#ifdef debug_messages
        int destructiveInsert;
#endif

public:
        ListNode(T * t)                  
        {
                data = t;
                next = NULL;
        };

};

#endif
