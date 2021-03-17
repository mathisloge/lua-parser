#pragma once
#include <iostream>
#include <map>
#include <ostream>
#include "../types.hpp"
#include "ast/ast.hpp"
namespace sre::lua::ast
{

class DotPrinter final
{
  public:
    DotPrinter(std::ostream &out, const Clones &clones);
    void operator()(const chunk &ast);
    ~DotPrinter();

  private:
    void operator()(const intptr_t parent, const ast::block &block);
    void operator()(const intptr_t parent, const ast::stat &stat);
    void operator()(const intptr_t parent, const ast::exp &exp);
    void operator()(const intptr_t parent, const std::string &value);
    void operator()(const intptr_t parent, const double value);
    void operator()(const intptr_t parent, const unsigned value);
    void operator()(const intptr_t parent, const bool value);
    void operator()(const intptr_t parent, const unary &value);
    void operator()(const intptr_t parent, const binary &value);
    void operator()(const intptr_t parent, const expression &value);
    void operator()(const intptr_t parent, const label &value);
    void operator()(const intptr_t parent, const Name &value);
    void operator()(const intptr_t parent, const funcname &value);
    void operator()(const intptr_t parent, const function &value);
    void operator()(const intptr_t parent, const local_function &value);
    void operator()(const intptr_t parent, const namelist &value);
    void operator()(const intptr_t parent, const funcbody &value);
    void operator()(const intptr_t parent, const functiondef &value);
    void operator()(const intptr_t parent, const field &value);
    void operator()(const intptr_t parent, const fieldlist &value);
    void operator()(const intptr_t parent, const tableconstructor &value);
    void operator()(const intptr_t parent, const for_namelist &value);
    void operator()(const intptr_t parent, const prefixexp &value);
    void operator()(const intptr_t parent, const functioncall &value);
    void operator()(const intptr_t parent, const args &value);
    void operator()(const intptr_t parent, const explist &value);
    void operator()(const intptr_t parent, const var &value);
    void operator()(const intptr_t parent, const var_wrapper &value);
    void operator()(const intptr_t parent, const varlist &value);
    void operator()(const intptr_t parent, const primaryexpression &value);
    void operator()(const intptr_t parent, const assign_or_call &value);
    void operator()(const intptr_t parent, const ifelse &value);
    void operator()(const intptr_t parent, const whiledo &value);
    void operator()(const intptr_t parent, const repeatuntil &value);
    void operator()(const intptr_t parent, const doblock &value);
    void operator()(const intptr_t parent, const forexp &value);
    void operator()(const intptr_t parent, const local_attnamelist_assign &value);
    void operator()(const intptr_t parent, const attnamelist &value);
    void operator()(const intptr_t parent, const name_attrib_pair &value);
    void operator()(const intptr_t parent, const ifelse_wrapper &value);
    void operator()(const intptr_t parent, const goto_stmt &value);
    void operator()(const intptr_t parent, const keyword_stmt &value);
    void operator()(const intptr_t parent, const numeral &value);
    void operator()(const intptr_t parent, const nil &nil);
    void operator()(const intptr_t parent, const optoken &nil);

    void node(const intptr_t parent, const std::string &label);
    void edge(const intptr_t parent, const intptr_t child);
    int getId();  

    void printClones();

  private:
    template <typename T, typename Tp>
    void is(const T &val, Ccit &start, Ccit end, int id)
    {

        for (; start != end; start++)
        {
            // hier reicht ein simpler zeiger vergleich, da die Daten in diesen Schritten nicht kopiert werden.
            auto p1 = std::get_if<Tp>(&start->first);
            auto p2 = std::get_if<Tp>(&start->second);
            if ((p1 && *p1 == &val) || (p2 && *p2 == &val))
            {
                clones_processed_.emplace_back(start, id);
            }
        }
    }

  private:
    std::ostream &out_;
    const Clones &clones_;
    std::vector<std::pair<Clones::const_iterator, int>> clones_processed_;
    int counter;
};

} // namespace sre::lua::ast
