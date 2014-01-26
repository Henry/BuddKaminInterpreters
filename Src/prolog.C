// -----------------------------------------------------------------------------
//  This file is part of
/// ---     Timothy Budd's Kamin Interpreters in C++
// -----------------------------------------------------------------------------
/// Title: Prolog
///  Description:
//    The Prolog interpreter from Chapter 8
// -----------------------------------------------------------------------------
#include <iostream>
#include "lisp.h"

// -----------------------------------------------------------------------------
/// Global declarations
// -----------------------------------------------------------------------------
extern Env globalEnvironment;
extern Env commands;
extern Env valueOps;
extern List emptyList;

// Need isTrue although not used
int isTrue(Expression*)
{
    return 0;
}

// -----------------------------------------------------------------------------
/// PrologValue
// -----------------------------------------------------------------------------
class PrologValue
:
    public Expression
{
private:

    Expr data;

public:

    PrologValue(Expression* d)
    {
        data = d;
    }

    virtual ~PrologValue()
    {
        data = 0;
    }

    virtual void print();

    virtual void eval(Expr&, Environment*, Environment*);

    virtual Symbol* isSymbol();

    virtual PrologValue* isPrologValue()
    {
        return this;
    }

    int isUndefined()
    {
        return data() == 0;
    }

    void setUndefined()
    {
        data = 0;
    }

    PrologValue* indirectPtr();

    void setIndirect(PrologValue* v)
    {
        data = v;
    }
};
///- PrologValue


void PrologValue::print()
{
    Symbol* s = isSymbol();
    if (s)
    {
        std::cout<< s->name();
    }
    else
    {
        std::cout<< "unbound variable";
    }
}

PrologValue* PrologValue::indirectPtr()
{
    if (!isUndefined())
    {
        return data()->isPrologValue();
    }
    return 0;
}


// -----------------------------------------------------------------------------
/// PrologEval
// -----------------------------------------------------------------------------
Symbol* PrologValue::isSymbol()
{
    PrologValue* iptr = indirectPtr();
    if (iptr)
    {
        return iptr->isSymbol();
    }
    if (!isUndefined())
    {
        return data()->isSymbol();
    }
    return 0;
}

void PrologValue::eval(Expr& target, Environment* valueOps, Environment* rho)
{
    Symbol* s = isSymbol();
    if (s)
    {
        Expression* r = rho->lookup(*s);
        if (r)
        {
            target = r;
            return;
        }

        // Symbol is not known
        const char* p = s->name().c_str();

        // If lower case, eval to itself
        if ((*p >= 'a') && (*p <= 'z'))
        {
            // Symbols eval to themselves
            target = this;
            return;
        }

        // Else make a new symbol
        target = new PrologValue(0);
        rho->add(s, target());

        return;
    }

    target = this;
    return;
}
///- PrologEval


// -----------------------------------------------------------------------------
/// PrologReader
//    reads prolog symbols (no longer recognizes integer)
// -----------------------------------------------------------------------------
class PrologReader
:
    public ReaderClass
{
protected:
    virtual Expression* readExpression();
};

Expression* PrologReader::readExpression()
{
    // It might be a list
    if (*p_ == '(')
    {
        p_++;
        return readList();
    }
    else
    {
        // Otherwise it must be a symbol
        return new PrologValue(readSymbol());
    }
}


// -----------------------------------------------------------------------------
/// PrologContiuation
//    Continuations are new types of expressions
// -----------------------------------------------------------------------------
class Continuation
:
    public Expression
{
public:

    virtual int withContinuation(Continuation*);

    virtual void print()
    {
        std::cout<< "<future>";
    }

    virtual Continuation* isContinuation()
    {
        return this;
    }
};

int Continuation::withContinuation(Continuation* future)
{
    // Default is to always work
    return 1;
}
///- PrologContiuation

// The null continuation
static Expr nothingExpr;
static Continuation* nothing;


