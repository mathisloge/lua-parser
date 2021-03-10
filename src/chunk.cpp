#include <boost/spirit/home/x3.hpp>
#include "chunk_def.hpp"
#include "config.hpp"

namespace sre::lua::parser::impl
{

BOOST_SPIRIT_INSTANTIATE(chunk_type, iterator_type, context_type);

}
