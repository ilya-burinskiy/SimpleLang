#include "relop.hpp"
#include "token.hpp"
#include "symbols.hpp"


Relop::Relop(Symbol tag, RelopType relopt): Token(tag), type{relopt}
{
}