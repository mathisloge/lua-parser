#pragma once
#include <boost/spirit/home/x3.hpp>
#include "ast/ast.hpp"
namespace sre::lua::parser
{
namespace impl
{
    
namespace x3 = boost::spirit::x3;
struct chunk_class;
using chunk_type = x3::rule<chunk_class, ast::chunk>;
BOOST_SPIRIT_DECLARE(chunk_type)

} // namespace impl
impl::chunk_type chunk();
} // namespace sre::lua::parser
