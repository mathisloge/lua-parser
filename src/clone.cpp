#include "clone.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <boost/timer/timer.hpp>
#include "bucketer.hpp"
#include "hasher.hpp"
#include "mass.hpp"
#include "seq_builder.hpp"
#include "similarity.hpp"
#include "walker.hpp"
namespace sre::lua::ast
{

bool operator<(const BucketItem &x, const BucketItem &y)
{
    return std::get<0>(x) < std::get<0>(y);
}
static bool isEqual(const double sim_threshold, const BucketItem &x, const BucketItem &y)
{
    if (std::get<1>(x) == std::get<1>(y))
    {
        // im normalfall sollte ein gleicher hash auch die gleiche strukturelle gleichheit bringen.
        // da die hashes aber viel kombiniert werden muessen, kann es bei tiefer rekursion eher zu kollisionen kommen.
        // Daher pruefen wir hier nochmal explizit auf die aehnlichkeit.
        const auto sim = Similarity{}.run(std::get<2>(x), std::get<2>(y));
        return sim >= sim_threshold;
    }
    return false;
}

void Clone::addChunk(chunk &&chunkt)
{
    chunks_.emplace_back(std::forward<chunk>(chunkt));
}
void Clone::run(const chunk &chunk)
{
    using namespace boost::timer;

    cpu_timer timer_complete;

    BucketList buckets;
    {
        cpu_timer timer_bucket;
        Bucketer{buckets, 6}.toBuckets(chunk);
        std::cout << "bucket duration: " << timer_bucket.format();
    }

    std::cout << "buckets: " << buckets.size() << std::endl;
    if (buckets.size() < 2)
        return;
    {
        cpu_timer timer_sort;
        std::sort(buckets.begin(), buckets.end());
        std::cout << "bucket sort duration: " << timer_sort.format();
    }

    Clones clones;

    {
        cpu_timer timer_clones_step1;
        for (auto bit = buckets.begin(); bit != buckets.end(); bit++)
        {
            for (auto it = buckets.begin(); it != buckets.end(); it++)
            {
                if (bit == it)
                    continue;

                if (isEqual(1.0, *bit, *it))
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
                        for (const auto &subclone : s_c)
                        {
                            while (true)
                            {
                                auto f_it = std::find_if(clones.begin(), clones.end(), [&subclone](const auto &x) {
                                    return (subclone.first == x.first || subclone.first == x.second) &&
                                           (subclone.second == x.first || subclone.second == x.second);
                                });
                                if (f_it != clones.end())
                                    clones.erase(f_it);
                                else
                                    break;
                            }
                        }
                    }
                    clones.push_back(clone_pair);
                }
            }
        }
        std::cout << "Duration step1: " << timer_clones_step1.format();
    }

    if (true)
    {
        cpu_timer timer_clones_dups;
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
        std::cout << "removing duplicates: " << timer_clones_dups.format();
    }

    std::cout << "unique clones after step1: " << clones.size() << std::endl;
    {
        cpu_timer timer_clones_step2;
        const int min_len = 2;
        const int max_len = 6;
        std::vector<int> range(max_len);
        std::generate(range.begin(), range.end(), [n = min_len]() mutable { return n++; });

        const auto sequences = SeqBuilder{clones, min_len}(chunk).subsequences();
        for(const auto& s : sequences) {
            std::cout << s.size() << std::endl;
        }
        for (auto k : range)
        {
            std::vector<Unit> clone_pairs;

            clone_pairs.reserve(k);
        }
        std::cout << "Duration step2: " << timer_clones_step2.format();
    }
    clones_ = clones;

    std::cout << "Detection duration: " << timer_complete.format();
}

const Clones &Clone::clones() const
{
    return clones_;
}
} // namespace sre::lua::ast
