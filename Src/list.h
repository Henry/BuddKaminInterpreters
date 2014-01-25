// -----------------------------------------------------------------------------
//  This file is part of
/// ---     Timothy Budd's Kamin Interpreters in C++
// -----------------------------------------------------------------------------
/// Title: Linked-list
///  Description:
//    ListNode element of a linked-list
//    List is a reference-counted wrapper around ListNode for GC
// -----------------------------------------------------------------------------

#ifndef List_H
#define List_H

#include "expression.h"

// -----------------------------------------------------------------------------
/// List
// -----------------------------------------------------------------------------
class ListNode
:
    public Expression
{
    //- The head element of cons-cell
    Expr head_;

    //- The tail element of a cons-cell
    Expr tail_;


public:

    //- Construct from the head and tail elements
    ListNode(Expression*, Expression*);

    //- Delete according to reference counts
    virtual ~ListNode();

    //- Evaluate the list
    virtual void eval(Expr&, Environment*, Environment*);

    //- Return the head
    virtual Expression* head()
    {
        return head_();
    }

    //- Set the head
    void head(Expression* x);

    //- Return the tail
    ListNode* tail();

    //- Specialised type predicate
    virtual ListNode* isList();

    //- Empty list predicate
    int isNil();

    //- Return the number of elements in the list O(1)
    int length();

    //- Return element at index
    Expression* at(const int index);

    //- Print
    virtual void print();
};


class List
:
    public Expr
{
public:

    //- Null constructor
    List()
    {}

    //- Construct from ListNode incrementing reference-count
    List(ListNode* r)
    {
        Expr::operator=(r);
    }

    //- Return the ListNode* if set
    ListNode* operator()()
    {
        return val() ? val()->isList() : 0;
    }

    //- Cast to ListNode
    operator ListNode*()
    {
        return operator()();
    }

    //- Set/reset the ListNode
    void operator=(ListNode* r)
    {
        Expr::operator=(r);
    }
};
///- List


// -----------------------------------------------------------------------------
/// Member functions for class ListNode
// -----------------------------------------------------------------------------

inline void ListNode::head(Expression* x)
{
    head_ = x;
}

// -----------------------------------------------------------------------------
#endif // List_H
// -----------------------------------------------------------------------------
