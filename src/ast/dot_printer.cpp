#pragma once
#include "dot_printer.hpp"

#include <fstream>
#include <memory>
#include <type_traits>
#include "printer.hpp"
namespace sre::lua::ast
{

#define IS(var, id)                                                                                                    \
    using val_type = std::remove_cv<std::remove_reference<decltype(var)>::type>::type;                                 \
    auto start_it = clones_.begin();                                                                                   \
    is<val_type, const val_type *>(var, start_it, clones_.end(), id);

DotPrinter::DotPrinter(std::ostream &out, const Clones &clones, const std::multimap<int, Sequence> &sequences)
    : out_{out}
    , clones_{clones}
    , sequences_{sequences}
    , counter{0}
{
    out_ << "digraph luaclones {" << std::endl;
    out_ << "node [fontsize=10, shape=box, height=0.25]" << std::endl;
    out_ << "edge [fontsize=10];" << std::endl;
}

void DotPrinter::printClones()
{
    std::sort(clones_processed_.begin(), clones_processed_.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });
    out_ << "subgraph clones {" << std::endl;
    while (clones_processed_.size() > 0)
    {
        auto f = clones_processed_.back();
        clones_processed_.pop_back();
        // eigentlich muesste hier ein reverse_iterator verwendet werden. Aber aus irgendeinem Grund wird der x3::[]
        // operator des lamdas verwendet. Das fuehrt dazu, dass es einen compile error gibt. Daher wird jetzt immer der
        // worst case ausgefuehrt, da dies aber nur zum debuggen ist, aktzeptiere ich es erstmal.
        auto f_it = std::find_if(
            clones_processed_.begin(), clones_processed_.end(), [f](const auto &a) { return f.first == a.first; });
        if (f_it != clones_processed_.end())
        {

            out_ << "\t"
                 << "\"" << f.second << "\" -> "
                 << "\"" << f_it->second << "\" [style=dashed, color=red, dir=none]" << std::endl;
            clones_processed_.erase(f_it);
        }
    }
    out_ << "}" << std::endl;
}
void DotPrinter::printSeqClones()
{
    std::cout << " seq: seqclones_processed_: " << seqclones_processed_.size() << std::endl;
#if 0 
    // only debugging
    for (const auto &x : sequences_)
    {
        std::cout << x.first << std::endl;
        for (const auto &y : x.second)
        {
            std::visit(
                [](auto x) {
                    std::cout << reinterpret_cast<intptr_t>(x) << " => ";
                    rexpr_printer{0, false}(*x);
                },
                y.first);
        }
    }
#endif
    for (const auto &seq : sequences_)
    {
        out_ << "subgraph seqclones" << seq.first << " {" << std::endl;
        std::set<int> all_ids;
        for (const auto &c : seq.second)
        {
            bool found = false;
            for (const auto &clone : seqclones_processed_)
            {
                if (clone.first == c.first)
                {
                    found = true;
                    all_ids.emplace(clone.second);
                    break;
                }
            }
            // only for debugging
            if (true && !found)
            {
                std::cout << "not found: ";
                std::visit([](auto x) { rexpr_printer{0, true}(*x); }, c.first);
            }
        }
        if (all_ids.size() > 1)
        {
            for (auto id = std::next(all_ids.begin()); id != all_ids.end(); id++)
            {
                out_ << "\t"
                     << "\"" << *(std::prev(id)) << "\" -> "
                     << "\"" << *id << "\" [style=dashed, color=blue, dir=none]" << std::endl;
            }
        }
        out_ << "}" << std::endl;
    }
}

