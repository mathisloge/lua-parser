#pragma once
#include "ast.hpp"
#include <map>
#include <iostream>
namespace sre::lua::ast
{

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

    struct rexpr_printer
    {
        static constexpr int const tabsize = 2;
        typedef void result_type;

        rexpr_printer(int indent = 0)
            : indent(indent) {}

        void operator()(const chunk &ast) const
        {
            std::cout << '{' << std::endl;
            tab(indent);
            std::cout << "exp chunk = " << std::endl;
            rexpr_printer{indent + tabsize}(ast.block_);
            std::cout << '}' << std::endl;
        }

        void operator()(ast::block const &block) const
        {
            tab(indent);
            std::cout << "block stat = " << std::endl;
            for (auto const &stat : block.stat_)
            {
                rexpr_printer{indent + tabsize}(stat);
            }
            tab(indent);
            std::cout << "block retstat = " << std::endl;
            for (auto const &retstat : block.retstat_)
            {
                rexpr_printer(indent + tabsize)(retstat);
            }
        }

        void operator()(ast::stat const &stat) const
        {
            tab(indent);
            std::cout << "stat = " << std::endl;
        }

        void operator()(ast::exp const &exp) const
        {
            tab(indent);
            std::cout << "exp" << std::endl;
            boost::apply_visitor(rexpr_printer(indent + tabsize), exp);
        }

        void operator()(std::string const &text) const
        {
            tab(indent);
            std::cout << "string \"" << text << '"' << std::endl;
        }

        void operator()(double const &text) const
        {
            tab(indent);
            std::cout << "double \"" << text << '"' << std::endl;
        }

        void operator()(bool const &text) const
        {
            tab(indent);
            std::cout << "bool \"" << text << '"' << std::endl;
        }

        void operator()(const unary &unary) const
        {
            const auto symbol = kOptokenSymbols.find(unary.operator_);
            tab(indent);
            std::cout << "UNARY " << symbol->second << std::endl;
            boost::apply_visitor(rexpr_printer(indent + tabsize), unary.rhs_);
        }

        void operator()(const binary &bin) const
        {
            const auto symbol = kOptokenSymbols.find(bin.operator_);
            tab(indent);
            std::cout << "binary " << symbol->second << std::endl;
            boost::apply_visitor(rexpr_printer(indent + tabsize), bin.rhs_);
        }

        void operator()(const expression &expression) const
        {
            tab(indent);
            std::cout << "expression " << std::endl;
            rexpr_printer{indent + tabsize}(expression.first_);
            rexpr_printer{indent + tabsize}(expression.rest_);
        }

        void operator()(const nil &nil) const
        {
            tab(indent);
            std::cout << "nil " << std::endl;
        }

        void tab(int spaces) const
        {
            for (int i = 0; i < spaces; ++i)
                std::cout << ' ';
        }

        int indent;
    };
}
