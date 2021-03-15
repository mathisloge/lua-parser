#pragma once
#include <functional>
#include <variant>
#include "ast/ast.hpp"
#include "bucketer.hpp"
namespace sre::lua::ast
{

class Clone final
{
    using ClonePair = std::pair<Unit, Unit>;

  public:
    void addChunk(chunk &&chunk);
    void run(const chunk &chunk);

  private:
    bool isMember(const Unit &a, const Unit &b, const std::vector<ClonePair> &clones) const;

  private:
    std::vector<chunk> chunks_;
    // hash and subtree
    BucketList bucket_;
};
} // namespace sre::lua::ast
