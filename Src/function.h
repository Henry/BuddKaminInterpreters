#ifndef functionh
#define functionh

#include "environment.h"

class ListNode;

/// Function
class Function
:
    public Expression
{
public:
    virtual Function* isFunction();

    virtual void apply(Expr&, ListNode*, Environment*);
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
    virtual void print();

    // isClosure is recognized only by functions
    virtual int isClosure();
};
///- Function

class UnaryFunction
:
    public Function
{
private:
    void (*fun) (Expr&, Expression*);

public:
    UnaryFunction()
    {
        fun = 0;
    }
    UnaryFunction(void (*afun) (Expr&, Expression*))
    {
        fun = afun;
    }
    virtual void apply(Expr&, ListNode*, Environment*);
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
};

class BinaryFunction
:
    public Function
{
private:
    void (*fun) (Expr&, Expression*, Expression*);

public:
    BinaryFunction()
    {
        fun = 0;
    }
    BinaryFunction(void (*afun) (Expr&, Expression*, Expression*))
    {
        fun = afun;
    }
    virtual void apply(Expr&, ListNode*, Environment*);
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
};

/// IntegerBinaryFunction
class IntegerBinaryFunction
:
    public BinaryFunction
{
private:
    int (*fun) (int, int);

public:
    IntegerBinaryFunction(int (*thefun) (int, int))
    {
        fun = thefun;
    }
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
};
///- IntegerBinaryFunction

/// BooleanBinaryFunction
class BooleanBinaryFunction
:
    public BinaryFunction
{
private:
    int (*fun) (int, int);

public:
    BooleanBinaryFunction(int (*thefun) (int, int))
    {
        fun = thefun;
    }
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
};
///- BooleanBinaryFunction

/// UserFunction
class UserFunction
:
    public Function
{
protected:
    List argNames;
    Expr body;
    Env context;

public:
    UserFunction(ListNode*, Expression*, Environment*);
    virtual void free();
    virtual void applyWithArgs(Expr&, ListNode*, Environment*);
    virtual int isClosure();
};
///- UserFunction

#endif
