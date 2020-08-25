#include "num.hpp"
#include "symbols.hpp"
#include "token.hpp"


Num::Num(Symbol tag, short v): Token(tag), val{v}
{
}