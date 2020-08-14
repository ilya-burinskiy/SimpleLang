#include "operation.hpp"
#include "token.hpp"
#include "symbols.hpp"


Operation::Operation(Symbol tag, Op opt): Token(tag), optype{opt}
{
}