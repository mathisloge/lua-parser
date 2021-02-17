#include "chunk_def.hpp"
#include "chunk.hpp"
namespace sre::lua::parser::impl
{
    void initBinOp()
    {
        // clang-format off
        bin_op.add
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

    /**
     * @brief 
     * @see 3.1 – Lexical Conventions
     */
    void initKeywords()
    {
        // clang-format off
        keywords.add
            //("and")
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
            //("not")
            //("or")
            ("repeat")
            ("return")
            ("then")
            ("true")
            ("until")
            ("while");
        // clang-format on
    }

}
