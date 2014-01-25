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

    //- Input buffer size
    static const int bufferSize = 1000;

    //- The input buffer
    char buffer[bufferSize];

    //- Current location in buffer
    char* p;

    //- Print prompt
    void printPrimaryPrompt() const;

    //- Print secondary prompt
    void printSecondaryPrompt() const;

    //- Remove \n
    char* removeNewline(char* s) const;

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
