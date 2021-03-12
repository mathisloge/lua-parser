#include "printer.hpp"
namespace sre::lua::ast
{
rexpr_printer::rexpr_printer(int indent)
    : indent(indent)
{}

void rexpr_printer::operator()(const chunk &ast) const
{
    std::cout << '{' << std::endl;
    tab(indent);
    std::cout << "exp chunk = " << std::endl;
    rexpr_printer{indent + tabsize}(ast.block_);
    std::cout << std::endl << '}' << std::endl;
}

void rexpr_printer::operator()(ast::block const &block) const
{
    tab(indent);
    std::cout << "block stat = " << std::endl;
    for (auto const &stat : block.stat_)
    {
        rexpr_printer{indent + tabsize}(stat);
    }
    tab(indent);
    if (block.retstat_.size() > 0)
        std::cout << "block retstat = " << std::endl;
    for (auto const &retstat : block.retstat_)
    {
        rexpr_printer(indent + tabsize)(retstat);
    }
}

void rexpr_printer::operator()(const namelist &value) const
{
    tab(indent);
    std::cout << "namelist = " << std::endl;
    rexpr_printer{indent + tabsize}(value.name_);

    for (const auto &name : value.chain_)
    {
        tab(indent);
        rexpr_printer{indent + tabsize}(name);
    }
}

void rexpr_printer::operator()(ast::stat const &stat) const
{
    tab(indent);
    std::cout << "stat = " << std::endl;
    boost::apply_visitor(rexpr_printer(indent + tabsize), stat);
}

void rexpr_printer::operator()(ast::exp const &exp) const
{
    tab(indent);
    std::cout << "exp" << std::endl;
    boost::apply_visitor(rexpr_printer(indent + tabsize), exp);
}

void rexpr_printer::operator()(std::string const &text) const
{
    tab(indent);
    std::cout << "string \"" << text << '"' << std::endl;
}

void rexpr_printer::operator()(double const &text) const
{
    tab(indent);
    std::cout << "double \"" << text << '"' << std::endl;
}

void rexpr_printer::operator()(unsigned const &text) const
{
    tab(indent);
    std::cout << "unsigned \"" << text << '"' << std::endl;
}
void rexpr_printer::operator()(bool const &text) const
{
    tab(indent);
    std::cout << "bool \"" << text << '"' << std::endl;
}

void rexpr_printer::operator()(const unary &unary) const
{
    const auto symbol = kOptokenSymbols.find(unary.operator_);
    tab(indent);
    std::cout << "UNARY " << symbol->second << std::endl;
    boost::apply_visitor(rexpr_printer(indent + tabsize), unary.rhs_);
}

void rexpr_printer::operator()(const binary &bin) const
{
    const auto symbol = kOptokenSymbols.find(bin.operator_);
    tab(indent);
    std::cout << "binary " << symbol->second << std::endl;
    boost::apply_visitor(rexpr_printer(indent + tabsize), bin.rhs_);
}

void rexpr_printer::operator()(const expression &expression) const
{
    tab(indent);
    std::cout << "expression " << std::endl;
    rexpr_printer{indent + tabsize}(expression.first_);
    rexpr_printer{indent + tabsize}(expression.rest_);
}

void rexpr_printer::operator()(const label &value) const
{
    tab(indent);
    std::cout << "label=" << value << std::endl;
}

void rexpr_printer::operator()(const Name &value) const
{
    tab(indent);
    std::cout << "name=" << value.name << std::endl;
}

void rexpr_printer::operator()(const funcname &value) const
{
    tab(indent);
    std::cout << "funcname=" << std::endl;
    rexpr_printer{indent + 2 * tabsize}(value.names_);
    if (!value.self_chain_.name.empty())
    {
        tab(indent);
        std::cout << "self_chain=" << std::endl;
        rexpr_printer{indent + tabsize}(value.self_chain_);
    }
}
void rexpr_printer::operator()(const funcbody &value) const
{
    tab(indent);
    std::cout << "params=" << std::endl;
    rexpr_printer{indent + 2 * tabsize}(value.parameters_);
    std::cout << "block=" << std::endl;
    rexpr_printer{indent + tabsize}(value.block_);
}
void rexpr_printer::operator()(const function &value) const
{
    tab(indent);
    std::cout << "function=" << std::endl;
    rexpr_printer{indent + tabsize}(value.funcname_);
    rexpr_printer{indent + tabsize}(value.funcbody_);
}

void rexpr_printer::operator()(const local_function &value) const
{
    tab(indent);
    std::cout << "local_function=" << std::endl;
    rexpr_printer{indent + tabsize}(value.funcname_);
    rexpr_printer{indent + tabsize}(value.funcbody_);
}

void rexpr_printer::operator()(const functiondef &value) const
{
    tab(indent);
    std::cout << "functiondef=" << std::endl;
    rexpr_printer{indent + tabsize}(value.funcbody_);
}

void rexpr_printer::operator()(const field &value) const
{
    tab(indent);
    rexpr_printer{indent + tabsize}(value.first);
    rexpr_printer{indent + tabsize}(value.second);
}

void rexpr_printer::operator()(const fieldlist &value) const
{
    tab(indent);
    for (const auto &f : value)
    {
        rexpr_printer{indent + tabsize}(f);
    }
}

void rexpr_printer::operator()(const tableconstructor &value) const
{
    tab(indent);
    std::cout << "tableconstructor = " << std::endl;
    rexpr_printer{indent + tabsize}(value.first_);
    rexpr_printer{indent + tabsize}(value.fields_);
}

void rexpr_printer::operator()(const for_namelist &value) const
{
    tab(indent);
    std::cout << "for_namelist = " << std::endl;
    rexpr_printer{indent + 2 * tabsize}(value.name_list_);
    rexpr_printer{indent + tabsize}(value.exp_list_);
    rexpr_printer{indent + tabsize}(value.block_);
}

void rexpr_printer::operator()(const prefixexp &value) const
{
    tab(indent);
    std::cout << "prefixexp =" << std::endl;
    boost::apply_visitor(rexpr_printer(indent + tabsize), value);
}

void rexpr_printer::operator()(const functioncall &value) const
{
    tab(indent);
    std::cout << "functioncall=" << std::endl;
    if (value.name_.has_value())
        rexpr_printer{indent + tabsize}(value.name_.value());
    boost::apply_visitor(rexpr_printer(indent + tabsize), value.args_);
    boost::apply_visitor(rexpr_printer(indent + tabsize), value.prefix_exp_);
}
void rexpr_printer::operator()(const explist &value) const
{
    tab(indent);
    std::cout << "explist=" << std::endl;
    for (const auto &expr : value)
    {
        rexpr_printer{indent + tabsize}(expr);
    }
}

void rexpr_printer::operator()(const var &value) const
{
    tab(indent);
    std::cout << "var =" << std::endl;
    boost::apply_visitor(rexpr_printer(indent + tabsize), value);
}

void rexpr_printer::operator()(const var_wrapper &value) const
{
    tab(indent);
    std::cout << "var_wrapper =" << std::endl;
    rexpr_printer{indent + tabsize}(value.var_);
    rexpr_printer{indent + tabsize}(value.next_);
}

void rexpr_printer::operator()(const varlist &value) const
{
    tab(indent);
    std::cout << "varlist =" << std::endl;
    for (auto &var : value.rest_)
    {
        rexpr_printer{indent + tabsize}(var);
    }
    rexpr_printer{indent + tabsize}(value.explist_);
}

void rexpr_printer::operator()(const primaryexpression &value) const
{
    tab(indent);
    std::cout << "primaryexpression=" << std::endl;
    rexpr_printer{indent + tabsize}(value.first_);
    boost::apply_visitor(rexpr_printer(indent + tabsize), value.rest_);
}
void rexpr_printer::operator()(const assign_or_call &value) const
{
    tab(indent);
    std::cout << "assign_or_call=" << std::endl;
    rexpr_printer{indent + tabsize}(value.primaryexp_);
    boost::apply_visitor(rexpr_printer(indent + tabsize), value.var_action_);
}

void rexpr_printer::operator()(const ifelse &value) const
{
    tab(indent);
    std::cout << "if=" << std::endl;
    rexpr_printer{indent + tabsize}(value.first_);
    for (const auto &v : value.rest_)
    {
        std::cout << "elseif=" << std::endl;
        rexpr_printer{indent + tabsize}(v);
    }
    if (value.else_.has_value())
    {
        std::cout << "else=" << std::endl;
        rexpr_printer{indent + tabsize}(value.else_.value());
    }
}
void rexpr_printer::operator()(const whiledo &value) const
{
    tab(indent);
    std::cout << "whiledo=" << std::endl;
    rexpr_printer{indent + tabsize}(value.exp_);
    rexpr_printer{indent + tabsize}(value.block_);
}
void rexpr_printer::operator()(const repeatuntil &value) const
{
    tab(indent);
    std::cout << "repeatuntil=" << std::endl;
    rexpr_printer{indent + tabsize}(value.block_);
    rexpr_printer{indent + tabsize}(value.exp_);
}
void rexpr_printer::operator()(const doblock &value) const
{
    tab(indent);
    std::cout << "doblock=" << std::endl;
    rexpr_printer{indent + tabsize}(value.block_);
}

void rexpr_printer::operator()(const forexp &value) const
{
    tab(indent);
    std::cout << "forexp=" << std::endl;
    rexpr_printer{indent + tabsize}(value.name_);
    rexpr_printer{indent + tabsize}(value.exp_first_);
    rexpr_printer{indent + tabsize}(value.exp_second_);
    rexpr_printer{indent + tabsize}(value.exp_third_);
    rexpr_printer{indent + tabsize}(value.block_);
}

void rexpr_printer::operator()(const local_attnamelist_assign &value) const
{
    tab(indent);
    std::cout << "local_attnamelist_assign=" << std::endl;
    rexpr_printer{indent + tabsize}(value.attnamelist_);
    rexpr_printer{indent + tabsize}(value.explist_);
}
void rexpr_printer::operator()(const attnamelist &value) const
{
    tab(indent);
    std::cout << "attnamelist=" << std::endl;
    rexpr_printer{indent + tabsize}(value.first_);
    for (const auto &v : value.rest_)
        rexpr_printer{indent + tabsize}(v);
}

void rexpr_printer::operator()(const name_attrib_pair &value) const
{
    tab(indent);
    std::cout << "name_attrib_pair=" << std::endl;
    rexpr_printer{indent + tabsize}(value.name_);

    if (value.attrib_.has_value())
    {
        std::cout << "attrib=" << std::endl;
        rexpr_printer{indent + tabsize}(value.attrib_.value());
    }
}

void rexpr_printer::operator()(const ifelse_wrapper &value) const
{
    tab(indent);
    std::cout << "ifelse_wrapper=" << std::endl;
    rexpr_printer{indent + tabsize}(value.exp_);
    rexpr_printer{indent + tabsize}(value.block_);
}

void rexpr_printer::operator()(const nil &nil) const
{
    tab(indent);
    std::cout << "nil " << std::endl;
}

void rexpr_printer::operator()(const goto_stmt &value) const
{
    tab(indent);
    std::cout << "goto=" << value.name_ << std::endl;
}

void rexpr_printer::operator()(const keyword_stmt &value) const
{
    tab(indent);
    std::cout << "keyword=" << value.keyword_ << std::endl;
}

void rexpr_printer::operator()(const numeral &value) const
{
    tab(indent);
    boost::apply_visitor(rexpr_printer(indent + tabsize), value);
}

void rexpr_printer::tab(int spaces) const
{
    for (int i = 0; i < spaces; ++i)
        std::cout << ' ';
}

} // namespace sre::lua::ast
