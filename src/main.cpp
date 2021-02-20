#include "ast/ast.hpp"
#include "ast/ast_adapted.hpp"
#include "chunk.hpp"
#include <iostream>
#include "ast/printer.hpp"
#include "ast/dot_printer.hpp"
#include "chunk_def.hpp"

int main()
{
    sre::lua::ast::chunk chunk_out;
    const std::string str = R"(
        -- defines a factorial function
        ::test1::
        function test2(in_test1, in_test3)
            function test3()
                ::test4::
            end
        return;
        end
    )";

    auto in = str.begin();
    auto end = str.end();

    const bool ret = boost::spirit::x3::phrase_parse(in, end, sre::lua::parser::chunk(), sre::lua::parser::impl::input_skipper, chunk_out);

    if (ret && in == end)
    {
        std::cout << "parsing succeeded" << std::endl;
        sre::lua::ast::rexpr_printer printer;
        //sre::lua::ast::dot_printer printer(0);
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
