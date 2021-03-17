#include "mass.hpp"
#include <iostream>
namespace sre::lua::ast
{
std::size_t Mass::operator()(const chunk &ast) const
{
    std::cout << "chunk mass " << 1 + (*this)(ast.block_) << std::endl;
    return 1 + (*this)(ast.block_);
}
std::size_t Mass::operator()(const block &block) const
{
    std::size_t x = 1;
    for (const auto &v : block.stat_)
        x += (*this)(v);
    x += (*this)(block.retstat_);
    return x;
}
std::size_t Mass::operator()(const stat &stat) const
{
    return 1 + boost::apply_visitor(Mass{}, stat);
}
std::size_t Mass::operator()(const exp &exp) const
{
    return boost::apply_visitor(Mass{}, exp);
}
std::size_t Mass::operator()(const explist &value) const
{
    std::size_t x = 1;
    for (const auto &v : value)
        x += (*this)(v);
    return x;
}
std::size_t Mass::operator()(const prefixexp &value) const
{
    return 1 + boost::apply_visitor(Mass{}, value);
}
std::size_t Mass::operator()(const expression &expression) const
{
    const std::size_t x = (*this)(expression.first_);
    return x + (*this)(expression.rest_);
}
std::size_t Mass::operator()(const primaryexpression &value) const
{
    return (*this)(value.first_) + (*this)(value.rest_);
}
std::size_t Mass::operator()(const assign_or_call &value) const
{
    return (*this)(value.primaryexp_) + (*this)(value.var_action_);
}
std::size_t Mass::operator()(const label &value) const
{
    return 1 + (*this)(value.name_);
}
std::size_t Mass::operator()(const funcname &value) const
{
    return 1 + (*this)(value.names_) + (*this)(value.self_chain_);
}
std::size_t Mass::operator()(const function &value) const
{
    return 1 + (*this)(value.funcname_) + (*this)(value.funcbody_);
}
std::size_t Mass::operator()(const local_function &value) const
{
    return 1 + (*this)(value.funcname_) + (*this)(value.funcbody_);
}
std::size_t Mass::operator()(const namelist &value) const
{
    return 1 + (*this)(value.name_) + [&]() {
        size_t x = 0;
        for (const auto &v : value.chain_)
            x += (*this)(v);
        return x;
    }();
}
std::size_t Mass::operator()(const functiondef &value) const
{
    return 1 + (*this)(value.funcbody_);
}
std::size_t Mass::operator()(const field &value) const
{
    return 1 + (*this)(value.first) + (*this)(value.second);
}
std::size_t Mass::operator()(const fieldlist &value) const
{
    std::size_t x = 1;
    for (const auto &v : value)
        x += (*this)(v);
    return x;
}
std::size_t Mass::operator()(const tableconstructor &value) const
{
    return 1 + (*this)(value.first_) + (*this)(value.fields_);
}
std::size_t Mass::operator()(const for_namelist &value) const
{
    return 1 + (*this)(value.name_list_) + (*this)(value.exp_list_) + (*this)(value.block_);
}
std::size_t Mass::operator()(const funcbody &value) const
{
    return 1 + (*this)(value.parameters_) + (*this)(value.block_);
}
std::size_t Mass::operator()(const functioncall &value) const
{
    return 1 + (value.name_.has_value() ? (*this)(value.name_.value()) : 0) + (*this)(value.args_) +
           (*this)(value.prefix_exp_);
}
std::size_t Mass::operator()(const args &value) const
{
    return 1 + boost::apply_visitor(Mass{}, value);
}
std::size_t Mass::operator()(const var &value) const
{
    return 1 + boost::apply_visitor(Mass{}, value);
}
std::size_t Mass::operator()(const var_wrapper &value) const
{
    return (*this)(value.var_) + (*this)(value.next_);
}
std::size_t Mass::operator()(const varlist &value) const
{
    std::size_t x = 1 + (*this)(value.explist_);
    for (const auto &v : value.rest_)
        x += (*this)(v);
    return x;
}
std::size_t Mass::operator()(const var_assign_or_list &value) const
{
    return boost::apply_visitor(Mass{}, value);
}
std::size_t Mass::operator()(const whiledo &value) const
{
    return 1 + (*this)(value.exp_) + (*this)(value.block_);
}
std::size_t Mass::operator()(const repeatuntil &value) const
{
    return 1 + (*this)(value.exp_) + (*this)(value.block_);
}
std::size_t Mass::operator()(const doblock &value) const
{
    return 1 + (*this)(value.block_);
}
std::size_t Mass::operator()(const forexp &value) const
{
    return 1 + (*this)(value.name_) + (*this)(value.exp_first_) + (*this)(value.exp_second_) +
           (*this)(value.exp_third_) + (*this)(value.block_);
}
std::size_t Mass::operator()(const local_attnamelist_assign &value) const
{
    return 1 + (*this)(value.attnamelist_) + (*this)(value.explist_);
}
std::size_t Mass::operator()(const attnamelist &value) const
{
    return 1 + (*this)(value.first_) + [&]() {
        std::size_t x = 0;
        for (const auto &v : value.rest_)
            x += (*this)(v);
        return x;
    }();
}
std::size_t Mass::operator()(const name_attrib_pair &value) const
{
    return (*this)(value.name_) + (value.attrib_.has_value() ? (*this)(value.attrib_.value()) : 0);
}
std::size_t Mass::operator()(const ifelse &value) const
{
    return 1 + (*this)(value.first_) + [&]() {
        std::size_t x = 0;
        for (const auto &v : value.rest_)
            x += (*this)(v);
        return x;
    }() + (value.else_.has_value() ? (*this)(value.else_.value()) : 0);
}
std::size_t Mass::operator()(const ifelse_wrapper &value) const
{
    return 1 + (*this)(value.exp_) + (*this)(value.block_);
}
std::size_t Mass::operator()(const goto_stmt &value) const
{
    return 1 + (*this)(value.name_);
}
std::size_t Mass::operator()(const unary &unary) const
{
    return 1 + (*this)(unary.operator_) + (*this)(unary.rhs_);
}
std::size_t Mass::operator()(const binary &bin) const
{
    return 1 + (*this)(bin.operator_) + (*this)(bin.rhs_);
}
std::size_t Mass::operator()(const keyword_stmt &) const
{
    return 1;
}
std::size_t Mass::operator()(const numeral &) const
{
    return 1;
}
std::size_t Mass::operator()(const Name &) const
{
    return 1;
}
std::size_t Mass::operator()(const std::string &) const
{
    return 1;
}
std::size_t Mass::operator()(const double &) const
{
    return 1;
}
std::size_t Mass::operator()(const unsigned &) const
{
    return 1;
}
std::size_t Mass::operator()(const bool &) const
{
    return 1;
}
std::size_t Mass::operator()(const optoken &) const
{
    return 1;
}
std::size_t Mass::operator()(const nil &) const
{
    return 0;
}
} // namespace sre::lua::ast
