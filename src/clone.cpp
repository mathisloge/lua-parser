#include "clone.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <boost/container_hash/hash.hpp>
#include <boost/timer/timer.hpp>
#include "bucketer.hpp"
#include "hasher.hpp"
#include "mass.hpp"
#include "runtime_config.hpp"
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
        // Daher pruefen wir hier nochmal explizit auf die gleichheit.
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
        Bucketer{buckets, static_cast<std::size_t>(RuntimeConfig::min_mass)}.toBuckets(chunk);
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

    if constexpr (true)
    {
        cpu_timer timer_clones_step2;
        const int min_len = RuntimeConfig::min_seq;
        // Build the list structures describing sequences
        const auto sequences = SeqBuilder{clones, static_cast<std::size_t>(min_len)}(chunk).subsequences();
        int max_len = min_len + 1;
        if (RuntimeConfig::max_seq <= 0)
        {
            for (const auto &s : sequences)
            {
                if (s.size() > max_len)
                    max_len = static_cast<int>(s.size());
            }
        }
        else
        {
            max_len = RuntimeConfig::max_seq;
        }
        std::vector<int> range(max_len - 1);
        std::generate(range.begin(), range.end(), [n = min_len]() mutable { return n++; });

        // For k = MinimumSequenceLengthThreshold to MaximumSequenceLength
        for (auto k : range)
        {
            std::map<HashVal, Sequence> seq_bucket;
            // BEGIN Place all subsequences of length k into buckets according to subsequence hash
            for (const auto &s : sequences)
            {
                if (s.size() < k)
                    continue;
                for (auto it = std::next(s.begin(), k - 1); it != s.end(); it++)
                {
                    Sequence subseq;
                    subseq.emplace(*it);
                    std::size_t sub_seq_hash = it->second;
                    for (auto pit = std::prev(it, k - 1); pit != it; pit++)
                    {
                        boost::hash_detail::hash_combine_impl(sub_seq_hash, pit->second);
                        subseq.emplace(*pit);
                    }
                    if (auto it = seq_bucket.find(sub_seq_hash); it != seq_bucket.end())
                    {
                        it->second.merge(subseq);
                    }
                    else
                    {
                        seq_bucket.emplace(sub_seq_hash, subseq);
                    }
                }
            }
            if (seq_bucket.size() == 0)
                continue;
            // END Place all subsequences of length k into buckets according to subsequence hash

            // BEGIN For each subsequence i and j in same bucket If CompareSequences (i,j,k) > SimilarityThreshold
            for (const auto bucket : seq_bucket)
            {
                Sequence bucket_same_seq;
                for (auto i = bucket.second.begin(); i != bucket.second.end(); i++)
                {
                    for (auto j = bucket.second.begin(); j != bucket.second.end(); j++)
                    {
                        if (i == j)
                            continue;

                        // eigentlich muesste hier wieder ein vergleich ueber die Similarity{} gemacht werden.
                        // Allerdings unterstuetzt dies nur einen vergleich von zwei klonpaaren und nicht von klonen
                        // beliebiger laenge. Eine nicht effinziente moeglichkeit waere, die Sequenzen i und j in
                        // paare zu unterteilen und fuer jedes moegliche paar die Similarity{} aufzurufen. Dies
                        // wuerde allerdings bei großen laengen zu langen laufzeiten fuehren. Daher wird in diesem Fall
                        // der aus den Sequenzbestandteilen kombinierte Hash auf gleichheit Überprüft.
                        if (i->second == j->second)
                        {

                            clones.erase(std::remove_if(clones.begin(),
                                                        clones.end(),
                                                        [i, j](const ClonePair &c) {
                                                            return (c.first == i->first || c.second == i->first) &&
                                                                   (c.first == j->first || c.second == j->first);
                                                        }),
                                         clones.end());

                            bucket_same_seq.insert(*i);
                            bucket_same_seq.insert(*j);
                        }
                    }
                }
                sequence_clones_.emplace(k, bucket_same_seq);
            }
            // END For each subsequence i and j in same bucket If CompareSequences (i,j,k) > SimilarityThreshold
        }
        std::cout << "Duration step2: " << timer_clones_step2.format();
    }
    clones_ = clones;

    std::cout << "clones step 2: " << sequence_clones_.size() << " " << sequence_clones_.rbegin()->second.size()
              << std::endl;

    std::cout << "Detection duration: " << timer_complete.format();
}

const Clones &Clone::clones() const
{
    return clones_;
}

const SequenceClones &Clone::sequence_clones() const
{
    return sequence_clones_;
}
} // namespace sre::lua::ast
