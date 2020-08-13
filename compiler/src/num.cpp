#include "num.hpp"
#include "symbols.hpp"
#include "token.hpp"

using Symbols::Terminal;

Num::Num(Terminal tag, int v): Token(tag), val{v}
{
}