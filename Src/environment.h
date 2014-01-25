// -----------------------------------------------------------------------------
//  This file is part of
/// ---     Timothy Budd's Kamin Interpreters in C++
// -----------------------------------------------------------------------------
/// Title: The Environment
///  Description:
//    Environment holds the symbols, values and link to parent environment.
//    Env is a reference-counting wrapper around Environment for GC
// -----------------------------------------------------------------------------

#ifndef Environment_H
#define Environment_H

#include "list.h"

// -----------------------------------------------------------------------------
/// Forward declarations
// -----------------------------------------------------------------------------
class Symbol;

// -----------------------------------------------------------------------------
/// Environment
// -----------------------------------------------------------------------------
class Env
:
    public Expr
{
public:

    inline Env();
    inline Env(Environment* r);
    inline operator Environment*();
    inline void operator=(Environment* r);
};

class Environment
:
    public Expression
{
    //- Symbols
    List names_;

    //- Values associated with the symbols
    List values_;

    //- Link to parent environment
    Environment* parent_;

public:

    //- Construct from components
    Environment(ListNode*, ListNode*, Environment*);

    //- Delete according to reference counts
    virtual ~Environment();

    //- Specialised type predicate
    virtual Environment* isEnvironment();

    //- Lookup symbol in
    Expression* lookup(const Symbol*);

    //- Add symbol with expression to the environment
    void add(Symbol*, Expression*);

    //- Set symbol to expression in the environment
    void set(Symbol*, Expression*);
};
///- Environment

// -----------------------------------------------------------------------------
/// Member functions for class Env
// -----------------------------------------------------------------------------

inline Env::Env()
:
    Expr(0)
{}

inline Env::Env(Environment* r)
{
    Expr::operator=(r);
}

inline Env::operator Environment*()
{
    return val() ? val()->isEnvironment() : 0;
}

inline void Env::operator=(Environment* r)
{
    Expr::operator=(r);
}

// -----------------------------------------------------------------------------
#endif // Environment_H
// -----------------------------------------------------------------------------
