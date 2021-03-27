#pragma once
#include <functional>
#include <variant>
#include "bucketer.hpp"
#include "types.hpp"
namespace sre::lua::ast
{

class Clone final
{
  public:
    void addChunk(chunk &&chunk);
    void run(const chunk &chunk);
    const Clones &clones() const;
    const std::map<int, Sequence>& sequence_clones() const;

  private:
    bool isMember(const Unit &a, const Unit &b, const Clones &clones) const;

  private:
    Clones clones_;
    std::vector<chunk> chunks_;
    std::map<int, Sequence> sequence_clones_;
    // hash and subtree
    BucketList bucket_;
    
};
} // namespace sre::lua::ast
