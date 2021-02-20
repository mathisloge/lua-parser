#include "ast/ast.hpp"
#include "ast/ast_adapted.hpp"
#include "chunk.hpp"
#include <iostream>
#include "ast/printer.hpp"

int main()
{
    sre::lua::ast::chunk chunk_out;
    const std::string str = "return hallo;";

    auto in = str.begin();
    auto end = str.end();
    const bool ret = boost::spirit::x3::phrase_parse(in, end, sre::lua::parser::chunk(), boost::spirit::x3::ascii::space, chunk_out);

    if (ret && in == end)
    {
        std::cout << "parsing succeeded" << std::endl;
        sre::lua::ast::rexpr_printer printer;
        printer(chunk_out);
    }
    else
    {
        std::string context(in, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        return 1;
    }
    //std::cout << b << ": " << chunk_out << std::endl;
}
