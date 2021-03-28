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
    const std::multimap<int, Sequence> &sequence_clones() const;

  private:
    bool isMember(const Unit &a, const Unit &b, const Clones &clones) const;

  private:
    Clones clones_;
    SequenceClones sequence_clones_;
    std::vector<chunk> chunks_;

    // hash and subtree
    BucketList bucket_;
};
} // namespace sre::lua::ast
