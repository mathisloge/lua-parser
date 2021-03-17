#include "clone.hpp"
#include <algorithm>
#include <iostream>
#include <set>
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

    Clones clones;
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

    {
        Clones unique_clones;
        for (const auto &c : clones)
        {
            auto f_it = std::find_if(unique_clones.begin(), unique_clones.end(), [&c](const auto &x) {
                return (c.first == x.first || c.first == x.second) && (c.second == x.first || c.second == x.second);
            });
            if (f_it == unique_clones.end())
            {
                unique_clones.emplace_back(c);
            }
        }
        clones = unique_clones;
    }

    std::cout << "Clones: " << clones.size() << std::endl;

    std::cout << "========= STEP2 ===========" << std::endl;
    const int min_len = 2;
    const int max_len = 6;
    std::vector<int> range(max_len);
    std::generate(range.begin(), range.end(), [n = min_len]() mutable { return n++; });

    auto genSubsequences = [](int k, const Clones &clones) {
        std::vector<std::vector<Unit>> buckets;


        std::vector<Unit> clone_pairs;
        clone_pairs.reserve(k);

        for (const auto &c : clones)
        {

        }

        return buckets;
    };
    for (auto k : range)
    {
        std::vector<Unit> clone_pairs;

        clone_pairs.reserve(k);
    }

    clones_ = clones;

#if 0
    std::cout << "============" << std::endl;
    do
        std::cout << "cmp hash " << std::get<1>(buckets[0]) << " == " << std::get<1>(buckets[1]) << std::endl;
    while (std::next_permutation(buckets.begin(), buckets.end()));
#endif
}

const Clones &Clone::clones() const
{
    return clones_;
}
} // namespace sre::lua::ast
