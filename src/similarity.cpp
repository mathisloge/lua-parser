#include "similarity.hpp"
#include "hasher.hpp"
#include "mass.hpp"

//! wenn teilbaum x != teilbaum y ist => L+= Mass von (x) R += Mass von (y)
namespace sre::lua::ast
{

#define op_begin(type)                                                                                                 \
    template <>                                                                                                        \
    void Similarity::operator()(const type &a, const type &b)                                                          \
    {                                                                                                                  \
        if (Hasher{}(a) == Hasher{}(b))                                                                                \
        {                                                                                                              \
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

#define op_end                                                                                                         \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        L += Mass{}(a);                                                                                                \
        R += Mass{}(b);                                                                                                \
    }                                                                                                                  \
    }

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

Similarity::Similarity()
{}

double Similarity::run(const Unit &a, const Unit &b)
{
    R = 0;
    L = 0;
    S = 0;
    std::visit([this](auto a, auto b) { (*this)(*a, *b); }, a, b);
    return 2. * S / (2. * S + L + R);
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
op_call_opt(name_)
op_call(args_);
op_call(prefix_exp_);
op_end;

} // namespace sre::lua::ast
