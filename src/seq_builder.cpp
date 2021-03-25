#include "seq_builder.hpp"
#include <iostream>
#include "hasher.hpp"
namespace sre::lua::ast
{

#define check_list(var)                                                                                                \
    Sequence subseq;                                                                                                   \
    for (const auto &c : clones_)                                                                                      \
    {                                                                                                                  \
        if (c.first.index() != c.second.index())                                                                       \
            continue;                                                                                                  \
                                                                                                                       \
        std::pair<bool, bool> clone_found{false, false};                                                               \
        for (const auto &s : var)                                                                                      \
        {                                                                                                              \
            const auto idx = is<std::remove_cv<std::remove_reference<decltype(s)>::type>::type>(s, c);                 \
            if (idx == 1)                                                                                              \
            {                                                                                                          \
                if (!clone_found.first)                                                                                \
                    clone_found.first = true;                                                                          \
                else                                                                                                   \
                    std::cout << "got same clone for first...?" << std::endl;                                          \
            }                                                                                                          \
            else if (idx == 2)                                                                                         \
            {                                                                                                          \
                if (!clone_found.second)                                                                               \
                    clone_found.second = true;                                                                         \
                else                                                                                                   \
                    std::cout << "got same clone for second...?" << std::endl;                                         \
            }                                                                                                          \
            if (clone_found.first && clone_found.second)                                                               \
                break;                                                                                                 \
        }                                                                                                              \
        if (clone_found.first && clone_found.second)                                                                   \
        {                                                                                                              \
            subseq.insert(std::pair<Unit, HashVal>{                                                                    \
                c.first, std::visit([](auto y) -> std::size_t { return Hasher{}(*y); }, c.first)});                    \
            subseq.insert(std::pair<Unit, HashVal>{                                                                    \
                c.second, std::visit([](auto y) -> std::size_t { return Hasher{}(*y); }, c.second)});                  \
        }                                                                                                              \
    }                                                                                                                  \
    if (subseq.size() > min_seq_len_)                                                                                  \
    {                                                                                                                  \
        clone_seq_.push_back(subseq);                                                                                  \
    }
#define call(var) (*this)(var)
#define call_list(var)                                                                                                 \
    for (const auto &v : var)                                                                                          \
        call(v);
#define call_visit(var) boost::apply_visitor(*this, var)

SeqBuilder::SeqBuilder(const Clones &clones, size_t min_seq_len)
    : clones_{clones}
    , min_seq_len_{min_seq_len}
{}

const AllSequences &SeqBuilder::subsequences() const
{
    return clone_seq_;
}

SeqBuilder &SeqBuilder::operator()(const chunk &ast)
{
    call(ast.block_);
    return *this;
}
void SeqBuilder::operator()(const block &block)
{
    check_list(block.stat_);
    call_list(block.stat_);
    call(block.retstat_);
}
void SeqBuilder::operator()(const stat &stat)
{
    call_visit(stat);
}
void SeqBuilder::operator()(const exp &exp)
{
    call_visit(exp);
}
void SeqBuilder::operator()(const explist &value)
{
    check_list(value);
    call_list(value);
}
void SeqBuilder::operator()(const prefixexp &value)
{
    call_visit(value);
}
void SeqBuilder::operator()(const expression &expression)
{
    // eigentlich koennen hier keine klon-paare entstehen. denn wenn klone vorhanden sind, werden die in diesem fall bis
    // zum stat oder explist subsumiert.
    call(expression.first_);
    call(expression.rest_);
}
void SeqBuilder::operator()(const primaryexpression &value)
{
    // eigentlich koennen hier keine klon-paare entstehen. denn wenn klone vorhanden sind, werden die in diesem fall bis
    // zum stat oder explist subsumiert.
    call(value.first_);
    call(value.rest_);
}
void SeqBuilder::operator()(const assign_or_call &value)
{
    // eigentlich koennen hier keine klon-paare entstehen. denn wenn klone vorhanden sind, werden die in diesem fall bis
    // zum stat oder explist subsumiert.
    call(value.primaryexp_);
    call(value.var_action_);
}
void SeqBuilder::operator()(const label &value)
{}
void SeqBuilder::operator()(const funcname &value)
{
    call(value.names_);
}
void SeqBuilder::operator()(const function &value)
{
    call(value.funcname_);
    call(value.funcbody_);
}
void SeqBuilder::operator()(const local_function &value)
{
    call(value.funcname_);
    call(value.funcbody_);
}
void SeqBuilder::operator()(const namelist &value)
{}
void SeqBuilder::operator()(const functiondef &value)
{
    call(value.funcbody_);
}
void SeqBuilder::operator()(const field &value)
{
    call(value.first);
    call(value.second);
}
void SeqBuilder::operator()(const fieldlist &value)
{
    check_list(value);
    call_list(value);
}
void SeqBuilder::operator()(const tableconstructor &value)
{
    call(value.first_);
    call(value.fields_);
}
void SeqBuilder::operator()(const for_namelist &value)
{
    call(value.name_list_);
    call(value.exp_list_);
    call(value.block_);
}
void SeqBuilder::operator()(const funcbody &value)
{
    call(value.parameters_);
    call(value.block_);
}
void SeqBuilder::operator()(const functioncall &value)
{
    call(value.args_);
    call(value.prefix_exp_);
}
void SeqBuilder::operator()(const args &value)
{
    call_visit(value);
}
void SeqBuilder::operator()(const var &value)
{
    call_visit(value);
}
void SeqBuilder::operator()(const var_wrapper &value)
{
    call(value.var_);
    call(value.next_);
}
void SeqBuilder::operator()(const varlist &value)
{
    call(value.explist_);
    check_list(value.rest_);
    call_list(value.rest_);
}
void SeqBuilder::operator()(const var_assign_or_list &value)
{
    call_visit(value);
}
void SeqBuilder::operator()(const whiledo &value)
{
    call(value.exp_);
    call(value.block_);
}
void SeqBuilder::operator()(const repeatuntil &value)
{
    call(value.exp_);
    call(value.block_);
}
void SeqBuilder::operator()(const doblock &value)
{
    call(value.block_);
}
void SeqBuilder::operator()(const forexp &value)
{
    call(value.name_);
    call(value.exp_first_);
    call(value.exp_second_);
    call(value.exp_third_);
    call(value.block_);
}
void SeqBuilder::operator()(const local_attnamelist_assign &value)
{
    call(value.attnamelist_);
    call(value.explist_);
}
void SeqBuilder::operator()(const attnamelist &value)
{
    check_list(value.rest_);
    call_list(value.rest_);
}
void SeqBuilder::operator()(const name_attrib_pair &value)
{}
void SeqBuilder::operator()(const ifelse &value)
{
    call(value.first_);
    check_list(value.rest_);
    call_list(value.rest_);
    if (value.else_.has_value())
        call(value.else_.value());
}
void SeqBuilder::operator()(const ifelse_wrapper &value)
{
    call(value.exp_);
    call(value.block_);
}
void SeqBuilder::operator()(const goto_stmt &value)
{}
void SeqBuilder::operator()(const unary &unary)
{}
void SeqBuilder::operator()(const binary &bin)
{}
void SeqBuilder::operator()(const keyword_stmt &)
{}
void SeqBuilder::operator()(const numeral &)
{}
void SeqBuilder::operator()(const Name &)
{}
void SeqBuilder::operator()(const std::string &)
{}
void SeqBuilder::operator()(const double &)
{}
void SeqBuilder::operator()(const unsigned &)
{}
void SeqBuilder::operator()(const bool &)
{}
void SeqBuilder::operator()(const optoken &)
{}
void SeqBuilder::operator()(const nil &)
{}
} // namespace sre::lua::ast
