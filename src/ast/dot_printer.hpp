#pragma once
#include "ast.hpp"
#include <map>
#include <iostream>
namespace sre::lua::ast
{
    struct dot_printer
    {
        int id;
        dot_printer(int id) : id{id} {}

        void operator()(const chunk &ast) const
        {
            std::cout << "digraph {" << std::endl;
            std::cout << "chunk -> ";
            dot_printer{id + 1}(ast.block_);
            std::cout << '}' << std::endl;
        }

        void operator()(ast::block const &block) const
        {
            std::cout << "Block_" << id << ";" << std::endl;

            for (auto const &stat : block.stat_)
            {
                std::cout << "Block_" << id << " -> ";
                dot_printer{id + 1}(stat);
            }

            for (auto const &retstat : block.retstat_)
            {
                std::cout << "Block_" << id << " -> ";
                dot_printer(id + 1)(retstat);
            }
        }

        void operator()(ast::stat const &stat) const
        {
            std::cout << "stat_" << id << ";" << std::endl;
        }

        void operator()(ast::exp const &exp) const
        {
            std::cout << "exp_" << id << ";" << std::endl;
            std::cout << "exp_" << id << " -> ";
            boost::apply_visitor(dot_printer(id + 1), exp);
        }

        void operator()(std::string const &text) const
        {
            std::cout << "string_" << id << ";" << std::endl;
        }

        void operator()(double const &text) const
        {
            std::cout << "double_" << id << ";" << std::endl;
        }

        void operator()(bool const &text) const
        {
            std::cout << "bool_" << id << ";" << std::endl;
        }

        void operator()(const unary &unary) const
        {
            const auto symbol = kOptokenSymbols.find(unary.operator_);
            std::cout << "\"unary_" << symbol->second << "_" << id << "\";" << std::endl;
            std::cout << "\"unary_" << symbol->second << "_" << id << "\" -> ";
            boost::apply_visitor(dot_printer(id + 1), unary.rhs_);
        }

        void operator()(const binary &bin) const
        {
            const auto symbol = kOptokenSymbols.find(bin.operator_);
            std::cout << "\"binary_" << symbol->second << "_" << id << "\";" << std::endl;
            std::cout << "\"binary_" << symbol->second << "_" << id << "\" -> ";
            boost::apply_visitor(dot_printer(id + 1), bin.rhs_);
        }

        void operator()(const expression &expression) const
        {
            std::cout << "\"expression_" << id << "\";" << std::endl;
            std::cout << "\"expression_" << id << "\" -> ";
            dot_printer{id + 1}(expression.first_);
            std::cout << "\"expression_" << id << "\" -> ";
            dot_printer{id + 1}(expression.rest_);
        }

        void operator()(const nil &nil) const
        {
            std::cout << "\"nil_" << id << "\";" << std::endl;
        }
    };
}
