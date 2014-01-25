#include <stdio.h>

#include "environment.h"
#include "function.h"
#include "list.h"

extern Env valueOps;

extern Expr trueExpr;
extern Expr falseExpr;

Function* Function::isFunction()
{
    return this;
}

void Function::print()
{
    printf("<closure>");
}

int Function::isClosure()
{
    return 0;
}

// Default behavior for function applications is to evaluate arguments

/// FunctionApply
static ListNode* evalArgs(ListNode* args, Environment* rho)
{
    if (args->isNil())
    {
        return args;
    }

    Expression* first = args->head();
    Expr newhead;
    first->eval(newhead, valueOps, rho);

    return new ListNode(newhead(), evalArgs(args->tail(), rho));
}

void Function::apply(Expr& target, ListNode* args, Environment* rho)
{
    // Hold the newargs as a List to ensure garbage collection
    List newargs(evalArgs(args, rho));
    applyWithArgs(target, newargs, rho);
}
///- FunctionApply

void Function::applyWithArgs(Expr& target, ListNode* args, Environment* rho)
{
    target = error("in function::applywithargs, should be overridden");
}

//
//      Unary functions take only one argument
//

void UnaryFunction::apply(Expr& target, ListNode* args, Environment* rho)
{
    if (args->length() != 1)
    {
        error("unary function given more than one argument");
        target = 0;
        return;
    }
    Function::apply(target, args, rho);
}

void UnaryFunction::applyWithArgs
(
    Expr& target,
    ListNode* args,
    Environment* rho
)
{
    if (!function_)
    {
        target = error("unaryfunction apply and no function");
    }
    else
    {
        function_(target, args->at(0));
    }
}

//
//      Binary functions take two arguments
//

void BinaryFunction::apply(Expr& target, ListNode* args, Environment* rho)
{
    if (args->length() != 2)
    {
        error("binary function given more than one argument");
        target = 0;
        return;
    }
    Function::apply(target, args, rho);
}

void BinaryFunction::applyWithArgs
(
    Expr& target,
    ListNode* args,
    Environment* rho
)
{
    if (!function_)
    {
        target = error("binary function apply and no function");
    }
    else
    {
        function_(target, args->at(0), args->at(1));
    }
}

//
//      Integer Binary Functions
//

/// IntegerBinaryFunctionApply
void IntegerBinaryFunction::applyWithArgs
(
    Expr& target,
    ListNode* args,
    Environment* rho
)
{
    Expression* left = args->at(0);
    Expression* right = args->at(1);
    if ((!left->isInteger()) || (!right->isInteger()))
    {
        target = error("arithmetic function with nonint args");
        return;
    }

    target = new IntegerExpression(function_(left->isInteger()->val(),
    right->isInteger()->val()));
}
///- IntegerBinaryFunctionApply

//
//      Boolean Binary Functions
//

/// BooleanBinaryFunctionApply
void BooleanBinaryFunction::applyWithArgs
(
    Expr& target,
    ListNode* args,
    Environment* rho
)
{
    Expression* left = args->at(0);
    Expression* right = args->at(1);
    if ((!left->isInteger()) || (!right->isInteger()))
    {
        error("arithmetic function with nonint args");
        return;
    }

    if (function_(left->isInteger()->val(), right->isInteger()->val()))
    {
        target = trueExpr();
    }
    else
    {
        target = falseExpr();
    }
}
///- BooleanBinaryFunctionApply

//
//      user functions have argument names and body
//

UserFunction::UserFunction(ListNode* anames, Expression* bod, Environment* ctx)
:
    argNames_(anames),
    body_(bod),
    context_(ctx)
{}

UserFunction::~UserFunction()
{
    argNames_ = 0;
    body_ = 0;
}

int UserFunction::isClosure()
{
    return 1;
}

/// UserFunctionApply
void UserFunction::applyWithArgs
(
    Expr& target,
    ListNode* args,
    Environment* rho
)
{
    // number of args should match definition
    ListNode* an = argNames_;
    if (an->length() != args->length())
    {
        target = error("argument length mismatch");
        return;
    }

    // make new environment
    Env newrho(new Environment(an, args, context_));

    // evaluate body in new environment
    Expression* bod = body_();
    if (bod)
    {
        bod->eval(target, valueOps, newrho);
    }
    else
    {
        target = 0;
    }
}
///- UserFunctionApply
