#include "word.hpp"
#include "symbols.hpp"
#include "token.hpp"
#include <string>

using namespace Symbols;
using std::string;

Word::Word(Terminal tag, const string& s): Token(tag), lexeme_(s)
{
}