#pragma once
#include <iomanip>
#include <iostream>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/transform_attribute.hpp>

#include "base_def.hpp"

#include "ast/ast.hpp"
#include "ast/ast_adapted.hpp"
#include "chunk.hpp"
namespace sre::lua::parser
{
namespace impl
{

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;
using x3::alnum;
using x3::alpha;
using x3::bool_;
using x3::char_;
using x3::eoi;
using x3::eol;
using x3::lexeme;
using x3::lit;
using x3::raw;

inline constexpr x3::rule<struct label_expr_class, ast::label> label_expr{"label_expr"};

inline constexpr x3::rule<struct ifelse_expr_class, ast::ifelse> ifelse_expr{"ifelse_expr"};

inline constexpr x3::rule<struct args_expr_class, ast::args> args_expr{"args_expr"};

inline constexpr x3::rule<struct functioncall_expr_class, ast::functioncall> functioncall_expr{"functioncall_expr"};

inline constexpr x3::rule<struct funcname_expr_class, ast::funcname> funcname_expr{"funcname_expr"};
inline constexpr x3::rule<struct function_expr_class, ast::function> function_expr{"function_expr"};
inline constexpr x3::rule<struct local_function_expr_class, ast::local_function> local_function_expr{"local_function_expr"};
inline constexpr x3::rule<struct functiondef_expr_class, ast::functiondef> functiondef_expr{"functiondef_expr"};

inline constexpr x3::rule<struct for_namelist_expr_class, ast::for_namelist> for_namelist_expr{"for_namelist_expr"};

inline constexpr x3::rule<struct field_expr_class, ast::field> field_expr{"field_expr"};
inline constexpr x3::rule<struct tableconstructor_expr_class, ast::tableconstructor> tableconstructor_expr{
    "tableconstructor_expr"};

inline constexpr x3::rule<struct var_expr_class, ast::var> var_expr{"var_expr"};
inline constexpr x3::rule<struct assignment_or_call_expr_class, ast::assign_or_call> assignment_or_call_expr{
    "assignment_or_call_expr"};
inline constexpr x3::rule<struct varlist_expr_class, ast::varlist> varlist_expr{"varlist_expr"};

inline constexpr x3::rule<struct namelist_expr_class, ast::namelist> namelist_expr{"namelist_expr"};
inline constexpr x3::rule<struct parlist_expr_class, ast::namelist> parlist_expr{"parlist_expr"};

inline constexpr x3::rule<struct exp_expr_class, ast::expression> exp_expr{"exp_expr"};
inline constexpr x3::rule<struct exp_sec_expr_class, ast::exp> exp_sec_expr{"exp_sec_expr"};

inline constexpr x3::rule<struct var_assign_expr_class, ast::explist> var_assign_expr{"var_assign_expr"};
inline constexpr x3::rule<struct primaryexp_expr_class, ast::primaryexpression> primaryexp_expr{"primaryexp_expr"};
inline constexpr x3::rule<struct prefixexp_expr_class, ast::exp> prefixexp_expr{"prefixexp_expr"};
inline constexpr x3::rule<struct prefixexp_sec_expr_class, ast::prefixexp> prefixexp_sec_expr{"prefixexp_sec_expr"};

inline constexpr x3::rule<struct binary_expr_class, ast::binary> binary_expr{"binary_expr"};
inline constexpr x3::rule<struct unary_expr_class, ast::unary> unary_expr{"unary_expr"};

inline constexpr x3::rule<struct whiledo_class, ast::whiledo> whiledo_expr{"whiledo_expr"};
inline constexpr x3::rule<struct repeatuntil_class, ast::repeatuntil> repeatuntil_expr{"repeatuntil_expr"};
inline constexpr x3::rule<struct doblock_class, ast::doblock> doblock_expr{"doblock_expr"};
inline constexpr x3::rule<struct forexp_class, ast::forexp> forexp_expr{"forexp_expr"};
inline constexpr x3::rule<struct attnamelist_expr_class, ast::attnamelist> attnamelist_expr{"attnamelist_expr"};
inline constexpr x3::rule<struct local_attnamelist_assign_class, ast::local_attnamelist_assign> local_attnamelist_assign_expr{
    "local_attnamelist_assign_expr"};

inline constexpr x3::rule<struct retstat_expr_class, ast::explist> retstat_expr{"retstat_expr"};
inline constexpr x3::rule<struct stat_expr_class, ast::stat> stat_expr{"stat_expr"};
inline constexpr x3::rule<struct block_expr_class, ast::block> block_expr{"block_expr"};
inline constexpr chunk_type chunk_expr{"chunk_expr"};

//  const auto nil_expr_def = lit("nil");


//! label ::= ‘::’ Name ‘::’
const auto label_expr_def = lit("::") >> name_expr >> lit("::");

//! if exp then block {elseif exp then block} [else block] end
const auto ifelse_body_expr = as<ast::ifelse_wrapper>(exp_expr >> lit("then") >> block_expr);
const auto ifelse_expr_def = lit("if") >> ifelse_body_expr >> *(lit("elseif") >> ifelse_body_expr) >
                             -(lit("else") >> block_expr) >> lit("end");

//! funcname ::= Name {‘.’ Name} [‘:’ Name]
const auto funcname_expr_def = as<ast::namelist>(name_expr >> *(lit(".") >> name_expr)) >> -(lit(':') >> name_expr);
//! namelist ::= Name {‘,’ Name}
const auto namelist_expr_def = name_expr >> *(lit(',') >> name_expr);
//! parlist ::= namelist [‘,’ ‘...’] | ‘...’
const auto parlist_expr_def = (namelist_expr >> -(lit(',') >> lit("..."))) | lit("...");
//! funcbody ::= ‘(’ [parlist] ‘)’ block end
const auto funcbody = lit('(') >> -parlist_expr >> lit(')') >> block_expr >> lit("end");
//! function funcname funcbody
const auto function_expr_def = lit("function") >> funcname_expr >> funcbody;
//! local function Name funcbody
const auto local_function_expr_def = lit("local") >> lit("function") >> name_expr >> funcbody;
//! functiondef ::= function funcbody
const auto functiondef_expr_def = lit("function") >> funcbody;

//! explist ::= exp {‘,’ exp}
const auto explist_expr = as<ast::explist>(exp_expr >> *(lit(',') >> exp_expr));
//! args ::=  ‘(’ [explist] ‘)’ | tableconstructor | LiteralString
const auto args_expr_def =
    as<ast::explist>(lit('(') >> -explist_expr >> lit(')')) | tableconstructor_expr | literal_str_expr;
//! prefixexp is left recursive. So change it to a right recursive rule.
//! P = prefixexp
//! V = var
//! F = functioncall
//! P ::= V | F | ‘(’ exp ‘)’
//! F::=  P args | P‘:’ Name args
//! V::=  Name | P‘[’ exp ‘]’ | P‘.’ Name
//! P ::= Name | P‘[’ exp ‘]’ | P‘.’ Name || P args | P‘:’ Name args  | ‘(’ exp ‘)’
//!        V        P
//! P = (Name | ‘(’ exp ‘)’) P'
//!            V              V          F             F
//! P'= ‘[’ exp ‘]’ P' | ‘.’ Name P' | args P' | ‘:’ Name args P'| e
const auto prefixexp_sec_expr_def = -(functioncall_expr | (var_expr >> prefixexp_sec_expr));
//! prefixexp ::= var | functioncall | ‘(’ exp ‘)’
const auto prefixexp_expr_def = (as<ast::exp>(name_expr) | as<ast::exp>(lit('(') >> exp_expr >> lit(')')));

const auto primaryexp_expr_def = prefixexp_expr >> prefixexp_sec_expr;

//! functioncall ::=  prefixexp args | prefixexp ‘:’ Name args
const auto functioncall_expr_def = -(lit(':') >> name_expr) >> args_expr >> prefixexp_sec_expr;

//! var ::=  Name | prefixexp ‘[’ exp ‘]’ | prefixexp ‘.’ Name
//! Name is directly in var list since the function call depends also on var. but function()Name is not possible
const auto var_expr_def = (as<ast::exp>(lit('[') >> exp_expr >> lit(']')) | as<ast::Name>(lit('.') >> name_expr));
const auto var_assign_expr_def = lit('=') >> explist_expr;
//! varlist ::= var {‘,’ var}
const auto varlist_expr_def = *(lit(',') >> (name_expr | var_expr)) >> var_assign_expr;
//! empty = call, ',' = var assign list, '=' single assign
const auto assignment_or_call_expr_def = primaryexp_expr >> -(var_assign_expr | varlist_expr);

//! retstat ::= return [explist] [‘;’]
//! local is needed here since a var could be declared as local myvar = nil;
const auto retstat_expr_def = lit("return") >> -explist_expr >> -lit(';');

//! field defs:
//! field ::= ‘[’ exp ‘]’ ‘=’ exp | Name ‘=’ exp | exp
const auto field_expr_def =
    (lit('[') >> exp_expr >> lit(']') >> lit('=') >> exp_expr) | (name_expr >> lit('=') >> exp_expr) | exp_expr;
//! fieldsep ::= ‘,’ | ‘;’
const auto fieldsep_expr = lit(',') | lit(';');
//! fieldlist ::= field {fieldsep field} [fieldsep]
const auto fieldlist_expr = field_expr >> *as<ast::field>(fieldsep_expr >> field_expr) >> -fieldsep_expr;
//! tableconstructor ::= ‘{’ [fieldlist] ‘}’
const auto tableconstructor_expr_def = lit('{') >> -fieldlist_expr >> lit('}');

//! for namelist in explist do block end
const auto for_namelist_expr_def = lit("for") >> namelist_expr >> lit("in") >> explist_expr >> lit("do") >> block_expr
                                   >> lit("end");

//! exp ::=  nil | false | true | Numeral | LiteralString | ‘...’ | functiondef | prefixexp | tableconstructor | exp
//! binop exp | unop exp exp binop exp is left recursive so we need to transform it. exp = (exp^binary_expr) exp' exp' =
//! binop exp exp' | empty
const auto exp_sec_expr_def = -(binary_expr);
const auto exp_expr_def = (bool_ | numeral_expr | nil_expr | functiondef_expr | primaryexp_expr |
                           tableconstructor_expr | unary_expr | literal_str_expr | lit("...")) >>
                          exp_sec_expr;

const auto binary_expr_def = (binary_op >> exp_expr);
const auto unary_expr_def = (unary_op >> exp_expr);

//! while exp do block end
const auto whiledo_expr_def = lit("while") >> exp_expr >> lit("do") >> block_expr >> lit("end");
//! repeat block until exp
const auto repeatuntil_expr_def = lit("repeat") >> block_expr >> lit("until") >> exp_expr;
//! do block end
const auto doblock_expr_def = lit("do") >> block_expr >> lit("end");
const auto forexp_exp_expr = lit(',') >> exp_expr;
//! for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end
const auto forexp_expr_def = lit("for") >> name_expr >> lit('=') >> exp_expr >> forexp_exp_expr >> -forexp_exp_expr >
                             lit("do") >> block_expr >> lit("end");

//! attrib ::= [‘<’ Name ‘>’]
const auto attrib_expr = -(lit('<') >> name_expr >> lit('>'));
//! attnamelist ::=  Name attrib {‘,’ Name attrib}
const auto attname_pair_expr = as<ast::name_attrib_pair>(name_expr >> -attrib_expr);
const auto attnamelist_expr_def = attname_pair_expr >> *(lit(',') >> attname_pair_expr);
//! local attnamelist [‘=’ explist]
const auto local_attnamelist_assign_expr_def = lit("local") >> attnamelist_expr >> -(lit('=') >> explist_expr);

//! goto Name
const auto goto_expr = as<ast::goto_stmt>(lit("goto") >> name_expr);
//! break
const auto break_expr = as<ast::keyword_stmt>(keyword_break);
const auto stat_expr_def = lit(';') | break_expr | goto_expr | assignment_or_call_expr | label_expr | doblock_expr |
                           whiledo_expr | repeatuntil_expr | ifelse_expr | forexp_expr | for_namelist_expr |
                           function_expr | local_function_expr | local_attnamelist_assign_expr;

//! {stat} [retstat]
const auto block_expr_def = *(stat_expr) >> -retstat_expr;
//! block
auto const chunk_expr_def = block_expr;

BOOST_SPIRIT_DEFINE(label_expr,
                    whiledo_expr,
                    repeatuntil_expr,
                    doblock_expr,
                    forexp_expr,
                    ifelse_expr,
                    attnamelist_expr,
                    namelist_expr,
                    parlist_expr,
                    args_expr,
                    primaryexp_expr,
                    prefixexp_expr,
                    prefixexp_sec_expr,
                    functioncall_expr,
                    var_expr,
                    var_assign_expr,
                    assignment_or_call_expr,
                    varlist_expr,
                    funcname_expr,
                    function_expr,
                    local_function_expr,
                    functiondef_expr,
                    for_namelist_expr,
                    field_expr,
                    tableconstructor_expr,
                    retstat_expr,
                    exp_expr,
                    local_attnamelist_assign_expr,
                    exp_sec_expr,
                    binary_expr,
                    unary_expr,
                    stat_expr,
                    block_expr,
                    chunk_expr)

const auto comment_skipper = "--" >> *(char_ - eol) >> (eol | eoi);
const auto comment_skipper_multiline = "--[[" >> *(char_ - "]]") >> ("]]" | eoi);
const auto input_skipper = x3::ascii::space | comment_skipper_multiline | comment_skipper;
} // namespace impl

} // namespace sre::lua::parser
