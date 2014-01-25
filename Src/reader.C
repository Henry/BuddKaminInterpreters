#include <cctype>
#include <iostream>

#include "expression.h"
#include "list.h"
#include "reader.h"

extern List emptyList;

void ReaderClass::printPrimaryPrompt() const
{
    std::cout<< "\n-> " << std::flush;
}

void ReaderClass::printSecondaryPrompt() const
{
    std::cout<< "> " << std::flush;
}

void ReaderClass::fillInputBuffer()
{
    std::getline(std::cin, buffer_);

    if (std::cin.eof())
    {
        buffer_ += "quit";
    }

    // Initialize the current pointer
    p_ = buffer_.c_str();

    skipSpaces();
}

int ReaderClass::isSeparator(int c) const
{
    switch (c)
    {
        case ' ':
        case '\t':
        case '\n':
        case '\0':
        case '\'':
        case ';':
        case ')':
        case '(':
            return 1;
    }
    return 0;
}

void ReaderClass::skipSpaces()
{
    while ((*p_ == ' ') || (*p_ == '\t'))
    {
        p_++;
    }

    if (*p_ == ';')      // comment
    {
        while (*p_)
        {
            p_++;        // read until end of line
        }
    }
}

void ReaderClass::skipNewlines()
{
    skipSpaces();
    while (*p_ == '\0')
    {
        // end of line
        printSecondaryPrompt();
        fillInputBuffer();
    }
}

/// ReaderPromptAndRead
Expression* ReaderClass::promptAndRead()
{
    // loop until the user types something
    do
    {
        printPrimaryPrompt();
        fillInputBuffer();
    } while (!*p_);

    // now that we have something, break it apart
    Expression* val = readExpression();

    // make sure we are at and of line
    skipSpaces();
    if (*p_)
    {
        error("unexpected characters at end of line:", p_);
    }
    return val;
}
///- ReaderPromptAndRead

/// ReaderReadExpression
Expression* ReaderClass::readExpression()
{
    // see if it's an integer
    if (isdigit(*p_))
    {
        return new IntegerExpression(readInteger());
    }

    // might be a signed integer
    if ((*p_ == '-') && isdigit(*(p_ + 1)))
    {
        p_++;
        return new IntegerExpression(-readInteger());
    }

    // or it might be a list
    if (*p_ == '(')
    {
        p_++;
        return readList();
    }

    // otherwise it must be a symbol
    return readSymbol();
}

ListNode* ReaderClass::readList()
{
    // skipNewlines will issue secondary prompt
    // until a valid character is typed
    skipNewlines();

    // if end of list, return empty list
    if (*p_ == ')')
    {
        p_++;
        return emptyList;
    }

    // now we have a non-empty character
    Expression* val = readExpression();
    return new ListNode(val, readList());
}
///- ReaderReadExpression

int ReaderClass::readInteger()
{
    int val = 0;
    while (isdigit(*p_))
    {
        val = val* 10 + (*p_ - '0');
        p_++;
    }
    return val;
}

Symbol* ReaderClass::readSymbol()
{
    const char* symbolStart = p_;
    int nSymbolChars = 0;

    while (!isSeparator(*p_))
    {
        nSymbolChars++;
        p_++;
    }

    return new Symbol(std::string(symbolStart, nSymbolChars));
}
