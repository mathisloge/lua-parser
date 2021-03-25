#pragma once
#include "ast/ast.hpp"
namespace sre::lua::ast
{
struct Hasher
{
    Hasher();
    std::size_t operator()(const chunk &ast) const;
    std::size_t operator()(const block &block) const;
    std::size_t operator()(const stat &stat) const;
    std::size_t operator()(const exp &exp) const;
    std::size_t operator()(const explist &value) const;
    std::size_t operator()(const prefixexp &value) const;
    std::size_t operator()(const expression &expression) const;
    std::size_t operator()(const primaryexpression &value) const;
    std::size_t operator()(const assign_or_call &value) const;
    std::size_t operator()(const label &value) const;
    std::size_t operator()(const funcname &value) const;
    std::size_t operator()(const function &value) const;
    std::size_t operator()(const local_function &value) const;
    std::size_t operator()(const namelist &value) const;
    std::size_t operator()(const functiondef &value) const;
    std::size_t operator()(const field &value) const;
    std::size_t operator()(const fieldlist &value) const;
    std::size_t operator()(const tableconstructor &value) const;
    std::size_t operator()(const for_namelist &value) const;
    std::size_t operator()(const funcbody &value) const;
    std::size_t operator()(const functioncall &value) const;
    std::size_t operator()(const args &value) const;
    std::size_t operator()(const var &value) const;
    std::size_t operator()(const var_wrapper &value) const;
    std::size_t operator()(const var_assign_or_list &value) const;
    std::size_t operator()(const varlist &value) const;
    std::size_t operator()(const whiledo &value) const;
    std::size_t operator()(const repeatuntil &value) const;
    std::size_t operator()(const doblock &value) const;
    std::size_t operator()(const forexp &value) const;
    std::size_t operator()(const local_attnamelist_assign &value) const;
    std::size_t operator()(const attnamelist &value) const;
    std::size_t operator()(const name_attrib_pair &value) const;
    std::size_t operator()(const ifelse &value) const;
    std::size_t operator()(const ifelse_wrapper &value) const;
    std::size_t operator()(const goto_stmt &value) const;
    std::size_t operator()(const unary &unary) const;
    std::size_t operator()(const binary &bin) const;
    std::size_t operator()(const keyword_stmt &value) const;
    std::size_t operator()(const numeral &value) const;
    std::size_t operator()(const Name &value) const;
    std::size_t operator()(const std::string &value) const;
    std::size_t operator()(const double &value) const;
    std::size_t operator()(const unsigned &value) const;
    std::size_t operator()(const bool &value) const;
    std::size_t operator()(const nil &nil) const;
};
} // namespace sre::lua::ast
