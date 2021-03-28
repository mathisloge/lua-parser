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
    DotPrinter(std::ostream &out, const Clones &clones, const SequenceClones &sequences);
    void operator()(const chunk &ast);
    ~DotPrinter();

  private:
    void operator()(const int parent, const ast::block &block);
    void operator()(const int parent, const ast::stat &stat);
    void operator()(const int parent, const ast::exp &exp);
    void operator()(const int parent, const std::string &value);
    void operator()(const int parent, const double value);
    void operator()(const int parent, const unsigned value);
    void operator()(const int parent, const bool value);
    void operator()(const int parent, const unary &value);
    void operator()(const int parent, const binary &value);
    void operator()(const int parent, const expression &value);
    void operator()(const int parent, const label &value);
    void operator()(const int parent, const Name &value);
    void operator()(const int parent, const funcname &value);
    void operator()(const int parent, const function &value);
    void operator()(const int parent, const local_function &value);
    void operator()(const int parent, const namelist &value);
    void operator()(const int parent, const funcbody &value);
    void operator()(const int parent, const functiondef &value);
    void operator()(const int parent, const field &value);
    void operator()(const int parent, const fieldlist &value);
    void operator()(const int parent, const tableconstructor &value);
    void operator()(const int parent, const for_namelist &value);
    void operator()(const int parent, const prefixexp &value);
    void operator()(const int parent, const functioncall &value);
    void operator()(const int parent, const args &value);
    void operator()(const int parent, const explist &value);
    void operator()(const int parent, const var &value);
    void operator()(const int parent, const var_wrapper &value);
    void operator()(const int parent, const varlist &value);
    void operator()(const int parent, const primaryexpression &value);
    void operator()(const int parent, const assign_or_call &value);
    void operator()(const int parent, const ifelse &value);
    void operator()(const int parent, const whiledo &value);
    void operator()(const int parent, const repeatuntil &value);
    void operator()(const int parent, const doblock &value);
    void operator()(const int parent, const forexp &value);
    void operator()(const int parent, const local_attnamelist_assign &value);
    void operator()(const int parent, const attnamelist &value);
    void operator()(const int parent, const name_attrib_pair &value);
    void operator()(const int parent, const ifelse_wrapper &value);
    void operator()(const int parent, const goto_stmt &value);
    void operator()(const int parent, const keyword_stmt &value);
    void operator()(const int parent, const numeral &value);
    void operator()(const int parent, const nil &nil);
    void operator()(const int parent, const optoken &nil);

    void node(const int parent, const std::string &label, const Unit& u);
    void edge(const int parent, const int child);
    int getId();

    void printClones();
    void printSeqClones();

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
    std::vector<std::pair<Unit, int>> seqclones_processed_;
    const std::multimap<int, Sequence> &sequences_;
    int counter;
};

} // namespace sre::lua::ast
