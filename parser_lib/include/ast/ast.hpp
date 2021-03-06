#pragma once
#include <list>
#include <map>
#include <string>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace sre::lua::ast
{
namespace x3 = boost::spirit::x3;

struct nil
{};

inline std::ostream &operator<<(std::ostream &out, nil)
{
    out << "nil";
    return out;
}

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

// struct statement;
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
struct var_wrapper;
struct functioncall;
struct stat_functioncall;
struct primaryexpression;
struct prefixexp_container;
struct varlist;
struct ifelse;
using explist = std::list<expression>;
struct Name
{
    Name(std::string const &name = "")
        : name(name)
    {}
    std::string name;
};

enum keyword
{
    keyword_nil,
    keyword_expand, //! ...
    keyword_break
};

struct keyword_stmt
{
    keyword keyword_;
};

struct numeral : x3::variant<double, unsigned>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct exp :
    x3::variant<nil,
                keyword_stmt, // nil | ...
                bool,
                numeral,
                std::string,
                Name, // only needed for prefixexp_expr since as<ast::exp>(name) will return Name and not std::string on linux
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
struct binary : x3::position_tagged
{
    // exp lhs_;
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

struct prefixexp : x3::variant<nil, x3::forward_ast<functioncall>, x3::forward_ast<var_wrapper>>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct primaryexpression
{
    exp first_;
    prefixexp rest_;
};

struct var : x3::variant<Name, exp>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct var_wrapper
{
    var var_;
    prefixexp next_;
};
struct varlist
{
    // var first_;
    std::list<var> rest_;
    explist explist_;
};
struct namelist
{
    Name name_;
    std::list<Name> chain_;
};

struct var_assign_or_list : x3::variant<nil, explist, varlist>
{
    using base_type::base_type;
    using base_type::operator=;
};
struct assign_or_call
{
    primaryexpression primaryexp_;
    var_assign_or_list var_action_;
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
    fieldlist fields_;
};
struct args : x3::variant<std::string, tableconstructor, explist>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct label
{
    Name name_;
};

struct funcname
{
    namelist names_;
    Name self_chain_;
};

struct name_attrib_pair
{
    Name name_;
    boost::optional<Name> attrib_;
};
struct attnamelist
{
    name_attrib_pair first_;
    std::list<name_attrib_pair> rest_;
};

struct whiledo;
struct repeatuntil;
struct doblock;
struct forexp;
struct local_attnamelist_assign;
struct goto_stmt
{
    Name name_;
};
struct stat :
    x3::variant<nil,
                label,
                keyword_stmt, //! break
                goto_stmt,
                x3::forward_ast<assign_or_call>,
                x3::forward_ast<function>,
                x3::forward_ast<local_function>,
                x3::forward_ast<for_namelist>,
                x3::forward_ast<repeatuntil>,
                x3::forward_ast<doblock>,
                x3::forward_ast<whiledo>,
                x3::forward_ast<ifelse>,
                x3::forward_ast<forexp>,
                x3::forward_ast<local_attnamelist_assign>>
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

using parlist = namelist;
struct funcbody
{
    parlist parameters_;
    block block_;
};

struct function
{
    funcname funcname_;
    funcbody funcbody_;
};

struct local_function
{
    Name funcname_;
    funcbody funcbody_;
};

struct for_namelist
{
    namelist name_list_;
    explist exp_list_;
    block block_;
};

struct functiondef
{
    funcbody funcbody_;
};

struct functioncall
{
    boost::optional<Name> name_;
    args args_;
    prefixexp prefix_exp_;
};

struct ifelse_wrapper
{
    exp exp_;
    block block_;
};

struct ifelse
{
    ifelse_wrapper first_;
    std::list<ifelse_wrapper> rest_;
    boost::optional<block> else_;
};

struct whiledo
{
    exp exp_;
    block block_;
};
struct repeatuntil
{

    block block_;
    exp exp_;
};

struct doblock
{
    block block_;
};

struct forexp
{
    Name name_;
    exp exp_first_;
    exp exp_second_;
    exp exp_third_;
    block block_;
};

struct local_attnamelist_assign
{
    attnamelist attnamelist_;
    explist explist_;
};

using boost::fusion::operator<<;

inline const std::map<optoken, std::string> kOptokenSymbols = {
    {op_plus, "+"},           {op_minus, "-"},       {op_times, "*"},   {op_divide, "/"},   {op_floor_divide, "//"},
    {op_exponentiation, "^"}, {op_modulo, "%"},      {op_and, "&"},     {op_xor, "~"},      {op_or, "|"},
    {op_right, ">>"},         {op_left, "<<"},       {op_concat, ".."}, {op_less, "<"},     {op_less_eq, "<="},
    {op_greater, ">"},        {op_greater_eq, ">="}, {op_equal, "=="},  {op_inequal, "~="}, {op_and_key, "and"},
    {op_or_key, "or"},        {op_not, "not"},       {op_prefix, "#"}};

} // namespace sre::lua::ast
