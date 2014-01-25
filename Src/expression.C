#include "expression.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//
//      class Expr - expression holders
//

/// ExprAssign
Expr::Expr(Expression* val)
:
    value_(val)
{
    if (value_)
    {
        value_->referenceCount++;
    }
}

Expr::Expr()
:
    value_(0)
{}

Expr::Expr(Expr& val)
:
    value_(val.value_)
{
    if (value_)
    {
        value_->referenceCount++;
    }
}

Expr::~Expr()
{
    operator=(0);
}

void Expr::operator=(Expression* newvalue)
{
    // increment right hand side of assignment
    if (newvalue)
    {
        newvalue->referenceCount++;
    }

    // decrement left hand side of assignment
    if (value_)
    {
        value_->referenceCount--;
        if (value_->referenceCount == 0)
        {
            delete value_;
        }
    }

    // then do the assignment
    value_ = newvalue;
}
///- ExprAssign

void Expr::evalAndPrint(Environment* valueops, Environment* rho)
{
    Expr target(0);

    // if we have a valid expression, evaluate it
    if (value_)
    {
        value_->eval(target, valueops, rho);
    }

    // Now if we have an expression, print it out
    if (target())
    {
        target()->print();
    }
}

#if 0
void Expr::print()
{
    if (value_)
    {
        value_->print();
    }
    printf("\n");
}
#endif

//
//      Expression - internal representation for expressions
//

Expression::Expression()
{
    referenceCount = 0;
}

Expression::~Expression()
{
    // do nothing
}

void Expression::eval(Expr& target, Environment* valueops, Environment* rho)
{
    // default is to do nothing
    target = this;
}

void Expression::print()
{
    fprintf(stderr, "in expression::print - should be subclassed\n");
}

// conversions

Expression* Expression::touch()
{
    return this;
}

IntegerExpression* Expression::isInteger()
{
    return 0;
}

Symbol* Expression::isSymbol()
{
    return 0;
}

ListNode* Expression::isList()
{
    return 0;
}

Environment* Expression::isEnvironment()
{
    return 0;
}

Function* Expression::isFunction()
{
    return 0;
}

APLValue* Expression::isAPLValue()
{
    return 0;
}

Method* Expression::isMethod()
{
    return 0;
}

Environment* Expression::isCluster()
{
    return 0;
}

PrologValue* Expression::isPrologValue()
{
    return 0;
}

Continuation* Expression::isContinuation()
{
    return 0;
}

//
//      basic objects - integers and symbols
//

#include "environment.h"

//
//      integers
//

void IntegerExpression::print()
{
    printf("%d", value_);
}

IntegerExpression* IntegerExpression::isInteger()
{
    return this;
}

//
//      symbols
//

Symbol::Symbol(const char* t)
{
    // make a new copy of text
    name_ = new char[strlen(t) + 1];
    if (!name_)
    {
        error("allocation failure for symbol ", t);
        exit(1);
    }
    strcpy(name_, t);
}

Symbol::~Symbol()
{
    delete[] name_;
}

void Symbol::eval(Expr& target, Environment* valueops, Environment* rho)
{
    Expression* result = rho->lookup(this);
    if (result)
    {
        result = result->touch();
    }
    else
    {
        result = error("evaluation of unknown symbol: ", name_);
    }
    target = result;
}

void Symbol::print()
{
    printf("%s", name_);
}

Symbol* Symbol::isSymbol()
{
    return this;
}

int Symbol::operator==(Expression* sym) const
{
    if (!sym)
    {
        return 0;
    }

    const Symbol* s = sym->isSymbol();

    if (s)
    {
        return 0 == strcmp(name_, s->name_);
    }

    return 0;
}

int Symbol::operator==(const char* t) const
{
    return 0 == strcmp(name_, t);
}
