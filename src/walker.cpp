#include "walker.hpp"
#include <iostream>

namespace sre::lua::ast
{
// we already have a match for both subtrees. so return when != 0
#define IS(DBG_STR)                                                                                                    \
    using val_type = std::remove_cv<std::remove_reference<decltype(value)>::type>::type;                               \
    auto start_it = clones_.begin();                                                                                   \
    if (is<val_type, const val_type *>(value, start_it, clones_.end()))                                                \
    {                                                                                                                  \
        clone_matches_.push_back(*start_it);                                                                           \
        if (prev_clone_matches_.size() != 0)                                                                           \
            return true;                                                                                               \
    }

#define LIST(var)                                                                                                      \
    for (const auto &v : var)                                                                                          \
        if ((*this)(v))                                                                                                \
            return true;
#define VISITR return boost::apply_visitor(*this, value);
#define VISIT_COND(var)                                                                                                \
    if (boost::apply_visitor(*this, var))                                                                              \
        return true;
#define F return false;
#define R_COND(var)                                                                                                    \
    if ((*this)(var))                                                                                                  \
        return true;
#define R(var) return (*this)(var);

Walker::Walker(const Clones &clones, const Clones &prevs)
    : clones_{clones}
    , prev_clone_matches_{prevs}
{}

bool Walker::operator()(const chunk &value)
{
    IS("chunk");
    R(value.block_);
}

bool Walker::operator()(const block &value)
{
    IS("block");
    LIST(value.stat_)
    R(value.retstat_)
}
bool Walker::operator()(const stat &value)
{
    IS("stat");
    VISITR
}
bool Walker::operator()(const exp &value)
{
    IS("exp");
    VISITR
}
bool Walker::operator()(const explist &value)
{
    IS("explist") LIST(value) F
}
bool Walker::operator()(const prefixexp &value)
{
    IS("prefixexp") VISITR
}
bool Walker::operator()(const expression &value)
{
    IS("expression") R_COND(value.first_) R(value.rest_)
}
bool Walker::operator()(const primaryexpression &value)
{
    IS("primaryexpression")
    R_COND(value.first_)
    R(value.rest_)
}
bool Walker::operator()(const assign_or_call &value)
{
    IS("assign_or_call");
    R_COND(value.primaryexp_)
    R(value.var_action_)
}
bool Walker::operator()(const label &value)
{
    IS("label");
    R(value.name_)
}
bool Walker::operator()(const funcname &value)
{
    IS("funcname");
    R_COND(value.names_)
    R(value.self_chain_)
}
bool Walker::operator()(const function &value)
{
    IS("function");
    R_COND(value.funcname_)
    R(value.funcbody_)
}
bool Walker::operator()(const local_function &value)
{
    IS("local_function");
    R_COND(value.funcname_)
    R(value.funcbody_)
}
bool Walker::operator()(const namelist &value)
{
    IS("namelist");
    R_COND(value.name_)
    LIST(value.chain_)
    F
}
bool Walker::operator()(const functiondef &value)
{
    IS("functiondef");
    R(value.funcbody_)
}
bool Walker::operator()(const field &value)
{
    IS("field");
    R_COND(value.first)
    R(value.second)
}
bool Walker::operator()(const fieldlist &value)
{
    IS("fieldlist");
    LIST(value)
    F
}
bool Walker::operator()(const tableconstructor &value)
{
    IS("tableconstructor");
    R_COND(value.first_)
    R(value.fields_)
}
bool Walker::operator()(const for_namelist &value)
{
    IS("for_namelist");
    R_COND(value.name_list_)
    R_COND(value.exp_list_)
    R(value.block_)
}
bool Walker::operator()(const funcbody &value)
{
    IS("funcbody");
    R_COND(value.parameters_)
    R(value.block_)
}
bool Walker::operator()(const functioncall &value)
{
    IS("functioncall");
    R_COND(value.args_)
    if (value.name_.has_value())
        R_COND(value.name_.value()) R(value.prefix_exp_)
}
bool Walker::operator()(const args &value)
{
    IS("args") VISITR
}
bool Walker::operator()(const var &value)
{
    IS("var") VISITR
}
bool Walker::operator()(const var_wrapper &value)
{
    IS("var_wrapper");
    R_COND(value.var_)
    R(value.next_)
}
bool Walker::operator()(const varlist &value)
{
    IS("varlist");
    R_COND(value.explist_)
    LIST(value.rest_)
    F
}
bool Walker::operator()(const var_assign_or_list &value)
{
    IS("var_assign_or_list") VISITR
}
bool Walker::operator()(const whiledo &value)
{
    IS("whiledo");
    R_COND(value.exp_)
    R(value.block_)
}
bool Walker::operator()(const repeatuntil &value)
{
    IS("repeatuntil");
    R_COND(value.block_)
    R(value.exp_)
}
bool Walker::operator()(const doblock &value)
{
    IS("doblock");
    R(value.block_)
}
bool Walker::operator()(const forexp &value)
{
    IS("forexp");
    R_COND(value.name_)
    R_COND(value.exp_first_)
    R_COND(value.exp_second_)
    R_COND(value.exp_third_)
    R(value.name_)
}
bool Walker::operator()(const local_attnamelist_assign &value)
{
    IS("local_attnamelist_assign");
    R_COND(value.attnamelist_)
    R(value.explist_)
}
bool Walker::operator()(const attnamelist &value)
{
    IS("attnamelist");
    R_COND(value.first_)
    LIST(value.rest_)
    F
}
bool Walker::operator()(const name_attrib_pair &value)
{
    IS("name_attrib_pair");
    R_COND(value.name_)
    if (value.attrib_.has_value())
        R(value.attrib_.value())
    F
}
bool Walker::operator()(const ifelse &value)
{
    IS("ifelse");
    R_COND(value.first_)
    LIST(value.rest_)
    if (value.else_.has_value())
        R(value.else_.value())
    F
}
bool Walker::operator()(const ifelse_wrapper &value)
{
    IS("ifelse_wrapper");
    R_COND(value.exp_)
    R(value.block_)
}
bool Walker::operator()(const goto_stmt &value)
{
    IS("goto_stmt");
    R(value)
}
bool Walker::operator()(const unary &value)
{
    IS("unary");
    R(value.rhs_)
}
bool Walker::operator()(const binary &value)
{
    IS("binary");
    R(value.rhs_)
}
bool Walker::operator()(const keyword_stmt &value)
{
    IS("keyword_stmt");
    F
}
bool Walker::operator()(const numeral &value)
{
    IS("numeral");
    F
}
bool Walker::operator()(const Name &value)
{
    F
}
bool Walker::operator()(const std::string &value)
{
    F
}
bool Walker::operator()(const double &value)
{
    F
}
bool Walker::operator()(const unsigned &value)
{
    F
}
bool Walker::operator()(const bool &value)
{
    F
}
bool Walker::operator()(const nil &nil){F}

Clones Walker::clones() const
{
    return clone_matches_;
}
} // namespace sre::lua::ast
