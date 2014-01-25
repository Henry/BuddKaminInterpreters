// -----------------------------------------------------------------------------
//  This file is part of
/// ---     Timothy Budd's Kamin Interpreters in C++
// -----------------------------------------------------------------------------
/// Title: Function
///  Description:
//    Base class and specialization for unary, binary and user-defined functions
// -----------------------------------------------------------------------------

#ifndef Function_H
#define Function_H

#include "environment.h"

// -----------------------------------------------------------------------------
/// Forward declarations
// -----------------------------------------------------------------------------
class ListNode;

// -----------------------------------------------------------------------------
/// Function
// -----------------------------------------------------------------------------
class Function
:
    public Expression
{
public:

    //- Specialised type predicate
    virtual Function* isFunction();

    //- Apply function to given list in given environment and return result
    virtual void apply(Expr& result, ListNode*, Environment*);

    //- Apply function with arguments to given list in given environment
    //  and return result
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);

    //- isClosure is recognized only by functions
    virtual int isClosure();

    //- Print
    virtual void print();
};
///- Function


// -----------------------------------------------------------------------------
/// UnaryFunction
// -----------------------------------------------------------------------------
class UnaryFunction
:
    public Function
{
    //- The function pointer
    void (*function_) (Expr&, Expression*);

public:

    //- Construct null
    UnaryFunction()
    :
        function_(0)
    {}

    //- Construct from function pointer
    UnaryFunction(void (*f) (Expr&, Expression*))
    :
        function_(f)
    {}

    //- Apply function to given list in given environment and return result
    virtual void apply(Expr&, ListNode*, Environment*);

    //- Apply function with arguments to given list in given environment
    //  and return result
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
};
///- UnaryFunction


// -----------------------------------------------------------------------------
/// BinaryFunction
// -----------------------------------------------------------------------------
class BinaryFunction
:
    public Function
{
    //- The function pointer
    void (*function_) (Expr&, Expression*, Expression*);

public:

    //- Construct null
    BinaryFunction()
    :
        function_(0)
    {}

    //- Construct from function pointer
    BinaryFunction(void (*f) (Expr&, Expression*, Expression*))
    :
        function_(f)
    {}

    //- Apply function to given list in given environment and return result
    virtual void apply(Expr&, ListNode*, Environment*);

    //- Apply function with arguments to given list in given environment
    //  and return result
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
};
///- BinaryFunction


// -----------------------------------------------------------------------------
/// IntegerBinaryFunction
// -----------------------------------------------------------------------------
class IntegerBinaryFunction
:
    public BinaryFunction
{
    //- The function pointer
    int (*function_) (int, int);

public:

    //- Construct from function pointer
    IntegerBinaryFunction(int (*thefun) (int, int))
    {
        function_ = thefun;
    }

    //- Apply function with arguments to given list in given environment
    //  and return result
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
};
///- IntegerBinaryFunction


// -----------------------------------------------------------------------------
/// BooleanBinaryFunction
// -----------------------------------------------------------------------------
class BooleanBinaryFunction
:
    public BinaryFunction
{
    //- The function pointer
    int (*function_) (int, int);

public:

    //- Construct from function pointer
    BooleanBinaryFunction(int (*thefun) (int, int))
    {
        function_ = thefun;
    }

    //- Apply function with arguments to given list in given environment
    //  and return result
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
};
///- BooleanBinaryFunction


// -----------------------------------------------------------------------------
/// UserFunction
// -----------------------------------------------------------------------------
class UserFunction
:
    public Function
{
protected:

    List argNames_;
    Expr body_;
    Environment* context_;

public:

    //- Construct from components
    UserFunction(ListNode*, Expression*, Environment*);

    //- Destructor
    virtual ~UserFunction();

    //- Apply function with arguments to given list in given environment
    //  and return result
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);

    //- Is this user-function a closure?
    virtual int isClosure();
};
///- UserFunction


// -----------------------------------------------------------------------------
#endif // Function_H
// -----------------------------------------------------------------------------
