#ifndef READER_H
#define READER_H

// Forward declarations
class Expression;
class IntegerExpression;
class Symbol;
class ListNode;

/// ReaderClass
class ReaderClass
{
public:

    Expression* promptAndRead();

protected:

    static const int bufferSize = 1000;
    char buffer[bufferSize];          // The input buffer
    char* p;                          // Current location in buffer

    // General functions
    void printPrimaryPrompt() const;
    void printSecondaryPrompt() const;
    char* removeNewline(char* s) const;
    void fillInputBuffer();
    int isSeparator(int) const;
    void skipSpaces();
    void skipNewlines();

    // Functions that can be overridden
    virtual Expression* readExpression();

    // Specific types of expressions
    int readInteger();
    Symbol* readSymbol();
    ListNode* readList();
};
///- ReaderClass

#endif
