#include "ast/ast.hpp"
#include "ast/ast_adapted.hpp"
#include "ast/printer.hpp"
#include "chunk.hpp"
#include <iostream>
//#include "ast/dot_printer.hpp"
#include "chunk_def.hpp"
//http://lua-users.org/lists/lua-l/2011-10/msg00440.html
int main()
{
    sre::lua::ast::chunk chunk_out;
    const std::string str = R"(
        -- defines a factorial function
        ::test1::
        { 1+1 }
        function test2(in_test1, in_test3)
            local function test6(intest3)
            end
            function test3()
                ::test4::
            end
        return;
        end
    )";
    const std::string str2 = R"(
            -- defines a factorial function
    function fact (n)
      return n * fact(n+1+3)(10+10) + 5
    end
    fact(5+5)
    test1, test2 = 10,20
    )";
    auto in = str2.begin();
    auto end = str2.end();

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
