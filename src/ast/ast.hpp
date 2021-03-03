#pragma once
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <list>
#include <map>
#include <string>

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
    struct local_function;
    struct functiondef;
    struct for_namelist;
    struct prefixexp;
    struct tableconstructor;
    struct var;
    struct functioncall;
    struct stat_functioncall;
    struct primaryexpression;
    struct prefixexp_container;
    struct varlist;
    using explist = std::list<expression>;
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
                     x3::forward_ast<functiondef>,
                     x3::forward_ast<tableconstructor>,
                     x3::forward_ast<binary>,
                     x3::forward_ast<unary>,
                     x3::forward_ast<expression>,
                     x3::forward_ast<primaryexpression>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct prefixexp : x3::variant<nil,
                                   x3::forward_ast<functioncall>,
                                   x3::forward_ast<var>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct primaryexpression
    {
        exp first_;
        prefixexp rest_;
    };

    struct var_assign_or_list : x3::variant<nil,
                                            x3::forward_ast<explist>, x3::forward_ast<varlist>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };
    struct assign_or_call
    {
        primaryexpression primaryexp_;
        var_assign_or_list var_action_;
    };
    struct args : x3::variant<std::string,
                              x3::forward_ast<tableconstructor>,
                              x3::forward_ast<explist>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct field
    {
        exp first;
        exp second;
    };
    using fieldlist = std::list<field>;

    struct tableconstructor
    {
        field first_;
        std::list<field> fields_;
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

    struct var : x3::variant<std::string, x3::forward_ast<exp>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };
    struct varlist
    {
        //var first_;
        std::list<var> rest_;
        explist explist_;
    };

    struct funcname
    {
        Name name_;
        std::list<Name> chain_;
        Name self_chain_;
    };

    struct stat : x3::variant<
                      nil,
                      label,
                      x3::forward_ast<assign_or_call>,
                      x3::forward_ast<function>,
                      x3::forward_ast<local_function>,
                      x3::forward_ast<for_namelist>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

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

    struct local_function
    {
        Name funcname_;
        parlist parameters_;
        block block_;
    };

    struct for_namelist
    {
        namelist name_list_;
        explist exp_list_;
        block block_;
    };

    struct functiondef
    {
        parlist parameters_;
        block block_;
    };

    struct functioncall
    {
        boost::optional<Name> name_;
        args args_;
        prefixexp prefix_exp_;
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
