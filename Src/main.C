//
// main program for c++ versions of kamin interpreters
//

#include "environment.h"
#include "reader.h"
#include <stdio.h>

// Forward definitions
extern ReaderClass* initialize();

// Definitions of global environments
List emptyList;
Env globalEnvironment;          // global symbols
Env valueOps;                   // built-in operations
Env commands;                   // top level commands

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
    reader = initialize();

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

    // Delete the dynamically-allocated reader
    delete reader;

    // Explicitly free the globalEnvironment
    // because there is a reference counting problem with it
    // globalEnvironment.operator Environment*()->free();
    // delete globalEnvironment.operator Environment*();

    return 0;
}
///- main
