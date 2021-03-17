#pragma once
#include <functional>
#include "types.hpp"
namespace sre::lua::ast
{


class Bucketer final
{
  public:
    explicit Bucketer(BucketList &bucket, std::size_t min_mass);
    void toBuckets(const chunk &chunkt);

  private:
    BucketList &bucket_;
    std::size_t min_mass_;

  public:
    void operator()(const chunk &ast);
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
};
} // namespace sre::lua::ast
