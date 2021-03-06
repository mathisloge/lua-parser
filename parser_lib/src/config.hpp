#pragma once
#include <boost/spirit/home/x3.hpp>

namespace sre::lua::parser
{
namespace x3 = boost::spirit::x3;

using iterator_type = std::string::const_iterator;
using context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;
} // namespace sre::lua::parser
