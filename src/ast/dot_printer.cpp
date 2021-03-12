#pragma once
#include "dot_printer.hpp"

#include <fstream>
#include <memory>
namespace sre::lua::ast
{

DotPrinter::DotPrinter(std::ostream &out)
    : out_{out}
{
    out_ << "digraph ninja {" << std::endl;
    out_ << "node [fontsize=10, shape=box, height=0.25]" << std::endl;
    out_ << "edge [fontsize=10];" << std::endl;
}
void DotPrinter::operator()(const chunk &ast)
{
    const auto self = getId();
    node(self, "chunk");
    (*this)(self, ast.block_);
}
void DotPrinter::operator()(const intptr_t parent, const ast::block &block)
{
    const auto self = getId();
    node(self, "block");
    edge(parent, self);
    for (const auto &stat : block.stat_)
        (*this)(self, stat);
    (*this)(self, block.retstat_);
}
void DotPrinter::operator()(const intptr_t parent, const ast::stat &stat)
{
    boost::apply_visitor([=](const auto &stat_any) { (*this)(parent, stat_any); }, stat);
}
void DotPrinter::operator()(const intptr_t parent, const ast::exp &exp)
{
    boost::apply_visitor([=](const auto &any) { (*this)(parent, any); }, exp);
}
void DotPrinter::operator()(const intptr_t parent, const std::string &value)
{
    const auto self = getId();
    node(self, "string");
    edge(parent, self);
}
void DotPrinter::operator()(const intptr_t parent, const double value)
{
    const auto self = getId();
    node(self, "double");
    edge(parent, self);
}
void DotPrinter::operator()(const intptr_t parent, const unsigned value)
{
    const auto self = getId();
    node(self, "unsigned");
    edge(parent, self);
}
void DotPrinter::operator()(const intptr_t parent, const bool value)
{
    const auto self = getId();
    node(self, "bool");
    edge(parent, self);
}
void DotPrinter::operator()(const intptr_t parent, const unary &value)
{
    const auto self = getId();
    node(self, "unary");
    edge(parent, self);
    (*this)(self, value.operator_);
    (*this)(self, value.rhs_);
}
void DotPrinter::operator()(const intptr_t parent, const binary &value)
{
    const auto self = getId();
    node(self, "binary");
    edge(parent, self);
    (*this)(self, value.operator_);
    (*this)(self, value.rhs_);
}
void DotPrinter::operator()(const intptr_t parent, const expression &value)
{
    (*this)(parent, value.first_);
    (*this)(parent, value.rest_);
}
void DotPrinter::operator()(const intptr_t parent, const label &value)
{
    const auto self = getId();
    node(self, "label");
    edge(parent, self);
    (*this)(self, value.name_);
}
void DotPrinter::operator()(const intptr_t parent, const Name &value)
{
    const auto self = getId();
    node(self, "Name");
    edge(parent, self);
}
void DotPrinter::operator()(const intptr_t parent, const funcname &value)
{
    const auto self = getId();
    node(self, "funcname");
    edge(parent, self);
    (*this)(self, value.names_);
    (*this)(self, value.self_chain_);
}
void DotPrinter::operator()(const intptr_t parent, const function &value)
{
    const auto self = getId();
    node(self, "function");
    edge(parent, self);
    (*this)(self, value.funcname_);
    (*this)(self, value.funcbody_);
}
void DotPrinter::operator()(const intptr_t parent, const local_function &value)
{
    const auto self = getId();
    node(self, "local_function");
    edge(parent, self);
    (*this)(self, value.funcname_);
    (*this)(self, value.funcbody_);
}
void DotPrinter::operator()(const intptr_t parent, const namelist &value)
{
    const auto self = getId();
    node(self, "namelist");
    edge(parent, self);
    (*this)(self, value.name_);
    for (const auto &name : value.chain_)
        (*this)(self, name);
}
void DotPrinter::operator()(const intptr_t parent, const funcbody &value)
{
    const auto self = getId();
    node(self, "funcbody");
    edge(parent, self);
    (*this)(self, value.parameters_);
    (*this)(self, value.block_);
}
void DotPrinter::operator()(const intptr_t parent, const functiondef &value)
{
    const auto self = getId();
    node(self, "functiondef");
    edge(parent, self);
    (*this)(self, value.funcbody_);
}
void DotPrinter::operator()(const intptr_t parent, const field &value)
{
    const auto self = getId();
    node(self, "field");
    edge(parent, self);
    (*this)(self, value.first);
    (*this)(self, value.second);
}
void DotPrinter::operator()(const intptr_t parent, const fieldlist &value)
{
    const auto self = getId();
    node(self, "fieldlist");
    edge(parent, self);
    for (const auto &field : value)
        (*this)(self, field);
}
void DotPrinter::operator()(const intptr_t parent, const tableconstructor &value)
{
    const auto self = getId();
    node(self, "table");
    edge(parent, self);
    (*this)(self, value.first_);
    (*this)(self, value.fields_);
}
void DotPrinter::operator()(const intptr_t parent, const for_namelist &value)
{
    const auto self = getId();
    node(self, "for");
    edge(parent, self);
    (*this)(self, value.name_list_);
    (*this)(self, value.exp_list_);
    (*this)(self, value.block_);
}
void DotPrinter::operator()(const intptr_t parent, const prefixexp &value)
{
    boost::apply_visitor([=](const auto &any) { (*this)(parent, any); }, value);
}
void DotPrinter::operator()(const intptr_t parent, const functioncall &value)
{
    const auto self = getId();
    node(self, "functioncall");
    edge(parent, self);
    (*this)(self, value.prefix_exp_);
    if (value.name_.has_value())
        (*this)(self, value.name_.value());
    (*this)(self, value.args_);
}
void DotPrinter::operator()(const intptr_t parent, const args &value)
{
    const auto self = getId();
    node(self, "args");
    edge(parent, self);
    boost::apply_visitor([=](const auto &any) { (*this)(self, any); }, value);
}
void DotPrinter::operator()(const intptr_t parent, const explist &value)
{
    const auto self = getId();
    node(self, "explist");
    edge(parent, self);
    for (const auto &exp : value)
        (*this)(self, exp);
}
void DotPrinter::operator()(const intptr_t parent, const var &value)
{
    const auto self = getId();
    node(self, "var");
    edge(parent, self);
    boost::apply_visitor([=](const auto &any) { (*this)(self, any); }, value);
}
void DotPrinter::operator()(const intptr_t parent, const var_wrapper &value)
{
    (*this)(parent, value.var_);
    (*this)(parent, value.next_);
}
void DotPrinter::operator()(const intptr_t parent, const varlist &value)
{
    const auto self = getId();
    node(self, "=");
    edge(parent, self);
    for (const auto &var : value.rest_)
        (*this)(self, var);
    (*this)(self, value.explist_);
}
void DotPrinter::operator()(const intptr_t parent, const primaryexpression &value)
{
    (*this)(parent, value.first_);
    (*this)(parent, value.rest_);
}
void DotPrinter::operator()(const intptr_t parent, const assign_or_call &value)
{
    (*this)(parent, value.primaryexp_);
    boost::apply_visitor([=](const auto &any) { (*this)(parent, any); }, value.var_action_);
}
void DotPrinter::operator()(const intptr_t parent, const ifelse &value)
{
    const auto self = getId();
    node(self, "if");
    edge(parent, self);
    (*this)(self, value.first_);
    for (const auto &ifr : value.rest_)
        (*this)(self, ifr);
    if (value.else_.has_value())
        (*this)(self, value.else_.value());
}
void DotPrinter::operator()(const intptr_t parent, const whiledo &value)
{
    const auto self = getId();
    node(self, "while");
    edge(parent, self);
    (*this)(self, value.exp_);
    (*this)(self, value.block_);
}
void DotPrinter::operator()(const intptr_t parent, const repeatuntil &value)
{
    const auto self = getId();
    node(self, "repeat");
    edge(parent, self);
    (*this)(self, value.block_);
    (*this)(self, value.exp_);
}
void DotPrinter::operator()(const intptr_t parent, const doblock &value)
{
    const auto self = getId();
    node(self, "do");
    edge(parent, self);
    (*this)(self, value.block_);
}
void DotPrinter::operator()(const intptr_t parent, const forexp &value)
{
    const auto self = getId();
    node(self, "forexp");
    edge(parent, self);
    (*this)(self, value.name_);
    (*this)(self, value.exp_first_);
    (*this)(self, value.exp_second_);
    (*this)(self, value.exp_third_);
    (*this)(self, value.block_);
}
void DotPrinter::operator()(const intptr_t parent, const local_attnamelist_assign &value)
{
    const auto self = getId();
    node(self, "local att =");
    edge(parent, self);
    (*this)(self, value.attnamelist_);
    (*this)(self, value.explist_);
}
void DotPrinter::operator()(const intptr_t parent, const attnamelist &value)
{
    const auto self = getId();
    node(self, "attnamelist");
    edge(parent, self);
    (*this)(self, value.first_);
    for (const auto &r : value.rest_)
        (*this)(self, r);
}
void DotPrinter::operator()(const intptr_t parent, const name_attrib_pair &value)
{
    const auto self = getId();
    node(self, "name_attrib");
    edge(parent, self);
    (*this)(self, value.name_);
    if (value.attrib_.has_value())
        (*this)(self, value.attrib_.value());
}
void DotPrinter::operator()(const intptr_t parent, const ifelse_wrapper &value)
{
    const auto self = getId();
    node(self, "ifelse");
    edge(parent, self);
    (*this)(self, value.exp_);
    (*this)(self, value.block_);
}

void DotPrinter::operator()(const intptr_t parent, const goto_stmt &value)
{
    const auto self = getId();
    node(self, "goto");
    edge(parent, self);
    (*this)(self, value.name_);
}
void DotPrinter::operator()(const intptr_t parent, const keyword_stmt &value)
{
    const auto self = getId();
    node(self, std::to_string(value.keyword_));
    edge(parent, self);
}
void DotPrinter::operator()(const intptr_t parent, const numeral &value)
{
    boost::apply_visitor([=](const auto &any) { (*this)(parent, any); }, value);
}
void DotPrinter::operator()(const intptr_t parent, const nil &nil)
{}

void DotPrinter::operator()(const intptr_t parent, const optoken &op)
{
    if (auto it = kOptokenSymbols.find(op); it != kOptokenSymbols.end())
    {
        const auto self = getId();
        node(self, it->second);
        edge(parent, self);
    }
}
void DotPrinter::node(const intptr_t parent, const std::string &label)
{
    out_ << "\t" << std::hex << "\"0x" << parent << std::dec << "\" [label=\"" << label << "\"]" << std::endl;
}
void DotPrinter::edge(const intptr_t parent, const intptr_t child)
{
    out_ << "\t" << std::hex << "\"0x" << parent << "\" -> "
         << "\"0x" << child << std::dec << "\"" << std::endl;
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
