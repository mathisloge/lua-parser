#pragma once
#include "ast/ast.hpp"
#include "types.hpp"
namespace sre::lua::ast
{
class SeqBuilder
{
    const Clones &clones_;
    std::vector<std::vector<ClonePair>> clone_seq_;

  public:
    explicit SeqBuilder(const Clones &clones);
    bool operator()(const chunk &ast);
    bool operator()(const block &block);
    bool operator()(const stat &stat);
    bool operator()(const exp &exp);
    bool operator()(const explist &value);
    bool operator()(const prefixexp &value);
    bool operator()(const expression &expression);
    bool operator()(const primaryexpression &value);
    bool operator()(const assign_or_call &value);
    bool operator()(const label &value);
    bool operator()(const funcname &value);
    bool operator()(const function &value);
    bool operator()(const local_function &value);
    bool operator()(const namelist &value);
    bool operator()(const functiondef &value);
    bool operator()(const field &value);
    bool operator()(const fieldlist &value);
    bool operator()(const tableconstructor &value);
    bool operator()(const for_namelist &value);
    bool operator()(const funcbody &value);
    bool operator()(const functioncall &value);
    bool operator()(const args &value);
    bool operator()(const var &value);
    bool operator()(const var_wrapper &value);
    bool operator()(const varlist &value);
    bool operator()(const var_assign_or_list &value);
    bool operator()(const whiledo &value);
    bool operator()(const repeatuntil &value);
    bool operator()(const doblock &value);
    bool operator()(const forexp &value);
    bool operator()(const local_attnamelist_assign &value);
    bool operator()(const attnamelist &value);
    bool operator()(const name_attrib_pair &value);
    bool operator()(const ifelse &value);
    bool operator()(const ifelse_wrapper &value);
    bool operator()(const goto_stmt &value);
    bool operator()(const unary &unary);
    bool operator()(const binary &bin);
    bool operator()(const keyword_stmt &value);
    bool operator()(const numeral &value);
    bool operator()(const Name &value);
    bool operator()(const std::string &value);
    bool operator()(const double &value);
    bool operator()(const unsigned &value);
    bool operator()(const bool &value);
    bool operator()(const optoken &op);
    bool operator()(const nil &nil);

    template <typename T>
    int is(const T &val, const ClonePair &clone)
    {
        auto p1 = std::get_if<const T *>(&clone.first);
        auto p2 = std::get_if<const T *>(&clone.second);
        if (p1 && *p1 == &val)
            return 1;
        else if (p2 && *p2 == &val)
            return 2;
        return false;
    }
};
} // namespace sre::lua::ast
