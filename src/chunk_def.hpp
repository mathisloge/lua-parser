#pragma once
#include <boost/spirit/home/x3.hpp>

#include "ast/ast.hpp"
#include "ast/ast_adapted.hpp"
#include "chunk.hpp"
namespace sre::lua::parser
{
    namespace impl
    {
        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        using ascii::char_;
        using x3::double_;
        using x3::int_;
        using x3::lexeme;
        using x3::lit;

        x3::symbols<ast::optoken> bin_op;
        x3::symbols<ast::optoken> unary_op;
        x3::symbols<> keywords;
        void initBinOp();
        void initUnaryOp();
        void initKeywords();

        const chunk_type chunk_expr = "chunk_expr";
        auto const quoted_string = lexeme['"' >> +(char_ - '"') >> '"'];
        auto const chunk_expr_def = lit("employee") >> quoted_string;

        BOOST_SPIRIT_DEFINE(chunk_expr)
    }
    impl::chunk_type chunk()
    {
        return impl::chunk_expr;
    }
}
