#include "base_def.hpp"
namespace sre::lua::parser
{
namespace impl
{

/**
 * @brief
 * @see 3.1 – Lexical Conventions
 */
void initKeywords()
{
    // clang-format off
    keywords.add
        ("and")
        ("break")
        ("do")
        ("else")
        ("elseif")
        ("end")
        ("false")
        ("for")
        ("function")
        ("goto")
        ("if")
        ("in")
        ("local")
        ("nil")
        ("not")
        ("or")
        ("repeat")
        ("return")
        ("then")
        ("true")
        ("until")
        ("while");
    // clang-format on

    keyword_break.add("break", ast::keyword_break);
    keyword_nil.add("nil", ast::keyword_nil)("...", ast::keyword_expand);
}

void initUnaryOp()
{
    // clang-format off
    unary_op.add
        ("-", ast::op_minus)
        ("not", ast::op_not)
        ("#", ast::op_prefix)
        ("~", ast::op_xor);
    // clang-format on
}

void initBinOp()
{
    // clang-format off
    binary_op.add
        ("+", ast::op_plus)
        ("-", ast::op_minus)
        ("*", ast::op_times)
        ("/", ast::op_divide)
        ("//", ast::op_floor_divide)
        ("^", ast::op_exponentiation)
        ("%", ast::op_modulo)
        ("&", ast::op_and)
        ("~", ast::op_xor)
        ("|", ast::op_or)
        (">>", ast::op_right)
        ("<<", ast::op_left)
        ("..", ast::op_concat)
        ("<", ast::op_less)
        ("<=", ast::op_less_eq)
        (">", ast::op_greater)
        (">=", ast::op_greater_eq)
        ("==", ast::op_equal)
        ("~=", ast::op_inequal)
        ("and", ast::op_and_key)
        ("or", ast::op_or_key);
    // clang-format on
}

} // namespace impl
} // namespace sre::lua::parser
