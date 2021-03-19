#include "seq_builder.hpp"
#include <iostream>
namespace sre::lua::ast
{
SeqBuilder::SeqBuilder(const Clones &clones)
    : clones_{clones}
{}

bool SeqBuilder::operator()(const chunk &ast)
{
    std::cout << "chunk mass " << 1 + (*this)(ast.block_) << std::endl;
    return 1 + (*this)(ast.block_);
}
bool SeqBuilder::operator()(const block &block)
{
    for (const auto &c : clones_)
    {
        // .index returns the stored type. if the indexes don't match, it isn't the same type. It shouldn't happen
        // either since the hashes should be different. but we could cope with hash collisions.
        if (c.first.index() != c.second.index())
            continue;

        std::pair<bool, bool> clone_found{false, false};
        for (const auto &s : block.stat_)
        {
            const auto idx = is<stat>(s, c);
            if (idx == 1)
            {
                if (!clone_found.first)
                    clone_found.first = true;
                else
                    std::cout << "got same clone for first...?" << std::endl;
            }
            else if (idx == 2)
            {
                if (!clone_found.second)
                    clone_found.second = true;
                else
                    std::cout << "got same clone for second...?" << std::endl;
            }

            if (clone_found.first && clone_found.second)
                break;
        }

        if (clone_found.first && clone_found.second){
            
        }
    }

    return true;
}
bool SeqBuilder::operator()(const stat &stat)
{
    return 1 + boost::apply_visitor(*this, stat);
}
bool SeqBuilder::operator()(const exp &exp)
{
    return 1 + boost::apply_visitor(*this, exp);
}
bool SeqBuilder::operator()(const explist &value)
{
    bool x = 1;
    for (const auto &v : value)
        x += (*this)(v);
    return x;
}
bool SeqBuilder::operator()(const prefixexp &value)
{
    return 1 + boost::apply_visitor(*this, value);
}
bool SeqBuilder::operator()(const expression &expression)
{
    bool x = (*this)(expression.first_);
    return x + (*this)(expression.rest_);
}
bool SeqBuilder::operator()(const primaryexpression &value)
{
    return 1 + (*this)(value.first_) + (*this)(value.rest_);
}
bool SeqBuilder::operator()(const assign_or_call &value)
{
    return 1 + (*this)(value.primaryexp_) + (*this)(value.var_action_);
}
bool SeqBuilder::operator()(const label &value)
{
    return 1 + (*this)(value.name_);
}
bool SeqBuilder::operator()(const funcname &value)
{
    return 1 + (*this)(value.names_) + (*this)(value.self_chain_);
}
bool SeqBuilder::operator()(const function &value)
{
    return 1 + (*this)(value.funcname_) + (*this)(value.funcbody_);
}
bool SeqBuilder::operator()(const local_function &value)
{
    return 1 + (*this)(value.funcname_) + (*this)(value.funcbody_);
}
bool SeqBuilder::operator()(const namelist &value)
{
    return 1 + (*this)(value.name_) + [&]() {
        size_t x = 0;
        for (const auto &v : value.chain_)
            x += (*this)(v);
        return x;
    }();
}
bool SeqBuilder::operator()(const functiondef &value)
{
    return 1 + (*this)(value.funcbody_);
}
bool SeqBuilder::operator()(const field &value)
{
    return 1 + (*this)(value.first) + (*this)(value.second);
}
bool SeqBuilder::operator()(const fieldlist &value)
{
    bool x = 1;
    for (const auto &v : value)
        x += (*this)(v);
    return x;
}
bool SeqBuilder::operator()(const tableconstructor &value)
{
    return 1 + (*this)(value.first_) + (*this)(value.fields_);
}
bool SeqBuilder::operator()(const for_namelist &value)
{
    return 1 + (*this)(value.name_list_) + (*this)(value.exp_list_) + (*this)(value.block_);
}
bool SeqBuilder::operator()(const funcbody &value)
{
    return 1 + (*this)(value.parameters_) + (*this)(value.block_);
}
bool SeqBuilder::operator()(const functioncall &value)
{
    return 1 + (value.name_.has_value() ? (*this)(value.name_.value()) : 0) + (*this)(value.args_) +
           (*this)(value.prefix_exp_);
}
bool SeqBuilder::operator()(const args &value)
{
    return 1 + boost::apply_visitor(*this, value);
}
bool SeqBuilder::operator()(const var &value)
{
    return 1 + boost::apply_visitor(*this, value);
}
bool SeqBuilder::operator()(const var_wrapper &value)
{
    return 1 + (*this)(value.var_) + (*this)(value.next_);
}
bool SeqBuilder::operator()(const varlist &value)
{
    bool x = 1 + (*this)(value.explist_);
    for (const auto &v : value.rest_)
        x += (*this)(v);
    return x;
}
bool SeqBuilder::operator()(const var_assign_or_list &value)
{
    return 1 + boost::apply_visitor(*this, value);
}
bool SeqBuilder::operator()(const whiledo &value)
{
    return 1 + (*this)(value.exp_) + (*this)(value.block_);
}
bool SeqBuilder::operator()(const repeatuntil &value)
{
    return 1 + (*this)(value.exp_) + (*this)(value.block_);
}
bool SeqBuilder::operator()(const doblock &value)
{
    return 1 + (*this)(value.block_);
}
bool SeqBuilder::operator()(const forexp &value)
{
    return 1 + (*this)(value.name_) + (*this)(value.exp_first_) + (*this)(value.exp_second_) +
           (*this)(value.exp_third_) + (*this)(value.block_);
}
bool SeqBuilder::operator()(const local_attnamelist_assign &value)
{
    return 1 + (*this)(value.attnamelist_) + (*this)(value.explist_);
}
bool SeqBuilder::operator()(const attnamelist &value)
{
    return 1 + (*this)(value.first_) + [&]() {
        bool x = 0;
        for (const auto &v : value.rest_)
            x += (*this)(v);
        return x;
    }();
}
bool SeqBuilder::operator()(const name_attrib_pair &value)
{
    return 1 + (*this)(value.name_) + (value.attrib_.has_value() ? (*this)(value.attrib_.value()) : 0);
}
bool SeqBuilder::operator()(const ifelse &value)
{
    return 1 + (*this)(value.first_) + [&]() {
        bool x = 0;
        for (const auto &v : value.rest_)
            x += (*this)(v);
        return x;
    }() + (value.else_.has_value() ? (*this)(value.else_.value()) : 0);
}
bool SeqBuilder::operator()(const ifelse_wrapper &value)
{
    return (*this)(value.exp_) + (*this)(value.block_);
}
bool SeqBuilder::operator()(const goto_stmt &value)
{
    return 1 + (*this)(value.name_);
}
bool SeqBuilder::operator()(const unary &unary)
{
    return 1 + (*this)(unary.operator_) + (*this)(unary.rhs_);
}
bool SeqBuilder::operator()(const binary &bin)
{
    return 1 + (*this)(bin.operator_) + (*this)(bin.rhs_);
}
bool SeqBuilder::operator()(const keyword_stmt &)
{
    return 1;
}
bool SeqBuilder::operator()(const numeral &)
{
    return 1;
}
bool SeqBuilder::operator()(const Name &)
{
    return 1;
}
bool SeqBuilder::operator()(const std::string &)
{
    return 1;
}
bool SeqBuilder::operator()(const double &)
{
    return 1;
}
bool SeqBuilder::operator()(const unsigned &)
{
    return 1;
}
bool SeqBuilder::operator()(const bool &)
{
    return 1;
}
bool SeqBuilder::operator()(const optoken &)
{
    return 1;
}
bool SeqBuilder::operator()(const nil &)
{
    return 0;
}
} // namespace sre::lua::ast
