//
// main program for c++ versions of kamin interpreters
//

#include "environment.h"
#include "reader.h"

// Forward definitions
extern void initialize();

// Definitions of global environments

Env globalEnvironment;          // global symbols
Env valueOps;                   // built-in operations
Env commands;                   // top level commands

List emptyList;

// The following globals are defined in the ``initialization'' routine
ReaderClass* reader;
Expr trueExpr;
Expr falseExpr;

/// main
int main()
{
    Expr entered;               // expression as entered by users

    // Common initialization
    emptyList = new ListNode(0, 0);
    globalEnvironment = new Environment(emptyList, emptyList, 0);
    valueOps = new Environment(emptyList, emptyList, 0);
    commands = new Environment(emptyList, emptyList, valueOps);

    // Interpreter-specific initialization (sets reader pointer)
    initialize();

    // Now the read-eval-print loop
    while (1)
    {
        entered = reader->promptAndRead();

        // Now see if expression is quit
        Symbol* sym = entered()->isSymbol();
        if (sym && (*sym == "quit"))
        {
            printf("\n");
            break;
        }

        // Nothing else, must just be an expression
        entered.evalAndPrint(commands, globalEnvironment);
    }

    // Unset all reference-counted object to cause destruction
    entered = 0;
    commands = 0;
    valueOps = 0;
    globalEnvironment = 0;
    emptyList = 0;

    // Delete the dynamically-allocated reader
    delete reader;

    return 0;
}
///- main
