#pragma once
#include <string>
#include <list>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/io.hpp>

namespace sre::lua::ast
{
    namespace x3 = boost::spirit::x3;

    // clang-format off
    struct nil { };
    // clang-format on

    inline std::ostream &operator<<(std::ostream &out, nil)
    {
        out << "nil";
        return out;
    }
    struct chunk;
    //struct block;
    //struct statement;
    struct expression;
    struct binary;
    struct unary;
    //struct functiondef;
    //struct prefixexp;
    //struct tableconstructor;

    struct Name : x3::position_tagged
    {
        Name(std::string const &name = "") : name(name) {}
        std::string name;
    };

    struct exp : x3::variant<
                     nil,
                     bool,
                     double,
                     std::string,
                     // ...?
                     //x3::forward_ast<functiondef>,
                     //x3::forward_ast<prefixexp>,
                     //x3::forward_ast<tableconstructor>,
                     x3::forward_ast<binary>,
                     x3::forward_ast<unary>,
                     x3::forward_ast<expression>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    enum optoken
    {
        op_plus,           // => +
        op_minus,          // => -
        op_times,          // => *
        op_divide,         // => /
        op_floor_divide,   // => //
        op_exponentiation, // => ^
        op_modulo,         // => %
        op_and,            // => &
        op_xor,            // => ~
        op_or,             // => |
        op_right,          // => >>
        op_left,           // => <<
        op_concat,         // => ..
        op_less,           // => <
        op_less_eq,        // => <=
        op_greater,        // => >
        op_greater_eq,     // => >=
        op_equal,          // => ==
        op_inequal,        // => ~=
        op_and_key,        // => and
        op_or_key,         // => or
        op_not,            // => not
        op_prefix          // => #
    };

    struct binary
    {
        //exp lhs_;
        optoken operator_;
        exp rhs_;
    };
    struct unary
    {
        optoken operator_;
        exp rhs_;
    };

    struct expression
    {
        exp first_;
        exp rest_;
    };

    //struct label
    //{
    //    Name name;
    //};

    //struct expression : x3::position_tagged
    //{
    //    operand op_;
    //};

    using explist = std::list<expression>;

    using retstat = explist;

    //struct statement : x3::variant<label>
    //{
    //    using base_type::base_type;
    //    using base_type::operator=;
    //};

    //struct block
    //{
    //    std::list<statement> stat_;
    //    retstat retstat_;
    //};

    struct stat
    {
        std::string test;
    };

    struct block
    {
        std::list<stat> stat_;
        retstat retstat_;
    };
    struct chunk
    {
        block block_;
    };

    using boost::fusion::operator<<;

}
