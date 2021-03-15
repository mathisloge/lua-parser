#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <thread>
#include "ast/dot_printer.hpp"
#include "ast/printer.hpp"
#include "clone.hpp"
#include "hasher.hpp"
#include "parser.hpp"
namespace fs = std::filesystem;

int parsefile(const std::string &file);
int main()
{
#if 0
    std::vector<std::thread> threads_;
    for (auto &p : fs::recursive_directory_iterator("testdir"))
    {
        if (p.path().has_extension() && p.path().extension() == ".lua")
        {
            std::cout << p.path() << '\n';

            try
            {
                parsefile(p.path().string());
            }
            catch (std::exception e)
            {
                std::cout << "got ex while parsing : " << e.what() << std::endl;
            }
        }
    }
#else
    parsefile("test.lua");
#endif
}
int parsefile(const std::string &file)
{
    sre::lua::ast::chunk chunk;

    std::ifstream t(file);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    auto in = str.begin();
    auto end = str.end();

    const bool ret = sre::lua::Parser{}.parse(in, end, chunk);

    if (ret && in == end)
    {
        std::cout << "parsing succeeded" << std::endl;
        std::ofstream ofs{fs::path(file).replace_extension(".dot")};

        sre::lua::ast::DotPrinter printer{ofs};
        sre::lua::ast::Hasher hasher{};
        printer(chunk);

        sre::lua::ast::Clone cloner;
        cloner.run(chunk);
    }
    else
    {
        // const std::string context(in, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed for file " << file << std::endl;
        // std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        return 1;
    }
    return 0;
}
