#pragma once
#include <string>
#include <list>
#include <map>
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
    //struct statement;
    struct expression;
    struct binary;
    struct unary;
    struct function;
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

    struct binary : x3::position_tagged
    {
        //exp lhs_;
        optoken operator_;
        exp rhs_;
    };
    struct unary : x3::position_tagged
    {
        optoken operator_;
        exp rhs_;
    };

    struct expression : x3::position_tagged
    {
        exp first_;
        exp rest_;
    };

    struct label : Name
    {
        using Name::Name;
    };

    struct var : Name
    {
        using Name::Name;
    };

    struct funcname
    {
        Name name_;
        std::list<Name> chain_;
        Name self_chain_;
    };

    struct varlist
    {
        std::list<var> vars_;
    };

    struct stat : x3::variant<
                      nil,
                      label,
                      boost::recursive_wrapper<function> //! needed since function depends on block and block on statement and statement on function...
                      >
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    using explist = std::list<expression>;
    using retstat = explist;

    struct block : x3::position_tagged
    {
        std::list<stat> stat_;
        retstat retstat_;
    };
    struct chunk : x3::position_tagged
    {
        block block_;
    };
    using namelist = std::list<Name>;
    using parlist = namelist;
    struct function
    {
        funcname funcname_;
        parlist parameters_;
        block block_;
    };

    using boost::fusion::operator<<;

    inline const std::map<optoken, std::string> kOptokenSymbols = {
        {op_plus, "+"},
        {op_minus, "-"},
        {op_times, "*"},
        {op_divide, "/"},
        {op_floor_divide, "//"},
        {op_exponentiation, "^"},
        {op_modulo, "%"},
        {op_and, "&"},
        {op_xor, "~"},
        {op_or, "|"},
        {op_right, ">>"},
        {op_left, "<<"},
        {op_concat, ".."},
        {op_less, "<"},
        {op_less_eq, "<="},
        {op_greater, ">"},
        {op_greater_eq, ">="},
        {op_equal, "=="},
        {op_inequal, "~="},
        {op_and_key, "and"},
        {op_or_key, "or"},
        {op_not, "not"},
        {op_prefix, "#"}};

}
