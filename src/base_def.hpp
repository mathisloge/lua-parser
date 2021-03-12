#pragma once
#include <iomanip>
#include <iostream>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/traits/transform_attribute.hpp>

#include "ast/ast.hpp"
#include "ast/ast_adapted.hpp"

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
using x3::double_;
using x3::eoi;
using x3::eol;
using x3::hex;
using x3::int_;
using x3::lexeme;
using x3::lit;
using x3::omit;
using x3::raw;

inline x3::symbols<> keywords;
inline x3::symbols<ast::keyword> keyword_break;
inline x3::symbols<ast::keyword> keyword_nil;
inline x3::symbols<ast::optoken> binary_op;
inline x3::symbols<ast::optoken> unary_op;

template <typename T>
auto as = [](auto p) { return x3::rule<struct tag, T>{"as"} = p; };

inline constexpr x3::rule<struct literal_str_expr_class, std::string> literal_str_expr{"literal_str_expr"};
inline constexpr x3::rule<struct name_expr_class, ast::Name> name_expr{"name_expr"};
inline constexpr x3::rule<struct nil_expr_class, ast::keyword_stmt> nil_expr{"nil_expr"};
inline constexpr x3::rule<struct numeral_expr_class, ast::numeral> numeral_expr{"numeral_expr"};

inline const auto distinct_keyword = lexeme[keywords >> !(x3::alnum | '_')];
inline const auto unchecked_name = lexeme[(x3::alpha | x3::char_('_')) >> *(x3::alnum | x3::char_('_'))];

constexpr auto genDelimStr = [](char delim) {
    return x3::lexeme[omit[delim] > *(char_("\\\\") | '\\' >> -char_(delim) | (char_ - delim)) > omit[delim]];
};


inline const auto literal_str_expr_def = genDelimStr('"') | genDelimStr('\'');
// a name can't be any keyword, so exclude it.
inline const auto name_expr_def = raw[unchecked_name - distinct_keyword];

inline const auto nil_expr_def = keyword_nil;

inline const auto numeral_expr_def = (lit("0x") >> hex) | double_;

BOOST_SPIRIT_DEFINE(name_expr, literal_str_expr, nil_expr, numeral_expr)

void initKeywords();
void initBinOp();
void initUnaryOp();

} // namespace impl
} // namespace sre::lua::parser
