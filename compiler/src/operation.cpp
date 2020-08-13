#include "operation.hpp"
#include "token.hpp"
#include "symbols.hpp"

using namespace Symbols;

Operation::Operation(Terminal tag, Op opt): Token(tag), optype{opt}
{
}