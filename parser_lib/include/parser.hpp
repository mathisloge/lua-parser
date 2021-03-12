#pragma once
#include "ast/ast.hpp"
#include "luaparser_export.h"
namespace sre::lua
{
class LUAPARSER_EXPORT Parser
{
  public:
    bool parse(std::string::iterator &start, const std::string::iterator end, sre::lua::ast::chunk &chunk);
};
} // namespace sre::lua
