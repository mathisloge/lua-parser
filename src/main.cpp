#include "ast/ast.hpp"
#include "ast/ast_adapted.hpp"
#include "chunk.hpp"
#include <iostream>

template <typename Parser>
bool test(std::string const &str, Parser p, sre::lua::ast::chunk &output, bool full_match = true)
{
    auto in = str.begin();
    auto end = str.end();
    bool ret = boost::spirit::x3::phrase_parse(in, end, p, boost::spirit::x3::ascii::space, output);
    ret &= (!full_match || (in == end));
    return ret;
}

int main()
{
    sre::lua::ast::chunk chunk_out;
    auto b = test("employee\"fobar\"", sre::lua::parser::chunk(), chunk_out);
    std::cout << b << ": " << chunk_out << std::endl;
}