void DotPrinter::operator()(const chunk &ast)
{
    out_ << "subgraph main {" << std::endl;
    const auto self = getId();
    IS(ast, self)
    node(self, "chunk", Unit{&ast});
    (*this)(self, ast.block_);
    out_ << "}" << std::endl;
    printClones();
    printSeqClones();
}
void DotPrinter::operator()(const int parent, const ast::block &block)
{
    const auto self = getId();
    IS(block, self)
    node(self, "block", Unit{&block});
    edge(parent, self);
    for (const auto &stat : block.stat_)
        (*this)(self, stat);
    (*this)(self, block.retstat_);
}
void DotPrinter::operator()(const int parent, const ast::stat &stat)
{
    const auto self = getId();
    seqclones_processed_.emplace_back(Unit{&stat}, self);

    IS(stat, self)
    node(self, "stat", Unit{&stat});
    edge(parent, self);
    boost::apply_visitor([=](const auto &stat_any) { (*this)(self, stat_any); }, stat);
}
void DotPrinter::operator()(const int parent, const ast::exp &exp)
{
    boost::apply_visitor([=](const auto &any) { (*this)(parent, any); }, exp);
}
void DotPrinter::operator()(const int parent, const std::string &value)
{
    const auto self = getId();
    // IS(value, self)
    node(self, "string", Unit{});
    edge(parent, self);
}
void DotPrinter::operator()(const int parent, const double value)
{
    const auto self = getId();
    // IS(value, self)
    node(self, "double=" + std::to_string(value), Unit{});
    edge(parent, self);
}
void DotPrinter::operator()(const int parent, const unsigned value)
{
    const auto self = getId();
    // IS(value, self)
    node(self, "unsigned=" + std::to_string(value), Unit{});
    edge(parent, self);
}
void DotPrinter::operator()(const int parent, const bool value)
{
    const auto self = getId();
    // IS(value, self)
    node(self, "bool=" + std::to_string(value), Unit{});
    edge(parent, self);
}
void DotPrinter::operator()(const int parent, const unary &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "unary", Unit{&value});
    edge(parent, self);
    (*this)(self, value.operator_);
    (*this)(self, value.rhs_);
}
void DotPrinter::operator()(const int parent, const binary &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "binary", Unit{&value});
    edge(parent, self);
    (*this)(self, value.operator_);
    (*this)(self, value.rhs_);
}
void DotPrinter::operator()(const int parent, const expression &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "expression", Unit{&value});
    edge(parent, self);

    (*this)(self, value.first_);
    (*this)(self, value.rest_);
}
void DotPrinter::operator()(const int parent, const label &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "label", Unit{&value});
    edge(parent, self);
    (*this)(self, value.name_);
}
void DotPrinter::operator()(const int parent, const Name &value)
{
    const auto self = getId();
    // IS(value, self)
    node(self, "Name=" + value.name, Unit{});
    edge(parent, self);
}
void DotPrinter::operator()(const int parent, const funcname &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "funcname", Unit{&value});
    edge(parent, self);
    (*this)(self, value.names_);
    (*this)(self, value.self_chain_);
}
void DotPrinter::operator()(const int parent, const function &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "function", Unit{&value});
    edge(parent, self);
    (*this)(self, value.funcname_);
    (*this)(self, value.funcbody_);
}
void DotPrinter::operator()(const int parent, const local_function &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "local_function", Unit{&value});
    edge(parent, self);
    (*this)(self, value.funcname_);
    (*this)(self, value.funcbody_);
}
void DotPrinter::operator()(const int parent, const namelist &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "namelist", Unit{&value});
    edge(parent, self);
    (*this)(self, value.name_);
    for (const auto &name : value.chain_)
        (*this)(self, name);
}
void DotPrinter::operator()(const int parent, const funcbody &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "funcbody", Unit{&value});
    edge(parent, self);
    (*this)(self, value.parameters_);
    (*this)(self, value.block_);
}
void DotPrinter::operator()(const int parent, const functiondef &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "functiondef", Unit{&value});
    edge(parent, self);
    (*this)(self, value.funcbody_);
}
void DotPrinter::operator()(const int parent, const field &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "field", Unit{&value});
    edge(parent, self);
    (*this)(self, value.first);
    (*this)(self, value.second);
}
void DotPrinter::operator()(const int parent, const fieldlist &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "fieldlist", Unit{&value});
    edge(parent, self);
    for (const auto &field : value)
        (*this)(self, field);
}
void DotPrinter::operator()(const int parent, const tableconstructor &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "table", Unit{&value});
    edge(parent, self);
    (*this)(self, value.first_);
    (*this)(self, value.fields_);
}
void DotPrinter::operator()(const int parent, const for_namelist &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "for", Unit{&value});
    edge(parent, self);
    (*this)(self, value.name_list_);
    (*this)(self, value.exp_list_);
    (*this)(self, value.block_);
}
void DotPrinter::operator()(const int parent, const prefixexp &value)
{
    boost::apply_visitor([=](const auto &any) { (*this)(parent, any); }, value);
}
void DotPrinter::operator()(const int parent, const functioncall &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "functioncall", Unit{&value});
    edge(parent, self);
    (*this)(self, value.prefix_exp_);
    if (value.name_.has_value())
        (*this)(self, value.name_.value());
    (*this)(self, value.args_);
}
void DotPrinter::operator()(const int parent, const args &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "args", Unit{&value});
    edge(parent, self);
    boost::apply_visitor([=](const auto &any) { (*this)(self, any); }, value);
}
void DotPrinter::operator()(const int parent, const explist &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "explist", Unit{&value});
    edge(parent, self);
    for (const auto &exp : value)
        (*this)(self, exp);
}
void DotPrinter::operator()(const int parent, const var &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "var", Unit{&value});
    edge(parent, self);
    boost::apply_visitor([=](const auto &any) { (*this)(self, any); }, value);
}
void DotPrinter::operator()(const int parent, const var_wrapper &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "var_wrapper", Unit{&value});
    edge(parent, self);
    (*this)(self, value.var_);
    (*this)(self, value.next_);
}
void DotPrinter::operator()(const int parent, const varlist &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "=", Unit{&value});
    edge(parent, self);
    for (const auto &var : value.rest_)
        (*this)(self, var);
    (*this)(self, value.explist_);
}
void DotPrinter::operator()(const int parent, const primaryexpression &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "primaryexpression", Unit{&value});
    edge(parent, self);
    (*this)(self, value.first_);
    (*this)(self, value.rest_);
}
void DotPrinter::operator()(const int parent, const assign_or_call &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "assign_or_call", Unit{&value});
    edge(parent, self);
    (*this)(self, value.primaryexp_);
    boost::apply_visitor([=](const auto &any) { (*this)(self, any); }, value.var_action_);
}
void DotPrinter::operator()(const int parent, const ifelse &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "if", Unit{&value});
    edge(parent, self);
    (*this)(self, value.first_);
    for (const auto &ifr : value.rest_)
        (*this)(self, ifr);
    if (value.else_.has_value())
        (*this)(self, value.else_.value());
}
void DotPrinter::operator()(const int parent, const whiledo &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "while", Unit{&value});
    edge(parent, self);
    (*this)(self, value.exp_);
    (*this)(self, value.block_);
}
void DotPrinter::operator()(const int parent, const repeatuntil &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "repeat", Unit{&value});
    edge(parent, self);
    (*this)(self, value.block_);
    (*this)(self, value.exp_);
}
void DotPrinter::operator()(const int parent, const doblock &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "do", Unit{&value});
    edge(parent, self);
    (*this)(self, value.block_);
}
void DotPrinter::operator()(const int parent, const forexp &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "forexp", Unit{&value});
    edge(parent, self);
    (*this)(self, value.name_);
    (*this)(self, value.exp_first_);
    (*this)(self, value.exp_second_);
    (*this)(self, value.exp_third_);
    (*this)(self, value.block_);
}
void DotPrinter::operator()(const int parent, const local_attnamelist_assign &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "local att =", Unit{&value});
    edge(parent, self);
    (*this)(self, value.attnamelist_);
    (*this)(self, value.explist_);
}
void DotPrinter::operator()(const int parent, const attnamelist &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "attnamelist", Unit{&value});
    edge(parent, self);
    (*this)(self, value.first_);
    for (const auto &r : value.rest_)
        (*this)(self, r);
}
void DotPrinter::operator()(const int parent, const name_attrib_pair &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "name_attrib", Unit{&value});
    edge(parent, self);
    (*this)(self, value.name_);
    if (value.attrib_.has_value())
        (*this)(self, value.attrib_.value());
}
void DotPrinter::operator()(const int parent, const ifelse_wrapper &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "ifelse", Unit{&value});
    edge(parent, self);
    (*this)(self, value.exp_);
    (*this)(self, value.block_);
}

