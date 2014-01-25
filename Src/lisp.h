//
//      The core classes for the basic lisp functions
//
#include "reader.h"
#include "function.h"

//
//      the Lisp reader adds quoted constants
//

/// LispReader
class QuotedConst
:
    public Expression
{
    Expr theValue;

public:

    QuotedConst(Expression* val)
    {
        theValue = val;
    }

    virtual ~QuotedConst();

    virtual void eval(Expr&, Environment*, Environment*);
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

//
//      The arithmetic functions
//

int PlusFunction(int, int);
int MinusFunction(int, int);
int TimesFunction(int, int);
int DivideFunction(int, int);

//
//      Relational functions
//

void EqualFunction(Expr&, Expression*, Expression*);
int IntEqualFunction(int, int);
int LessThanFunction(int, int);
int GreaterThanFunction(int, int);

//
//      We can do Car and Cdr because they all evaluate their arguments
//      But we can't include cons because in chap5 is ceases to evaluate
//      its arguments
//


void CarFunction(Expr&, Expression*);
void CdrFunction(Expr&, Expression*);
void ConsFunction(Expr&, Expression*, Expression*);

//
//      predicates
//

/// BooleanUnary
class BooleanUnary
:
    public UnaryFunction
{
private:
    int (*fun) (Expression*);

public:
    BooleanUnary(int (*thefun) (Expression*))
    {
        fun = thefun;
    }
    virtual void applyWithArgs(Expr& target, ListNode* args, Environment*);
};
///- BooleanUnary

int NumberpFunction(Expression*);
int SymbolpFunction(Expression*);
int ListpFunction(Expression*);
int NullpFunction(Expression*);
int PrimoppFunction(Expression*);
int ClosurepFunction(Expression*);

void PrintFunction(Expr&, Expression*);

//
//      commands
//

/// Define
class DefineStatement
:
    public Function
{
public:
    virtual void apply(Expr&, ListNode*, Environment*);
};
///- Define

/// IfStatement
class IfStatement
:
    public Function
{
public:
    virtual void apply(Expr&, ListNode*, Environment*);
};
///- IfStatement

/// WhileStatement
class WhileStatement
:
    public Function
{
public:
    virtual void apply(Expr&, ListNode*, Environment*);
};
///- WhileStatement

/// SetStatement
class SetStatement
:
    public Function
{
public:
    virtual void apply(Expr&, ListNode*, Environment*);
};
///- SetStatement

/// BeginStatement
class BeginStatement
:
    public Function
{
public:
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
};
///- BeginStatement
