#pragma once
#include "ast/ast.hpp"
namespace sre::lua::ast
{
struct Hasher
{
    Hasher(int indent = 0);
    void operator()(const chunk &ast) const;
    void operator()(const block &block) const;
    void operator()(const stat &stat) const;
    void operator()(const exp &exp) const;
    void operator()(const unary &unary) const;
    void operator()(const binary &bin) const;
    void operator()(const expression &expression) const;
    void operator()(const label &value) const;

    void operator()(const funcname &value) const;
    void operator()(const function &value) const;
    void operator()(const local_function &value) const;
    void operator()(const namelist &value) const;
    void operator()(const functiondef &value) const;
    void operator()(const field &value) const;
    void operator()(const fieldlist &value) const;
    void operator()(const tableconstructor &value) const;
    void operator()(const for_namelist &value) const;
    void operator()(const prefixexp &value) const;
    void operator()(const functioncall &value) const;
    void operator()(const explist &value) const;
    void operator()(const var &value) const;
    void operator()(const var_wrapper &value) const;
    void operator()(const varlist &value) const;
    void operator()(const primaryexpression &value) const;
    void operator()(const assign_or_call &value) const;
    void operator()(const ifelse &value) const;
    void operator()(const whiledo &value) const;
    void operator()(const repeatuntil &value) const;
    void operator()(const doblock &value) const;
    void operator()(const forexp &value) const;
    void operator()(const local_attnamelist_assign &value) const;
    void operator()(const attnamelist &value) const;
    void operator()(const name_attrib_pair &value) const;
    void operator()(const ifelse_wrapper &value) const;
    void operator()(const goto_stmt &value) const;
    void operator()(const keyword_stmt &value) const;
    void operator()(const numeral &value) const;
    void operator()(const Name &value) const;
    void operator()(const std::string &value) const;
    void operator()(const double &value) const;
    void operator()(const unsigned &value) const;
    void operator()(const bool &value) const;
    void operator()(const nil &nil) const;
};
} // namespace sre::lua::ast
