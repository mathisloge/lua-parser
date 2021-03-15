#include "bucketer.hpp"
#include "hasher.hpp"
#include "mass.hpp"

namespace sre::lua::ast
{

#define CHECK(var)                                                                                                     \
    const auto mass = Mass{}(var);                                                                                     \
    if (mass > min_mass_)                                                                                              \
    {                                                                                                                  \
        bucket_.emplace_back(mass, Hasher{}(var), Unit{&var});

#define ENDCHECK }

#define BUCK(var) Bucketer{bucket_, min_mass_}(var);
#define BUCK_OP(var)                                                                                                   \
    if (var.has_value())                                                                                               \
        Bucketer{bucket_, min_mass_}(var.value());
#define VISIT(var) boost::apply_visitor(Bucketer{bucket_, min_mass_}, var);

#define BUCK_LIST(var)                                                                                                 \
    for (const auto &v : var)                                                                                          \
    {
#define BUCK_LIST_END }

Bucketer::Bucketer(BucketList &bucket, std::size_t min_mass)
    : bucket_{bucket}
    , min_mass_{min_mass}
{}
void Bucketer::toBuckets(const chunk &chunkt)
{
    BUCK(chunkt)
}

void Bucketer::operator()(const chunk &ast)
{
    CHECK(ast)
    BUCK(ast.block_)
    ENDCHECK
}
void Bucketer::operator()(const block &block)
{
    CHECK(block)

    BUCK_LIST(block.stat_)
    BUCK(v)
    BUCK_LIST_END

    BUCK(block.retstat_)

    ENDCHECK
}
void Bucketer::operator()(const stat &stat)
{
    CHECK(stat)
    VISIT(stat)
    ENDCHECK
}
void Bucketer::operator()(const exp &exp)
{
    CHECK(exp)
    VISIT(exp)
    ENDCHECK
}
void Bucketer::operator()(const explist &value)
{
    CHECK(value)
    BUCK_LIST(value)
    BUCK(v)
    BUCK_LIST_END
    ENDCHECK
}
void Bucketer::operator()(const prefixexp &value)
{
    CHECK(value)
    VISIT(value)
    ENDCHECK
}
void Bucketer::operator()(const expression &expression)
{
    BUCK(expression.first_)
    BUCK(expression.rest_)
}
void Bucketer::operator()(const primaryexpression &value)
{
    BUCK(value.first_)
    BUCK(value.rest_)
}
void Bucketer::operator()(const assign_or_call &value)
{
    BUCK(value.primaryexp_)
    BUCK(value.var_action_)
}
void Bucketer::operator()(const label &value)
{
    CHECK(value)
    BUCK(value.name_)
    ENDCHECK
}
void Bucketer::operator()(const funcname &value)
{
    CHECK(value)
    BUCK(value.names_)
    BUCK(value.self_chain_)
    ENDCHECK
}
void Bucketer::operator()(const function &value)
{
    CHECK(value)
    BUCK(value.funcname_)
    BUCK(value.funcbody_)
    ENDCHECK
}
void Bucketer::operator()(const local_function &value)
{
    CHECK(value)
    BUCK(value.funcname_)
    BUCK(value.funcbody_)
    ENDCHECK
}
void Bucketer::operator()(const namelist &value)
{
    CHECK(value)
    BUCK(value.name_)
    BUCK_LIST(value.chain_)
    BUCK(v)
    BUCK_LIST_END
    ENDCHECK
}
void Bucketer::operator()(const functiondef &value)
{
    CHECK(value)
    BUCK(value.funcbody_)
    ENDCHECK
}
void Bucketer::operator()(const field &value)
{
    CHECK(value)
    BUCK(value.first)
    BUCK(value.second)
    ENDCHECK
}
void Bucketer::operator()(const fieldlist &value)
{
    CHECK(value)
    BUCK_LIST(value)
    BUCK(v)
    BUCK_LIST_END
    ENDCHECK
}
void Bucketer::operator()(const tableconstructor &value)
{
    CHECK(value)
    BUCK(value.first_)
    BUCK(value.fields_)
    ENDCHECK
}
void Bucketer::operator()(const for_namelist &value)
{
    CHECK(value)
    BUCK(value.name_list_)
    BUCK(value.exp_list_)
    BUCK(value.block_)
    ENDCHECK
}
void Bucketer::operator()(const funcbody &value)
{
    CHECK(value)
    BUCK(value.parameters_)
    BUCK(value.block_)
    ENDCHECK
}
void Bucketer::operator()(const functioncall &value)
{
    CHECK(value)
    BUCK_OP(value.name_)
    BUCK(value.args_)
    BUCK(value.prefix_exp_)
    ENDCHECK
}
void Bucketer::operator()(const args &value)
{
    CHECK(value)
    VISIT(value)
    ENDCHECK
}
void Bucketer::operator()(const var &value)
{
    CHECK(value)
    VISIT(value)
    ENDCHECK
}
void Bucketer::operator()(const var_wrapper &value)
{
    BUCK(value.var_)
    BUCK(value.next_)
}
void Bucketer::operator()(const varlist &value)
{
    CHECK(value)
    BUCK(value.explist_)
    BUCK_LIST(value.rest_)
    BUCK(v)
    BUCK_LIST_END
    ENDCHECK
}
void Bucketer::operator()(const var_assign_or_list &value)
{
    VISIT(value)
}
void Bucketer::operator()(const whiledo &value)
{
    CHECK(value)
    BUCK(value.exp_)
    BUCK(value.block_)
    ENDCHECK
}
void Bucketer::operator()(const repeatuntil &value)
{
    CHECK(value)
    BUCK(value.block_)
    BUCK(value.exp_)
    ENDCHECK
}
void Bucketer::operator()(const doblock &value)
{
    CHECK(value)
    BUCK(value.block_)
    ENDCHECK
}
void Bucketer::operator()(const forexp &value)
{
    CHECK(value)
    BUCK(value.name_)
    BUCK(value.exp_first_)
    BUCK(value.exp_second_)
    BUCK(value.exp_third_)
    BUCK(value.block_)
    ENDCHECK
}
void Bucketer::operator()(const local_attnamelist_assign &value)
{
    CHECK(value)
    BUCK(value.attnamelist_)
    BUCK(value.explist_)
    ENDCHECK
}
void Bucketer::operator()(const attnamelist &value)
{
    CHECK(value)
    BUCK(value.first_)
    BUCK_LIST(value.rest_)
    BUCK(v)
    BUCK_LIST_END
    ENDCHECK
}
void Bucketer::operator()(const name_attrib_pair &value)
{
    BUCK(value.name_)
    BUCK_OP(value.attrib_)
}
void Bucketer::operator()(const ifelse &value)
{
    CHECK(value)
    BUCK(value.first_)
    BUCK_LIST(value.rest_)
    BUCK(v)
    BUCK_LIST_END
    BUCK_OP(value.else_)
    ENDCHECK
}
void Bucketer::operator()(const ifelse_wrapper &value)
{
    CHECK(value)
    BUCK(value.exp_)
    BUCK(value.block_)
    ENDCHECK
}
void Bucketer::operator()(const goto_stmt &value)
{
    CHECK(value)
    BUCK(value.name_)
    ENDCHECK
}
void Bucketer::operator()(const unary &unary)
{
    CHECK(unary)
    BUCK(unary.operator_)
    BUCK(unary.rhs_)
    ENDCHECK
}
void Bucketer::operator()(const binary &bin)
{
    CHECK(bin)
    BUCK(bin.operator_)
    BUCK(bin.rhs_)
    ENDCHECK
}
void Bucketer::operator()(const keyword_stmt &value)
{
    CHECK(value)
    ENDCHECK
}
void Bucketer::operator()(const numeral &value)
{
    VISIT(value)
}
void Bucketer::operator()(const Name &value)
{}
void Bucketer::operator()(const std::string &value)
{}
void Bucketer::operator()(const double &value)
{}
void Bucketer::operator()(const unsigned &value)
{}
void Bucketer::operator()(const bool &value)
{}
void Bucketer::operator()(const optoken &op)
{}
void Bucketer::operator()(const nil &nil)
{}
} // namespace sre::lua::ast