// -----------------------------------------------------------------------------
/// PrologComposeContinuation
//    Compose used in implementing and relation
// -----------------------------------------------------------------------------
class ComposeContinuation
:
    public Continuation
{
    Expr left;
    Expr right;

public:

    ComposeContinuation(Expression* a, Expression* b)
    {
        left = a;
        right = b;
    }

    virtual ~ComposeContinuation()
    {
        left = 0;
        right = 0;
    }

    virtual int withContinuation(Continuation*);
};

int ComposeContinuation::withContinuation(Continuation* future)
{
    Continuation* a = left()->isContinuation();
    Continuation* b = right()->isContinuation();
    if ((!a) || (!b))
    {
        error("and with non relations");
        return 0;
    }
    return a->withContinuation(b);
}
///- PrologComposeContinuation


// -----------------------------------------------------------------------------
/// PrologAndContinuation
// -----------------------------------------------------------------------------
class AndContinuation
:
    public Continuation
{
    List relArgs;

public:

    AndContinuation(ListNode* args)
    {
        relArgs = args;
    }

    virtual ~AndContinuation()
    {
        relArgs = 0;
    }

    virtual int withContinuation(Continuation*);
};

int AndContinuation::withContinuation(Continuation* future)
{
    ListNode* args;
    args = relArgs;
    Continuation* newrel = future;

    for (int i = args->length() - 1; i >= 0; i--)
    {
        newrel = new ComposeContinuation(args->at(i), newrel);
    }

    // Construct an Expr for the final continuation for GC
    Expr p(newrel);

    int result = newrel->withContinuation(nothing);

    return result;
}
///- PrologAndContinuation


// -----------------------------------------------------------------------------
/// PrologOrContinuation
// -----------------------------------------------------------------------------
class OrContinuation
:
    public Continuation
{
    List relArgs;

public:

    OrContinuation(ListNode* args)
    {
        relArgs = args;
    }

    virtual ~OrContinuation()
    {
        relArgs = 0;
    }

    virtual int withContinuation(Continuation*);
};

int OrContinuation::withContinuation(Continuation* future)
{
    ListNode* args;

    // Try each alternative in turn
    for (args = relArgs; !args->isNil(); args = args->tail())
    {
        Continuation* r = args->head()->isContinuation();
        if (!r)
        {
            error("or argument is non-relation");
            return 0;
        }
        if (r->withContinuation(future))
        {
            return 1;
        }
    }

    // Nothing worked
    return 0;
}
///- PrologOrContinuation


// -----------------------------------------------------------------------------
/// PrologUnify
// -----------------------------------------------------------------------------
static int unify(PrologValue* &c, PrologValue* a, PrologValue* b)
{
    // If either one is undefined, set it to the other
    if (a->isUndefined())
    {
        c = a;
        a->setIndirect(b);
        return 1;
    }
    else if (b->isUndefined())
    {
        c = b;
        b->setIndirect(a);
        return 1;
    }

    // If either one are indirect, run down chain
    PrologValue* indirval;
    indirval = a->indirectPtr();
    if (indirval)
    {
        return unify(c, indirval, b);
    }

    indirval = b->indirectPtr();
    if (indirval)
    {
        return unify(c, a, indirval);
    }

    // Both must now be symbolic, work if the same
    c = 0;
    Symbol* as = a->isSymbol();
    Symbol* bs = b->isSymbol();
    if ((!as) || (!bs))
    {
        error("impossible", "unification of non-symbols");
    }
    else if (as->name() == bs->name())
    {
        return 1;
    }
    return 0;
}
///- PrologUnify


// -----------------------------------------------------------------------------
/// PrologUnifyContinuation
// -----------------------------------------------------------------------------
class UnifyContinuation
:
    public Continuation
{
    Expr left;
    Expr right;

public:

    UnifyContinuation(Expression* a, Expression* b)
    {
        left = a;
        right = b;
    }

    virtual ~UnifyContinuation()
    {
        left = 0;
        right = 0;
    }

    virtual int withContinuation(Continuation*);
};

