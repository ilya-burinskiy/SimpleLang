#include <iostream>
#include <string>

#include "token.hpp"
#include "symbols.hpp"

using std::ostream;
using std::string;

Token::Token(Symbol t): term{t} 
{
}

Word::Word(Symbol tag, const string& s): Token(tag), lexeme{s}
{
}


Num::Num(Symbol tag, short v): Token(tag), val{v}
{
}

Relop::Relop(Symbol tag, RelopType relopt): Token(tag), type{relopt}
{
}