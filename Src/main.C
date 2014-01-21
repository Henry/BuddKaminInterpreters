//
// main program for c++ versions of kamin interpreters
//

#include "environment.h"
#include "reader.h"

//      forward definitions
extern void initialize();

//      definitions of global environments

Env globalEnvironment;          // global symbols
Env valueOps;                   // built-in operations
Env commands;                   // top level commands

List emptyList;

//      the following globals are defined in the ``initialization'' routine
ReaderClass* reader;
Expr trueExpr;
Expr falseExpr;

/// main
int main()
{
    Expr entered;               // expression as entered by users

    // common initialization
    emptyList = new ListNode(0, 0);
    globalEnvironment = new Environment(emptyList, emptyList, 0);
    valueOps = new Environment(emptyList, emptyList, 0);
    commands = new Environment(emptyList, emptyList, valueOps);

    // interpreter-specific initialization
    initialize();

    // now the read-eval-print loop
    while (1)
    {
        entered = reader->promptAndRead();

        // now see if expression is quit
        Symbol* sym = entered()->isSymbol();
        if (sym && (*sym == "quit"))
        {
            break;
        }

        // nothing else, must just be an expression
        entered.evalAndPrint(commands, globalEnvironment);
    }

    delete reader;

    commands = 0;
    delete commands;
    valueOps = 0;
    delete valueOps;
    globalEnvironment = 0;
    delete globalEnvironment;
    emptyList = 0;
    delete emptyList;

    return 0;
}
///- main
