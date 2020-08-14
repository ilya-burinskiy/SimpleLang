#include "word.hpp"
#include "symbols.hpp"
#include "token.hpp"
#include <string>

using std::string;

Word::Word(Symbol tag, const string& s): Token(tag), lexeme{s}
{
}