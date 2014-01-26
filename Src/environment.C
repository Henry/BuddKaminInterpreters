#include "environment.h"

//
//      Environment - an environment is built out of two parallel lists
//

Environment::Environment
(
    ListNode* names,
    ListNode* values,
    Environment* parent
)
:
    names_(names),
    values_(values),
    parent_(parent)
{}

Environment::~Environment()
{
    names_ = 0;
    values_ = 0;
}

Environment* Environment::isEnvironment()
{
    return this;
}

/// EnvironmentAdd
void Environment::add(Symbol* s, Expression* v)
{
    names_ = new ListNode(s, names_.operator ListNode*());
    values_ = new ListNode(v, values_.operator ListNode*());
}

void Environment::set(Symbol* sym, Expression* value)
{
    ListNode* nameit = names_;
    ListNode* valueit = values_;

    while (!nameit->isNil())
    {
        if (*sym == nameit->head())
        {
            valueit->head(value);
            return;
        }
        nameit = nameit->tail();
        valueit = valueit->tail();
    }

    // Otherwise see if we can find it on somebody elses list
    if (parent_)
    {
        parent_->set(sym, value);
        return;
    }

    // not found and we're the end of the line, just add
    add(sym, value);
}
///- EnvironmentAdd

/// EnvironmentLookup
Expression* Environment::lookup(const Symbol& sym)
{
    ListNode* nameit = names_;
    ListNode* valueit = values_;

    while (!nameit->isNil())
    {
        if (sym == nameit->head())
        {
            return valueit->head();
        }

        nameit = nameit->tail();
        valueit = valueit->tail();
    }

    // Otherwise see if we can find it on somebody elses list
    if (parent_)
    {
        return parent_->lookup(sym);
    }

    // Symbol not found, return nil value
    return 0;
}
///- EnvironmentLookup
