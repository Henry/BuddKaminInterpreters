// -----------------------------------------------------------------------------
//  This file is part of
/// ---     Timothy Budd's Kamin Interpreters in C++
// -----------------------------------------------------------------------------
/// Title: The core classes for the basic lisp functions
///  Description:
//    LispReader: adds quoted constants to ReaderClass
// -----------------------------------------------------------------------------

#ifndef Lisp_H
#define Lisp_H

#include "reader.h"
#include "function.h"

// -----------------------------------------------------------------------------
/// LispReader
// -----------------------------------------------------------------------------
class QuotedConst
:
    public Expression
{
    //- The expression
    Expr value_;

public:

    //- Construct from the givin expression
    QuotedConst(Expression* v)
    {
        value_ = v;
    }

    //- Delete according to reference counts
    virtual ~QuotedConst();

    //- Evaluate the quoted expression
    virtual void eval(Expr&, Environment*, Environment*);

    //- Print
    virtual void print();
};

class LispReader
:
    public ReaderClass
{
protected:
    virtual Expression* readExpression();
};
///- LispReader


// -----------------------------------------------------------------------------
/// Arithmetic functions
// -----------------------------------------------------------------------------
int PlusFunction(int, int);
int MinusFunction(int, int);
int TimesFunction(int, int);
int DivideFunction(int, int);

// -----------------------------------------------------------------------------
/// Relational functions
// -----------------------------------------------------------------------------
void EqualFunction(Expr&, Expression*, Expression*);
int IntEqualFunction(int, int);
int LessThanFunction(int, int);
int GreaterThanFunction(int, int);

// -----------------------------------------------------------------------------
//      We can do Car and Cdr because they all evaluate their arguments
//      But we can't include cons because in chap5 is ceases to evaluate
//      its arguments
// -----------------------------------------------------------------------------
void CarFunction(Expr&, Expression*);
void CdrFunction(Expr&, Expression*);
void ConsFunction(Expr&, Expression*, Expression*);

// -----------------------------------------------------------------------------
/// BooleanUnary
// -----------------------------------------------------------------------------
class BooleanUnary
:
    public UnaryFunction
{
    int (*function_) (Expression*);

public:
    BooleanUnary(int (*f) (Expression*))
    {
        function_ = f;
    }
    virtual void applyWithArgs(Expr& target, ListNode* args, Environment*);
};
///- BooleanUnary


// -----------------------------------------------------------------------------
/// Predicates
// -----------------------------------------------------------------------------
int NumberpFunction(Expression*);
int SymbolpFunction(Expression*);
int ListpFunction(Expression*);
int NullpFunction(Expression*);
int PrimoppFunction(Expression*);
int ClosurepFunction(Expression*);


// -----------------------------------------------------------------------------
/// Print
// -----------------------------------------------------------------------------
void PrintFunction(Expr&, Expression*);


// -----------------------------------------------------------------------------
/// Define
// -----------------------------------------------------------------------------
class DefineStatement
:
    public Function
{
public:
    virtual void apply(Expr&, ListNode*, Environment*);
};
///- Define


// -----------------------------------------------------------------------------
/// IfStatement
// -----------------------------------------------------------------------------
class IfStatement
:
    public Function
{
public:
    virtual void apply(Expr&, ListNode*, Environment*);
};
///- IfStatement


// -----------------------------------------------------------------------------
/// WhileStatement
// -----------------------------------------------------------------------------
class WhileStatement
:
    public Function
{
public:
    virtual void apply(Expr&, ListNode*, Environment*);
};
///- WhileStatement


// -----------------------------------------------------------------------------
/// SetStatement
// -----------------------------------------------------------------------------
class SetStatement
:
    public Function
{
public:
    virtual void apply(Expr&, ListNode*, Environment*);
};
///- SetStatement


// -----------------------------------------------------------------------------
/// BeginStatement
// -----------------------------------------------------------------------------
class BeginStatement
:
    public Function
{
public:
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
};
///- BeginStatement


// -----------------------------------------------------------------------------
#endif // Lisp_H
// -----------------------------------------------------------------------------
