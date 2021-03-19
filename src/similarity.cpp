#include "similarity.hpp"
#include <iostream>
#include "hasher.hpp"
#include "mass.hpp"

//! wenn teilbaum x != teilbaum y ist => L+= Mass von (x) R += Mass von (y)
namespace sre::lua::ast
{

#define op_begin(type)                                                                                                 \
    template <>                                                                                                        \
    void Similarity::operator()(const type &a, const type &b)                                                          \
    {                                                                                                                  \
        S += 1;

#define op_call(acc) (*this)(a.##acc, b.##acc);
#define op_call_opt(acc)                                                                                               \
    if (a.##acc.has_value() && b.##acc.has_value())                                                                    \
    {                                                                                                                  \
        op_call(acc.value());                                                                                          \
    }

#define op_visit(vara, varb) boost::apply_visitor(*this, vara, varb);
#define op_call_list_var(vara, varb)                                                                                   \
    const auto min_s = std::min(vara.size(), varb.size());                                                             \
    auto it_a = vara.begin();                                                                                          \
    auto it_b = varb.begin();                                                                                          \
    for (auto i = 0; i < min_s; i++)                                                                                   \
    {                                                                                                                  \
        (*this)(*it_a, *it_b);                                                                                         \
        it_a++;                                                                                                        \
        it_b++;                                                                                                        \
    }                                                                                                                  \
    if (min_s < vara.size())                                                                                           \
    {                                                                                                                  \
        auto it = vara.begin();                                                                                        \
        std::advance(it, min_s);                                                                                       \
        while (it != vara.end())                                                                                       \
        {                                                                                                              \
            L += Mass{}(*it);                                                                                          \
            it++;                                                                                                      \
        }                                                                                                              \
    }                                                                                                                  \
    else if (min_s < varb.size())                                                                                      \
    {                                                                                                                  \
        auto it = varb.begin();                                                                                        \
        std::advance(it, min_s);                                                                                       \
        while (it != varb.end())                                                                                       \
        {                                                                                                              \
            R += Mass{}(*it);                                                                                          \
            it++;                                                                                                      \
        }                                                                                                              \
    }
#define op_call_list(acc) op_call_list_var(a.##acc, b.##acc)

#define op_end }

Similarity::Similarity()
{}

double Similarity::run(const Unit &a, const Unit &b)
{
    R = 0;
    L = 0;
    S = 0;
    std::visit([this](auto a, auto b) { (*this)(*a, *b); }, a, b);
    const double div = (2. * double(S)) + double(L) + double(R);
    if (div > 0.)
        return (2. * double(S)) / div;
    return 0.;
}

op_begin(stat);
op_visit(a, b);
op_end;

op_begin(block);
op_call_list(stat_);
op_call(retstat_) op_end;

op_begin(chunk);
op_call(block_);
op_end;

op_begin(exp);
op_visit(a, b);
op_end;

op_begin(explist);
op_call_list_var(a, b);
op_end;

op_begin(prefixexp);
op_visit(a, b);
op_end;

op_begin(expression);
op_call(first_);
op_call(rest_);
op_end;

op_begin(primaryexpression);
op_call(first_);
op_call(rest_);
op_end;

op_begin(assign_or_call);
op_call(primaryexp_);
op_call(var_action_);
op_end;

op_begin(label);
op_call(name_);
op_end;

op_begin(funcname);
op_call(names_);
op_call(self_chain_);
op_end;

op_begin(function);
op_call(funcname_);
op_call(funcbody_);
op_end;

op_begin(local_function);
op_call(funcname_);
op_call(funcbody_);
op_end;

op_begin(namelist);
op_call(name_);
op_call_list(chain_);
op_end;

op_begin(functiondef);
op_call(funcbody_);
op_end;

op_begin(field);
op_call(first);
op_call(second);
op_end;

op_begin(fieldlist);
op_call_list_var(a, b);
op_end;

op_begin(tableconstructor);
op_call(first_);
op_call(fields_);
op_end;

op_begin(for_namelist);
op_call(name_list_);
op_call(exp_list_);
op_call(block_);
op_end;

op_begin(funcbody);
op_call(parameters_);
op_call(block_);
op_end;

op_begin(functioncall);
op_call_opt(name_);
op_call(args_);
op_call(prefix_exp_);
op_end;

op_begin(args);
op_visit(a, b);
op_end;

op_begin(var);
op_visit(a, b);
op_end;

op_begin(var_wrapper);
op_call(var_);
op_call(next_);
op_end;

op_begin(varlist);
op_call_list(rest_);
op_call(explist_);
op_end;

op_begin(var_assign_or_list);
op_visit(a, b);
op_end;

op_begin(whiledo);
op_call(exp_);
op_call(block_);
op_end;

op_begin(repeatuntil);
op_call(block_);
op_call(exp_);
op_end;

op_begin(doblock);
op_call(block_);
op_end;

op_begin(forexp);
op_call(name_);
op_call(exp_first_);
op_call(exp_second_);
op_call(exp_third_);
op_call(block_);
op_end;

op_begin(local_attnamelist_assign);
op_call(attnamelist_);
op_call(explist_);
op_end;

op_begin(attnamelist);
op_call(first_);
op_call_list(rest_);
op_end;

op_begin(name_attrib_pair);
op_call(name_);
op_call_opt(attrib_);
op_end;

op_begin(ifelse);
op_call(first_);
op_call_list(rest_);
op_call_opt(else_);
op_end;

op_begin(ifelse_wrapper);
op_call(exp_);
op_call(block_);
op_end;

op_begin(goto_stmt);
op_call(name_);
op_end;

op_begin(unary);
op_call(operator_);
op_call(rhs_);
op_end;

op_begin(binary);
op_call(operator_);
op_call(rhs_);
op_end;

template <>
void Similarity::operator()(const optoken &a, const optoken &b)
{
    if (a == b)
        S += 1;
    else
    {
        L += 1;
        R += 1;
    }
}

template <>
void Similarity::operator()(const keyword_stmt &a, const keyword_stmt &b)
{
    if (a.keyword_ == b.keyword_)
        S += 1;
    else
    {
        L += 1;
        R += 1;
    }
}

op_begin(numeral);
op_visit(a, b);
op_end;

op_begin(Name) op_end op_begin(std::string) op_end op_begin(double) op_end op_begin(unsigned) op_end
    op_begin(bool) op_end

    template <>
    void Similarity::operator()(const nil &a, const nil &b)
{}

} // namespace sre::lua::ast
