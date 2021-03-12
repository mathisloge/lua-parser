#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include "ast/dot_printer.hpp"
#include "ast/printer.hpp"
#include "parser.hpp"

int main()
{
    sre::lua::ast::chunk chunk;

    std::ifstream t("test.lua");
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    auto in = str.begin();
    auto end = str.end();

    const bool ret = sre::lua::Parser{}.parse(in, end, chunk);

    if (ret && in == end)
    {
        std::cout << "parsing succeeded" << std::endl;
        std::ofstream ofs{"test.dot"};

        sre::lua::ast::DotPrinter printer{ofs};
        printer(chunk);
    }
    else
    {
        const std::string context(in, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        return 1;
    }
}
