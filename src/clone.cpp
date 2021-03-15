#include "clone.hpp"
#include <algorithm>
#include <iostream>
#include "bucketer.hpp"
#include "hasher.hpp"
#include "mass.hpp"
#include "walker.hpp"
namespace sre::lua::ast
{

bool operator<(const BucketItem &x, const BucketItem &y)
{
    return std::get<0>(x) < std::get<0>(y);
}
static bool isEqual(const BucketItem &x, const BucketItem &y)
{
    return std::get<1>(x) == std::get<1>(y);
}

bool Clone::isMember(const Unit &a, const Unit &b, const std::vector<ClonePair> &clones) const
{
    auto it = std::find_if(clones.begin(), clones.end(), [&a, &b](const ClonePair &clone_pair) {
        return clone_pair.first == a && clone_pair.second == b;
    });
    return it != clones.end();
}

void Clone::addChunk(chunk &&chunkt)
{
    chunks_.emplace_back(std::forward<chunk>(chunkt));
}
void Clone::run(const chunk &chunk)
{
    BucketList buckets;
    Bucketer{buckets, 4}.toBuckets(chunk);

    std::cout << "bucket size: " << buckets.size() << std::endl;
    if (buckets.size() < 2)
        return;

    std::sort(buckets.begin(), buckets.end());

    std::vector<ClonePair> clones;
    for (auto bit = buckets.begin(); bit != buckets.end(); bit++)
    {
        for (auto it = buckets.begin(); it != buckets.end(); it++)
        {
            if (bit == it)
                continue;

            if (isEqual(*bit, *it))
            {
                auto clone_pair = std::make_pair(std::get<2>(*bit), std::get<2>(*it));
                auto f_c = std::visit(
                    [&clones](const auto u) {
                        Walker w{clones};
                        w(*u);
                        return w.clones();
                    },
                    clone_pair.first);
                auto s_c = std::visit(
                    [&clones, &f_c](const auto u) {
                        Walker w{clones, f_c};
                        w(*u);
                        return w.clones();
                    },
                    clone_pair.second);

                if (s_c.size() > 0)
                {
                    clones.erase(std::remove_if(clones.begin(),
                                                clones.end(),
                                                [&s_c](const auto &p) {
                                                    return std::find(s_c.begin(), s_c.end(), p) != s_c.end();
                                                }),
                                 clones.end());
                }

                clones.push_back(clone_pair);
            }
        }
    }
    std::cout << "Clones: " << clones.size() << std::endl;
#if 0
    std::cout << "============" << std::endl;
    do
        std::cout << "cmp hash " << std::get<1>(buckets[0]) << " == " << std::get<1>(buckets[1]) << std::endl;
    while (std::next_permutation(buckets.begin(), buckets.end()));
#endif
}
} // namespace sre::lua::ast
