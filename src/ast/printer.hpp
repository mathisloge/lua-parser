#pragma once
#include "ast.hpp"
#include <map>
#include <iostream>
#include "ast_adapted.hpp"
namespace sre::lua::ast
{
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
            std::cout << std::endl
                      << '}' << std::endl;
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
            if (block.retstat_.size() > 0)
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
            boost::apply_visitor(rexpr_printer(indent + tabsize), stat);
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

        void operator()(const label &value) const
        {
            tab(indent);
            std::cout << "label=" << value << std::endl;
        }

        void operator()(const Name &value) const
        {
            tab(indent);
            std::cout << "name=" << value.name << std::endl;
        }

        void operator()(const funcname &value) const
        {
            tab(indent);
            std::cout << "funcname=" << std::endl;
            rexpr_printer{indent + tabsize}(value.name_);

            for (const auto &name : value.chain_)
            {
                tab(indent);
                std::cout << "chain=" << std::endl;
                rexpr_printer{indent + tabsize}(name);
            }

            if (!value.self_chain_.name.empty())
            {
                tab(indent);
                std::cout << "self_chain=" << std::endl;
                rexpr_printer{indent + tabsize}(value.self_chain_);
            }
        }

        void operator()(const function &value) const
        {
            tab(indent);
            std::cout << "function=" << std::endl;
            rexpr_printer{indent + tabsize}(value.funcname_);
            tab(indent + tabsize);
            std::cout << "params=" << std::endl;
            for (const auto &arg : value.parameters_)
            {
                rexpr_printer{indent + 2 * tabsize}(arg);
            }
            rexpr_printer{indent + tabsize}(value.block_);
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
