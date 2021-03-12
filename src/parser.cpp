#include "parser.hpp"
#include "ast/ast_adapted.hpp"
#include "chunk_def.hpp"
namespace sre::lua
{
bool Parser::parse(std::string::iterator &start, const std::string::iterator end, sre::lua::ast::chunk &chunk)
{
    return boost::spirit::x3::phrase_parse(
        start, end, sre::lua::parser::chunk(), sre::lua::parser::impl::input_skipper, chunk);
}
} // namespace sre::lua
