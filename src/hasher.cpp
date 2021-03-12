#include "hasher.hpp"
#include <functional>
#include <boost/container_hash/hash.hpp>
namespace sre::lua::ast
{
struct nil_exception : std::exception
{
    const char *what() const noexcept override
    {
        return "exp is nil";
    }
};
// clang-format off
#define GET_EXP_HASH(var, exp) \
    try { boost::hash_detail::hash_combine_impl(var, Hasher{}(exp)); } catch (...) {}

#define GET_EXP_RAW_HASH(var, exp) \
    std::size_t var = 0; try { var = Hasher{}(exp); } catch (...) {}
// clang-format on

std::size_t Hasher::operator()(const chunk &ast) const
{
    auto hash = std::hash<std::string>{}("chunk");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(ast.block_));
    return hash;
}
std::size_t Hasher::operator()(const block &block) const
{
    auto hash = std::hash<std::string>{}("block");
    for (const auto &stat : block.stat_)
        boost::hash_detail::hash_combine_impl(hash, Hasher{}(stat));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(block.retstat_));
    return hash;
}
std::size_t Hasher::operator()(const stat &stat) const
{
    auto hash = Hasher{}("stat");
    boost::hash_detail::hash_combine_impl(hash, boost::apply_visitor(Hasher{}, stat));
    return hash;
}
std::size_t Hasher::operator()(const exp &exp) const
{
    return boost::apply_visitor(Hasher{}, exp);
}
std::size_t Hasher::operator()(const explist &value) const
{
    auto hash = std::hash<std::string>{}("explist");
    for (const auto &exp : value)
    {
        GET_EXP_HASH(hash, exp)
    }
}
std::size_t Hasher::operator()(const prefixexp &value) const
{
    auto hash = std::hash<std::string>{}("prefixexp");
    boost::hash_detail::hash_combine_impl(hash, boost::apply_visitor(Hasher{}, value));
    return hash;
}
std::size_t Hasher::operator()(const expression &expression) const
{
    GET_EXP_RAW_HASH(hash, expression.first_)
    GET_EXP_HASH(hash, expression.rest_)
    return hash;
}
std::size_t Hasher::operator()(const primaryexpression &value) const
{
    GET_EXP_RAW_HASH(hash, value.first_)
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.rest_));
    return hash;
}
std::size_t Hasher::operator()(const assign_or_call &value) const
{
    auto hash = Hasher{}(value.primaryexp_);
    boost::hash_detail::hash_combine_impl(hash, boost::apply_visitor(Hasher{}, value.var_action_));
    return hash;
}
std::size_t Hasher::operator()(const label &value) const
{
    auto hash = std::hash<std::string>{}("label");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.name_));
    return hash;
}
std::size_t Hasher::operator()(const funcname &value) const
{
    auto hash = std::hash<std::string>{}("funcname");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.names_));
    if (!value.self_chain_.name.empty())
        boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.self_chain_));
    return hash;
}
std::size_t Hasher::operator()(const function &value) const
{
    auto hash = std::hash<std::string>{}("function");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.funcname_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.funcbody_));
}
std::size_t Hasher::operator()(const local_function &value) const
{
    auto hash = std::hash<std::string>{}("local_function");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.funcname_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.funcbody_));
}
std::size_t Hasher::operator()(const namelist &value) const
{
    auto hash = std::hash<std::string>{}("namelist");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.name_));
    for (const auto &name : value.chain_)
        boost::hash_detail::hash_combine_impl(hash, Hasher{}(name));
    return hash;
}
std::size_t Hasher::operator()(const funcbody &value) const
{
    auto hash = std::hash<std::string>{}("funcbody");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.parameters_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.block_));
    return hash;
}
std::size_t Hasher::operator()(const functiondef &value) const
{
    auto hash = std::hash<std::string>{}("functiondef");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.funcbody_));
    return hash;
}
std::size_t Hasher::operator()(const field &value) const
{
    auto hash = std::hash<std::string>{}("field");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.first));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.second));
    return hash;
}
std::size_t Hasher::operator()(const fieldlist &value) const
{
    auto hash = std::hash<std::string>{}("fieldlist");
    for (const auto &field : value)
        boost::hash_detail::hash_combine_impl(hash, Hasher{}(field));
    return hash;
}
std::size_t Hasher::operator()(const tableconstructor &value) const
{
    auto hash = std::hash<std::string>{}("tableconstructor");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.first_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.fields_));
    return hash;
}
std::size_t Hasher::operator()(const for_namelist &value) const
{
    auto hash = std::hash<std::string>{}("fornamelist");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.name_list_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.exp_list_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.block_));
}
std::size_t Hasher::operator()(const functioncall &value) const
{
    auto hash = std::hash<std::string>{}("functioncall");
    if (value.name_.has_value())
        boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.name_.value()));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.args_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.prefix_exp_));
    return hash;
}
std::size_t Hasher::operator()(const args &value) const
{
    return boost::apply_visitor(Hasher{}, value);
}
std::size_t Hasher::operator()(const var &value) const
{
    return boost::apply_visitor(Hasher{}, value);
}
std::size_t Hasher::operator()(const var_wrapper &value) const
{
    auto hash = Hasher{}(value.var_);
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.next_));
    return hash;
}
std::size_t Hasher::operator()(const varlist &value) const
{
    auto hash = std::hash<std::string>{}("varlist");
    for (const auto &var : value.rest_)
        boost::hash_detail::hash_combine_impl(hash, Hasher{}(var));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.explist_));
    return hash;
}
std::size_t Hasher::operator()(const whiledo &value) const
{
    auto hash = std::hash<std::string>{}("while");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.exp_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.block_));
    return hash;
}
std::size_t Hasher::operator()(const repeatuntil &value) const
{
    auto hash = std::hash<std::string>{}("repeat");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.block_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.exp_));
    return hash;
}
std::size_t Hasher::operator()(const doblock &value) const
{
    auto hash = std::hash<std::string>{}("do");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.block_));
    return hash;
}
std::size_t Hasher::operator()(const forexp &value) const
{
    auto hash = std::hash<std::string>{}("for");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.name_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.exp_first_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.exp_second_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.exp_third_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.block_));
    return hash;
}
std::size_t Hasher::operator()(const local_attnamelist_assign &value) const
{
    auto hash = std::hash<std::string>{}("assign");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.attnamelist_));
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.explist_));
    return hash;
}
std::size_t Hasher::operator()(const attnamelist &value) const
{
    auto hash = std::hash<std::string>{}("attnamelist");
    boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.first_));
    for (const auto &more : value.rest_)
    {
        boost::hash_detail::hash_combine_impl(hash, Hasher{}(more));
    }
    return hash;
}
std::size_t Hasher::operator()(const name_attrib_pair &value) const
{
    auto hash = Hasher{}(value.name_);
    if (value.attrib_.has_value())
    {
        boost::hash_detail::hash_combine_impl(hash, Hasher{}(value.attrib_.value()));
    }
    return hash;
}
std::size_t Hasher::operator()(const ifelse &value) const
{
    auto hash_if = Hasher{}(value.first_);
    for (const auto &elif : value.rest_)
    {
        boost::hash_detail::hash_combine_impl(hash_if, Hasher{}(elif));
    }
    if (value.else_.has_value())
    {
        boost::hash_detail::hash_combine_impl(hash_if, Hasher{}(value.else_.value()));
    }
    return hash_if;
}
std::size_t Hasher::operator()(const ifelse_wrapper &value) const
{
    auto hash_if = std::hash<std::string>{}("if");
    boost::hash_detail::hash_combine_impl(hash_if, Hasher{}(value.exp_));
    boost::hash_detail::hash_combine_impl(hash_if, Hasher{}(value.block_));
    return hash_if;
}
std::size_t Hasher::operator()(const goto_stmt &value) const
{
    auto hash_goto = std::hash<std::string>{}("goto");
    boost::hash_detail::hash_combine_impl(hash_goto, Hasher{}(value.name_));
    return hash_goto;
}
std::size_t Hasher::operator()(const unary &unary) const
{
    auto hash_op = std::hash<optoken>{}(unary.operator_);
    boost::hash_detail::hash_combine_impl(hash_op, Hasher{}(unary.rhs_));
    return hash_op;
}
std::size_t Hasher::operator()(const binary &bin) const
{
    auto hash_op = std::hash<optoken>{}(bin.operator_);
    boost::hash_detail::hash_combine_impl(hash_op, Hasher{}(bin.rhs_));
    return hash_op;
}
std::size_t Hasher::operator()(const keyword_stmt &value) const
{
    return std::hash<ast::keyword>{}(value.keyword_); //! value.keyword_ maps directly to the underlying keyword.
}
std::size_t Hasher::operator()(const numeral &value) const
{
    return boost::apply_visitor(Hasher{}, value); // numeral is just a rule so don't gen a hash for this
}
std::size_t Hasher::operator()(const Name &value) const
{
    return std::hash<std::string>{}("Name");
}
std::size_t Hasher::operator()(const std::string &value) const
{
    return std::hash<std::string>{}("string");
}
std::size_t Hasher::operator()(const double &value) const
{
    return std::hash<std::string>{}("double");
}
std::size_t Hasher::operator()(const unsigned &value) const
{
    return std::hash<std::string>{}("unsigned");
}
std::size_t Hasher::operator()(const bool &value) const
{
    return std::hash<std::string>{}("bool");
}
std::size_t Hasher::operator()(const nil &nil) const
{
    throw nil_exception{};
}
} // namespace sre::lua::ast
