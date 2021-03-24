#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <thread>
#include "ast/dot_printer.hpp"
#include "ast/printer.hpp"
#include "clone.hpp"
#include "cxxopts.hpp"
#include "hasher.hpp"
#include "parser.hpp"
namespace fs = std::filesystem;

int parsefile(const std::string &file);
int main(int argc, char **argv)
{
    cxxopts::Options options(argv[0], "Lua Typ-1 und Typ-2 Klonerkennung nach Baxter");
    // clang-format off
    options.add_options()
        ("p,path", "Datei oder Ordner die zu puefen sind", cxxopts::value<std::string>()->default_value("testdir"))
        ("e,extension", "Falls die Extension der Lua-Datein innerhalb eines Ordners anders sein sollten", cxxopts::value<std::string>()->default_value(".lua"))
        ("h,help", "Print help");
    // clang-format on
    auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    const std::filesystem::path parse_path{result["path"].as<std::string>()};
    if (!std::filesystem::exists(parse_path))
    {
        std::cout << "Der angegebene Pfad existiert nicht. Absolut: " << std::filesystem::absolute(parse_path)
                  << std::endl;
        std::cout << options.help() << std::endl;
        exit(-1);
    }
    if (std::filesystem::is_regular_file(parse_path))
    {
        parsefile(parse_path.string());
    }
    else
    {
        const std::string extension{result["extension"].as<std::string>()};
        std::cout << "using extension: " << extension << std::endl;
        std::vector<std::thread> threads_;
        for (auto &p : fs::recursive_directory_iterator(parse_path))
        {
            auto run_parse = [&extension, p]() {
                if (p.path().has_extension() && p.path().extension() == extension)
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
            };
            run_parse();
        }
        for (auto &t : threads_)
        {
            if (t.joinable())
                t.join();
        }
    }
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

        sre::lua::ast::Clone cloner;
        cloner.run(chunk);
        sre::lua::ast::DotPrinter printer{ofs, cloner.clones()};
        printer(chunk);
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
