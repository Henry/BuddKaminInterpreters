// -----------------------------------------------------------------------------
//  This file is part of
/// ---     Timothy Budd's Kamin Interpreters in C++
// -----------------------------------------------------------------------------
/// Title: Exrpessions
///  Description:
//    Expression is the reference counted base-class for all expressions
//    Expr is a reference-counting wrapper around Expression for GC
// -----------------------------------------------------------------------------

#ifndef Expression_H
#define Expression_H

// -----------------------------------------------------------------------------
/// Forward declarations
// -----------------------------------------------------------------------------
class Environment;
class Expression;
class IntegerExpression;
class Symbol;
class ListNode;
class Function;
class Environment;
class APLValue;
class Method;
class PrologValue;
class Continuation;

// -----------------------------------------------------------------------------
/// Expr
// -----------------------------------------------------------------------------
class Expr
{
    //- The expression
    Expression* value_;

protected:

    //- Return the expression
    Expression* val()
    {
        return value_;
    }

public:

    //- Construct null setting the value to 0
    Expr();

    //- Construct from given expression
    Expr(Expression*);

    //- Construct copy incrementing reference count
    Expr(Expr&);

    //- Delete according to reference counts
    ~Expr();

    //- Return the expression
    Expression* operator()()
    {
        return value_;
    }

    //- Set/reset expression
    void operator=(Expression*);

    //- Evaluate and print the expression
    void evalAndPrint(Environment*, Environment*);
};
///- Expr


// -----------------------------------------------------------------------------
/// Expression
// -----------------------------------------------------------------------------
class Expression
{
    //- The reference-count for GC
    mutable int referenceCount;

public:

    friend class Expr;

    //- Construct null
    Expression();

    //- Delete according to reference counts
    virtual ~Expression();

    //- Evaluate expression
    virtual void eval(Expr&, Environment*, Environment*);

    //- Print
    virtual void print();

    // Conversion/type predicates
    virtual Expression* touch();
    virtual IntegerExpression* isInteger();
    virtual Symbol* isSymbol();
    virtual Function* isFunction();
    virtual ListNode* isList();
    virtual Environment* isEnvironment();
    virtual APLValue* isAPLValue();
    virtual Method* isMethod();
    virtual Environment* isCluster();
    virtual PrologValue* isPrologValue();
    virtual Continuation* isContinuation();
};
///- Expression


// -----------------------------------------------------------------------------
/// IntegerExpression
// -----------------------------------------------------------------------------
class IntegerExpression
:
    public Expression
{
    //- The integer value
    int value_;

public:

    //- Construct
    IntegerExpression(const int v)
    {
        value_ = v;
    }

    //- Specialised type predicate
    virtual IntegerExpression* isInteger();

    //- Return the integer value
    int val()
    {
        return value_;
    }

    //- Print
    virtual void print();

};
///- IntegerExpression


// -----------------------------------------------------------------------------
/// Symbol
// -----------------------------------------------------------------------------
class Symbol
:
    public Expression
{
    //- The symbol name
    char* name_;

public:

    //- Construct given the name
    Symbol(const char* name);

    //- Destructor which deletes the name
    virtual ~Symbol();

    //- Evaluate the symbol
    virtual void eval(Expr&, Environment*, Environment*);

    //- Print
    virtual void print();

    //- Specialised type predicate
    virtual Symbol* isSymbol();

    //- Compare with the given expression
    int operator==(Expression*) const;

    //- Compare with the given name
    int operator==(const char*) const;

    //- Return the symbol's name
    const char* name() const
    {
        return name_;
    }
};
///- Symbol

//- Generate an error expression
Expression* error(const char*, const char* x = 0);

// -----------------------------------------------------------------------------
#endif // Expression_H
// -----------------------------------------------------------------------------
