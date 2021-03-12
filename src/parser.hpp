#pragma once
#include "ast/ast.hpp"
namespace sre::lua
{
class Parser
{
  public:
    bool parse(std::string::iterator &start, const std::string::iterator end, sre::lua::ast::chunk &chunk);
};
} // namespace sre::lua
