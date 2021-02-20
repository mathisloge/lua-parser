#pragma once
#include <iostream>
#include <iomanip>
#include <boost/spirit/home/x3.hpp>

#include "ast/ast.hpp"
#include "ast/ast_adapted.hpp"
#include "chunk.hpp"
namespace sre::lua::parser
{
    namespace impl
    {
        struct ehbase
        {
            template <typename It, typename Ctx>
            x3::error_handler_result on_error(It f, It l, x3::expectation_failure<It> const &e, Ctx const & /*ctx*/) const
            {
                std::cout << std::string(f, l) << "\n"
                          << std::setw(1 + std::distance(f, e.where())) << "^"
                          << "-- expected: " << e.which() << "\n";
                return x3::error_handler_result::fail;
            }
        };

        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;
        using x3::alnum;
        using x3::alpha;
        using x3::bool_;
        using x3::char_;
        using x3::double_;
        using x3::int_;
        using x3::lexeme;
        using x3::lit;
        using x3::raw;

        inline x3::symbols<ast::optoken> binary_op;
        inline x3::symbols<ast::optoken> unary_op;
        inline x3::symbols<> keywords;

        //struct nil_expr_class;
        //using nil_expr_type = x3::rule<nil_expr_class, ast::nil>;
        //const nil_expr_type nil_expr = "nil_expr";

        struct literal_string_expr_class;
        inline const x3::rule<literal_string_expr_class, std::string> literal_string_expr{"literal_string_expr"};

        struct exp_expr_class : ehbase{};
        inline const x3::rule<exp_expr_class, ast::expression> exp_expr{"exp_expr"};
        struct exp_sec_expr_class : ehbase{};
        inline const x3::rule<exp_sec_expr_class, ast::exp> exp_sec_expr{"exp_sec_expr"};

        struct binary_expr_class;
        inline const x3::rule<binary_expr_class, ast::binary> binary_expr{"binary_expr"};

        struct unary_expr_class;
        inline const x3::rule<unary_expr_class, ast::unary> unary_expr{"unary_expr"};

        struct retstat_expr_class;
        inline const x3::rule<retstat_expr_class, ast::explist> retstat_expr{"retstat_expr"};

        struct stat_expr_class;
        inline const x3::rule<stat_expr_class, ast::stat> stat_expr{"stat_expr"};

        struct block_expr_class;
        inline const x3::rule<block_expr_class, ast::block> block_expr{"block_expr"};

        inline const chunk_type chunk_expr{"chunk_expr"};

        void initBinOp();
        void initUnaryOp();
        void initKeywords();

        //inline const auto nil_expr_def = lit("nil");
        template <typename T>
        static auto as = [](auto p) { return x3::rule<struct tag, T>{"as"} = p; };

        inline const auto distinct_keyword = lexeme[keywords >> !(x3::alnum | '_')];
        inline const auto literal_string_expr_def = raw[lexeme[(alpha | '_') >> *(alnum | '_')]] - distinct_keyword;
        //! exp {‘,’ exp}
        inline const auto explist_expr = exp_expr >> *(lit(',') > exp_expr);

        inline const auto retstat_expr_def = lit("return") > explist_expr > -lit(';');
        inline const auto attrib_expr = -(lit('<') > literal_string_expr_def > lit('>'));

        //! nil | false | true | Numeral | LiteralString | ‘...’ | functiondef | prefixexp | tableconstructor | exp binop exp | unop exp
        //! exp binop exp is left recursive so we need to transform it. 
        //! exp = (exp^binary_expr) exp'
        //! exp' = binop exp exp' | empty
        inline const auto exp_sec_expr_def = -(binary_expr);
        inline const auto exp_expr_def = (bool_ | double_ | literal_string_expr | unary_expr) > exp_sec_expr;
       

        inline const auto binary_expr_def = (binary_op > exp_expr);
        inline const auto unary_expr_def = (unary_op > exp_expr);

        inline const auto stat_expr_def = literal_string_expr_def;

        //! {stat} [retstat]
        inline const auto block_expr_def = *(stat_expr) >> retstat_expr;
        //! block
        inline auto const chunk_expr_def = block_expr;

        BOOST_SPIRIT_DEFINE(
            //nil_expr,
            retstat_expr,
            literal_string_expr,
            exp_expr,
            exp_sec_expr,
            binary_expr,
            unary_expr,
            stat_expr,
            block_expr,
            chunk_expr)
    }

}