void DotPrinter::operator()(const int parent, const goto_stmt &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, "goto", Unit{&value});
    edge(parent, self);
    (*this)(self, value.name_);
}
void DotPrinter::operator()(const int parent, const keyword_stmt &value)
{
    const auto self = getId();
    IS(value, self)
    node(self, std::to_string(value.keyword_), Unit{&value});
    edge(parent, self);
}
void DotPrinter::operator()(const int parent, const numeral &value)
{
    boost::apply_visitor([=](const auto &any) { (*this)(parent, any); }, value);
}
void DotPrinter::operator()(const int parent, const nil &nil)
{}

void DotPrinter::operator()(const int parent, const optoken &op)
{
    if (auto it = kOptokenSymbols.find(op); it != kOptokenSymbols.end())
    {
        const auto self = getId();
        // IS(it->second, self)
        node(self, it->second, Unit{});
        edge(parent, self);
    }
}
void DotPrinter::node(const int parent, const std::string &label, const Unit &u)
{
    out_ << "\t"
         << "\"" << parent << "\" [label=\"" << label << "\"]" << std::endl;

    seqclones_processed_.emplace_back(u, parent);
}
void DotPrinter::edge(const int parent, const int child)
{
    out_ << "\t"
         << "\"" << parent << "\" -> "
         << "\"" << child << "\"" << std::endl;
}
int DotPrinter::getId()
{
    return counter++;
}
DotPrinter::~DotPrinter()
{
    out_ << "}" << std::endl;
}

} // namespace sre::lua::ast
