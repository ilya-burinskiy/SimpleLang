#include "num.hpp"
#include "symbols.hpp"
#include "token.hpp"


Num::Num(Symbol tag, int v): Token(tag), val{v}
{
}