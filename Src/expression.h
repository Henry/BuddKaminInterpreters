#ifndef expressionh
#define expressionh

// forward references
class Environment;
class Expression;

/// Expr
class Expr
{
private:

    Expression* value;

protected:

    Expression* val()
    {
        return value;
    }

public:

    Expr(Expression* = 0);

    Expression* operator()()
    {
        return val();
    }

    void operator=(Expression*);

    void evalAndPrint(Environment*, Environment*);
};
///- Expr

// more forward declarations
class IntegerExpression;
class Symbol;
class ListNode;
class Function;
class Environment;
class APLValue;
class Method;
class PrologValue;
class Continuation;

/// Expression
class Expression
{
    public:
//private:
    friend class Expr;
    mutable int referenceCount;

public:
    Expression();

    virtual ~Expression()
    {}

    virtual void free();

    // basic object protocol
    virtual void eval(Expr&, Environment*, Environment*);
    virtual void print();

    // conversion tests
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

/// IntegerExpression
class IntegerExpression
:
    public Expression
{
private:
    int value;

public:
    IntegerExpression(int v)
    {
        value = v;
    }

    virtual void print();
    virtual IntegerExpression* isInteger();

    int val()
    {
        return value;
    }
};
///- IntegerExpression

/// Symbol
class Symbol
:
    public Expression
{
private:
    char* text;

public:
    Symbol(const char*);

    virtual void free();
    virtual void eval(Expr&, Environment*, Environment*);
    virtual void print();
    virtual Symbol* isSymbol();

    int operator==(Expression*) const;
    int operator==(const char*) const;
    char* chars()
    {
        return text;
    }
};
///- Symbol

Expression* error(const char*, const char* x = 0);

#endif
