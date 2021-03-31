#pragma once
#include <tuple>
#include "types.hpp"
namespace sre::lua::ast
{
class Metrics final
{
  public:
    explicit Metrics(const std::string &project, const std::string &output_file_name);
    void addChunkResult(const std::string &filename,
                        chunk &&chunk,
                        const std::size_t loc,
                        const Clones &clones,
                        const SequenceClones &sequences);
    void parsedFailed(const std::string &filename);
    void saveMetrics();

  private:
    void saveMassClones();

  private:
    const std::string project_;
    const std::string output_file_name_;
    // filename, <chunk, loc, clones, sequences>
    std::map<std::string, std::tuple<chunk, std::size_t, Clones, SequenceClones>> all_results_;
    std::vector<std::string> all_failed_;
    std::map<int, int> mass_clones_res_;
};
} // namespace sre::lua::ast