int UnifyContinuation::withContinuation(Continuation* future)
{
    PrologValue* a = left()->isPrologValue();
    PrologValue* b = right()->isPrologValue();

    // The following shouldn't ever happen, but check anyway
    if ((!a) || (!b))
    {
        error("impossible", "missing prolog values in unification");
        return 0;
    }

    // Now try unification
    PrologValue* c = 0;
    if (unify(c, a, b) && future->withContinuation(nothing))
    {
        return 1;
    }

    // Didn't work, undo assignment and fail
    if (c)
    {
        c->setUndefined();
    }

    return 0;
}
///- PrologUnifyContinuation


// -----------------------------------------------------------------------------
/// PrologPrintContinuation
//    The printing relation
// -----------------------------------------------------------------------------
class PrintContinuation
:
    public Continuation
{
    Expr val;

public:

    PrintContinuation(Expression* x)
    {
        val = x;
    }

    virtual ~PrintContinuation()
    {
        val = 0;
    }

    virtual int withContinuation(Continuation*);
};

int PrintContinuation::withContinuation(Continuation* future)
{
    // See if we are a symbol, if so print it out
    Symbol* s = val()->isSymbol();
    if (s)
    {
        std::cout<< s->name() << '\n';
        return future->withContinuation(nothing);
    }
    return 0;
}
///- PrologPrintContinuation


// -----------------------------------------------------------------------------
/// PrologUnifyOperation
//    The operations used when reading rules
// -----------------------------------------------------------------------------
class UnifyOperation
:
    public BinaryFunction
{
public:
    virtual void applyWithArgs(Expr& target, ListNode* args, Environment*)
    {
        target = new UnifyContinuation(args->at(0), args->at(1));
    }

};

class PrintOperation
:
    public UnaryFunction
{
public:
    virtual void applyWithArgs(Expr& target, ListNode* args, Environment*)
    {
        target = new PrintContinuation(args->at(0));
    }
};

class AndOperation
:
    public Function
{
public:
    virtual void applyWithArgs(Expr& target, ListNode* args, Environment*)
    {
        target = new AndContinuation(args);
    }
};

class OrOperation
:
    public Function
{
public:
    virtual void applyWithArgs(Expr& target, ListNode* args, Environment*)
    {
        target = new OrContinuation(args);
    }
};
///- PrologUnifyOperation


// -----------------------------------------------------------------------------
/// PrologQueryStatement
//    The query statement is used to ask questions
// -----------------------------------------------------------------------------
class QueryStatement
:
    public Function
{
public:
    virtual void apply(Expr&, ListNode*, Environment*);
};

void QueryStatement::apply(Expr& target, ListNode* args, Environment* rho)
{
    if (args->length() != 1)
    {
        target = error("wrong number of args to query");
        return;
    }

    // We make a new environment to isolate any new variables defined
    Env newrho(new Environment(emptyList, emptyList, rho));

    args->at(0)->eval(target, valueOps, newrho);

    Continuation* f = 0;
    if (target())
    {
        f = target()->isContinuation();
    }

    if (!f)
    {
        target = error("query given non-relation");
        return;
    }

    if (f->withContinuation(nothing))
    {
        target = new Symbol("ok");
    }
    else
    {
        target = new Symbol("not ok");
    }
}
///- PrologQueryStatement


// -----------------------------------------------------------------------------
/// PrologInitialize
// -----------------------------------------------------------------------------
ReaderClass* initialize()
{
    // Create the reader/parser
    ReaderClass* reader = new PrologReader;

    // Construct the "nothing" continuation
    nothing = new Continuation;
    nothingExpr = nothing;

    // Construct the operators that are legal inside of relations
    Environment* rops = valueOps;
    rops->add(new Symbol("print"), new PrintOperation);
    rops->add(new Symbol(":=:"), new UnifyOperation);
    rops->add(new Symbol("and"), new AndOperation);
    rops->add(new Symbol("or"), new OrOperation);

    // Initialize the commands environment
    Environment* cmds = commands;
    cmds->add(new Symbol("define"), new DefineStatement);
    cmds->add(new Symbol("query"), new QueryStatement);

    return reader;
}
///- PrologInitialize

// -----------------------------------------------------------------------------
