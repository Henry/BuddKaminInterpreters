// -----------------------------------------------------------------------------
//  This file is part of
/// ---     Timothy Budd's Kamin Interpreters in C++
// -----------------------------------------------------------------------------
/// Title: Reader
///  Description:
//    Standard lisp reader
//    May be specialised for particular languages
// -----------------------------------------------------------------------------

#ifndef Reader_H
#define Reader_H

#include <string>

// -----------------------------------------------------------------------------
/// Forward declarations
// -----------------------------------------------------------------------------
class Expression;
class IntegerExpression;
class Symbol;
class ListNode;

// -----------------------------------------------------------------------------
/// ReaderClass
// -----------------------------------------------------------------------------
class ReaderClass
{
protected:

    //- The input buffer
    std::string buffer_;

    //- Current location in buffer
    const char* p_;

    //- Print prompt
    void printPrimaryPrompt() const;

    //- Print secondary prompt
    void printSecondaryPrompt() const;

    //- Read input into buffer
    void fillInputBuffer();

    //- Separator predicate
    int isSeparator(int) const;

    //- Skip spaces
    void skipSpaces();

    //- Skip new lines
    void skipNewlines();

    //- Read integer
    int readInteger();

    //- Read symbol
    Symbol* readSymbol();

    //- Read List
    ListNode* readList();

    //- Read expression function that can be overridden for particular languages
    virtual Expression* readExpression();

public:

    //- Print prompt and read next statement
    Expression* promptAndRead();
};
///- ReaderClass

// -----------------------------------------------------------------------------
#endif // Reader_H
// -----------------------------------------------------------------------------
