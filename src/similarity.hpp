#pragma once
#include <iostream>
#include "bucketer.hpp"
#include "mass.hpp"
#include "types.hpp"
namespace sre::lua::ast
{
class Similarity final : public boost::static_visitor<>
{
  public:
    Similarity();
    double run(const Unit &a, const Unit &b);

    // simple forward declaration since we need to use templates in this case..
    template <typename Ta>
    void operator()(const x3::forward_ast<Ta> &a, const x3::forward_ast<Ta> &b)
    {
        (*this)(a.get(), b.get());
    }

    template <typename Ta, typename Tb>
    void operator()(const Ta &x, const Tb &y)
    {
        L += Mass{}(x);
        R += Mass{}(y);
    }
    template <>
    void operator()(const stat &a, const stat &b);
    template <>
    void operator()(const block &a, const block &b);
    template <>
    void operator()(const chunk &a, const chunk &b);
    template <>
    void operator()(const exp &a, const exp &b);
    template <>
    void operator()(const explist &a, const explist &b);
    template <>
    void operator()(const prefixexp &a, const prefixexp &b);
    template <>
    void operator()(const expression &a, const expression &b);
    template <>
    void operator()(const primaryexpression &a, const primaryexpression &b);
    template <>
    void operator()(const assign_or_call &a, const assign_or_call &b);
    template <>
    void operator()(const label &a, const label &b);
    template <>
    void operator()(const funcname &a, const funcname &b);
    template <>
    void operator()(const function &a, const function &b);
    template <>
    void operator()(const local_function &a, const local_function &b);
    template <>
    void operator()(const namelist &a, const namelist &b);
    template <>
    void operator()(const functiondef &a, const functiondef &b);
    template <>
    void operator()(const field &a, const field &b);
    template <>
    void operator()(const fieldlist &a, const fieldlist &b);
    template <>
    void operator()(const tableconstructor &a, const tableconstructor &b);
    template <>
    void operator()(const for_namelist &a, const for_namelist &b);
    template <>
    void operator()(const funcbody &a, const funcbody &b);
    template <>
    void operator()(const functioncall &a, const functioncall &b);
    template <>
    void operator()(const args &a, const args &b);
    template <>
    void operator()(const var &a, const var &b);
    template <>
    void operator()(const var_wrapper &a, const var_wrapper &b);
    template <>
    void operator()(const varlist &a, const varlist &b);
    template <>
    void operator()(const var_assign_or_list &a, const var_assign_or_list &b);
    template <>
    void operator()(const whiledo &a, const whiledo &b);
    template <>
    void operator()(const repeatuntil &a, const repeatuntil &b);
    template <>
    void operator()(const doblock &a, const doblock &b);
    template <>
    void operator()(const forexp &a, const forexp &b);
    template <>
    void operator()(const local_attnamelist_assign &a, const local_attnamelist_assign &b);
    template <>
    void operator()(const attnamelist &a, const attnamelist &b);
    template <>
    void operator()(const name_attrib_pair &a, const name_attrib_pair &b);
    template <>
    void operator()(const ifelse &a, const ifelse &b);
    template <>
    void operator()(const ifelse_wrapper &a, const ifelse_wrapper &b);
    template <>
    void operator()(const goto_stmt &a, const goto_stmt &b);
    template <>
    void operator()(const unary &a, const unary &b);
    template <>
    void operator()(const binary &a, const binary &b);
    template <>
    void operator()(const optoken &a, const optoken &b);
    template <>
    void operator()(const keyword_stmt &a, const keyword_stmt &b);
    template <>
    void operator()(const numeral &a, const numeral &b);
    template <>
    void operator()(const Name &a, const Name &b);
    template <>
    void operator()(const std::string &a, const std::string &b);
    template <>
    void operator()(const double &a, const double &b);
    template <>
    void operator()(const unsigned &a, const unsigned &b);
    template <>
    void operator()(const bool &a, const bool &b);
    template <>
    void operator()(const nil &a, const nil &b);

  private:
    MassVal S;
    MassVal L;
    MassVal R;
};
} // namespace sre::lua::ast
