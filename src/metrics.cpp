#include "metrics.hpp"
#include <fstream>
#include <iostream>
#include "mass.hpp"
namespace sre::lua::ast
{

Metrics::Metrics(const std::string &project, const std::string &output_file_name)
    : project_{project}
    , output_file_name_{output_file_name}
{}

void Metrics::addChunkResult(const std::string &filename,
                             chunk &&chunk,
                             const std::size_t loc,
                             const Clones &clones,
                             const SequenceClones &sequences)
{
    const auto clones_size = clones.size();
    const auto seq_size = sequences.size(); // all found sequences. not the single sequence length.
    const auto max_seq_len = sequences.rbegin()->first;
    all_results_.emplace(filename, std::make_tuple(std::move(chunk), loc, clones, sequences));

    std::set<Unit> uniques_;
    for (const auto &c : clones)
    {
        uniques_.insert(c.first);
        uniques_.insert(c.second);
    }
    for (const auto &c : uniques_)
    {
        const auto mass = std::visit([](auto var) { return Mass{}(*var); }, c);
        if (auto it = mass_clones_res_.find(mass); it != mass_clones_res_.end())
            it->second++;
        else
            mass_clones_res_.emplace(mass, 1);
    }
}

void Metrics::parsedFailed(const std::string &filename)
{
    all_failed_.push_back(filename);
}

void Metrics::saveMetrics()
{
    std::ofstream file{output_file_name_};
    if (file.is_open())
    {
        file << "project,file,loc,nclones,nsequences,maxseqlen" << std::endl;
        for (const auto &r : all_results_)
        {
            int max_seq_len = 0;
            if (std::get<3>(r.second).size() > 0)
                max_seq_len = std::get<3>(r.second).rbegin()->first;
            file << project_ << "," << r.first << "," << std::get<1>(r.second) << "," << std::get<2>(r.second).size()
                 << "," << std::get<3>(r.second).size() << "," << max_seq_len << std::endl;
        }
    }
    else
    {
        std::cout << "CANNOT WRITE METRICS!" << std::endl;
    }
    saveMassClones();
    std::cout << all_failed_.size() << " files could not be parsed." << std::endl;
}

void Metrics::saveMassClones()
{
    std::ofstream file{"mass_" + output_file_name_};

    if (file.is_open())
    {
        file << "project,mass,occurrences" << std::endl;
        for (const auto &r : mass_clones_res_)
        {
            file << project_ << "," << r.first << "," << r.second << std::endl;
        }
    }
    else
    {
        std::cout << "CANNOT WRITE MASS METRICS!" << std::endl;
    }
}
} // namespace sre::lua::ast
