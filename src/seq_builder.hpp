#pragma once
#include "ast/ast.hpp"
#include "types.hpp"
namespace sre::lua::ast
{
class SeqBuilder
{
    const Clones &clones_;
    size_t min_seq_len_;
    std::vector<std::vector<ClonePair>> clone_seq_;

  public:
    explicit SeqBuilder(const Clones &clones, size_t min_seq_len);
    const std::vector<std::vector<ClonePair>> &subsequences() const;
    //! \returns SeqBuilder& for self chain
    SeqBuilder &operator()(const chunk &ast);
    void operator()(const block &block);
    void operator()(const stat &stat);
    void operator()(const exp &exp);
    void operator()(const explist &value);
    void operator()(const prefixexp &value);
    void operator()(const expression &expression);
    void operator()(const primaryexpression &value);
    void operator()(const assign_or_call &value);
    void operator()(const label &value);
    void operator()(const funcname &value);
    void operator()(const function &value);
    void operator()(const local_function &value);
    void operator()(const namelist &value);
    void operator()(const functiondef &value);
    void operator()(const field &value);
    void operator()(const fieldlist &value);
    void operator()(const tableconstructor &value);
    void operator()(const for_namelist &value);
    void operator()(const funcbody &value);
    void operator()(const functioncall &value);
    void operator()(const args &value);
    void operator()(const var &value);
    void operator()(const var_wrapper &value);
    void operator()(const varlist &value);
    void operator()(const var_assign_or_list &value);
    void operator()(const whiledo &value);
    void operator()(const repeatuntil &value);
    void operator()(const doblock &value);
    void operator()(const forexp &value);
    void operator()(const local_attnamelist_assign &value);
    void operator()(const attnamelist &value);
    void operator()(const name_attrib_pair &value);
    void operator()(const ifelse &value);
    void operator()(const ifelse_wrapper &value);
    void operator()(const goto_stmt &value);
    void operator()(const unary &unary);
    void operator()(const binary &bin);
    void operator()(const keyword_stmt &value);
    void operator()(const numeral &value);
    void operator()(const Name &value);
    void operator()(const std::string &value);
    void operator()(const double &value);
    void operator()(const unsigned &value);
    void operator()(const bool &value);
    void operator()(const optoken &op);
    void operator()(const nil &nil);

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
