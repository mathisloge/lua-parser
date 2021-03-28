#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <streambuf>
#include <string>
#include <thread>
#include "ast/dot_printer.hpp"
#include "ast/printer.hpp"
#include "clone.hpp"
#include "cxxopts.hpp"
#include "hasher.hpp"
#include "metrics.hpp"
#include "parser.hpp"
#include "runtime_config.hpp"
namespace fs = std::filesystem;

int parsefile(const std::string &file, sre::lua::ast::Metrics &metrics);
int main(int argc, char **argv)
{
    cxxopts::Options options(argv[0], "Lua Typ-1 und Typ-2 Klonerkennung nach Baxter");
    // clang-format off
    options.add_options()
        ("p,path", "Datei oder Ordner die zu puefen sind", cxxopts::value<std::string>()->default_value("testdir"))
        ("e,extension", "Falls die Extension der Lua-Datein innerhalb eines Ordners anders sein sollten", cxxopts::value<std::string>()->default_value(".lua"))
        ("t,type", "Typ 1 oder 2", cxxopts::value<int>()->default_value("2"))
        ("n,name", "Project name", cxxopts::value<std::string>())
        ("d,dot", "Generate dot files", cxxopts::value<bool>()->default_value("true"))
        ("h,help", "Print help");
    // clang-format on
    auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    if (result.count("name") <= 0)
    {
        std::cout << "Name is missing" << std::endl;
        std::cout << options.help() << std::endl;
        exit(1);
    }

    if (result["type"].as<int>() == 1)
    {
        RuntimeConfig::clone_type = RuntimeConfig::CloneType::type1;
        std::cout << "Detecting type 1 clones" << std::endl;
    }
    else
    {
        RuntimeConfig::clone_type = RuntimeConfig::CloneType::type2;
        std::cout << "Detecting type 2 clones" << std::endl;
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
        sre::lua::ast::Metrics metrics{"debug", "debug.csv"};
        parsefile(parse_path.string(), metrics);
    }
    else
    {
        const auto projname = result["name"].as<std::string>();
        sre::lua::ast::Metrics metrics{projname, projname + ".csv"};
        const std::string extension{result["extension"].as<std::string>()};
        std::cout << "using extension: " << extension << std::endl;
        std::vector<std::thread> threads_;
        for (auto &p : fs::recursive_directory_iterator(parse_path))
        {
            auto run_parse = [&metrics, &extension, p]() {
                if (p.path().has_extension() && p.path().extension() == extension)
                {
                    std::cout << p.path() << '\n';

                    try
                    {
                        parsefile(p.path().string(), metrics);
                    }
                    catch (std::exception e)
                    {
                        std::cout << "got ex while parsing : " << e.what() << std::endl;
                    }
                }
            };
            threads_.emplace_back(run_parse);
            // run_parse();
        }
        for (auto &t : threads_)
        {
            if (t.joinable())
                t.join();
        }
        metrics.saveMetrics();
    }
}
int parsefile(const std::string &file, sre::lua::ast::Metrics &metrics)
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
        sre::lua::ast::DotPrinter printer{ofs, cloner.clones(), cloner.sequence_clones()};
        printer(chunk);

        std::size_t loc = 0;
        for (const auto c : str)
        {
            if (c == '\n')
                loc++;
        }
        metrics.addChunkResult(file, std::move(chunk), loc, cloner.clones(), cloner.sequence_clones());
    }
    else
    {
        // const std::string context(in, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed for file " << file << std::endl;
        // std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        metrics.parsedFailed(file);
        return 1;
    }
    return 0;
}
