#include "chunk_def.hpp"
#include "chunk.hpp"
namespace sre::lua::parser
{
impl::chunk_type chunk()
{
    impl::initBinOp();
    impl::initUnaryOp();
    impl::initKeywords();
    return impl::chunk_expr;
}
} // namespace sre::lua::parser
