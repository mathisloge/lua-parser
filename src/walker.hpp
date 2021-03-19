#pragma once
#include <iostream>
#include "bucketer.hpp"
#include "types.hpp"
namespace sre::lua::ast
{
class Walker final
{
  private:
    const Clones &clones_;
    const Clones &prev_clone_matches_;
    Clones clone_matches_;

    template <typename T, typename Tp>
    bool is(const T &val, Ccit &start, Ccit end)
    {
        for (; start != end; start++)
        {
            // hier reicht ein simpler zeiger vergleich, da die Daten in diesen Schritten nicht kopiert werden.
            auto p1 = std::get_if<Tp>(&start->first);
            auto p2 = std::get_if<Tp>(&start->second);
            if ((p1 && *p1 == &val) || (p2 && *p2 == &val))
            {
                //! wenn bereits matches aus einem vorherigen lauf bekannt sind, muessen wir auch nur auf die bereits
                //! bekannten matchen.
                if (prev_clone_matches_.size() > 0)
                {
                    auto pit = std::find_if(prev_clone_matches_.begin(),
                                            prev_clone_matches_.end(),
                                            [start](const auto &x) { return (*start) == x; });
                    // wir haben zwar einen guelitgen match fuer einen der beiden klone, jedoch nicht fuer beide.
                    if (pit == prev_clone_matches_.end())
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }

  public:
    Walker(const Clones &clones, const Clones &prevs = {});
    Clones clones() const;

  public:
    bool operator()(const chunk &value);
    bool operator()(const block &value);
    bool operator()(const stat &value);
    bool operator()(const exp &value);
    bool operator()(const explist &value);
    bool operator()(const prefixexp &value);
    bool operator()(const expression &value);
    bool operator()(const primaryexpression &value);
    bool operator()(const assign_or_call &value);
    bool operator()(const label &value);
    bool operator()(const funcname &value);
    bool operator()(const function &value);
    bool operator()(const local_function &value);
    bool operator()(const namelist &value);
    bool operator()(const functiondef &value);
    bool operator()(const field &value);
    bool operator()(const fieldlist &value);
    bool operator()(const tableconstructor &value);
    bool operator()(const for_namelist &value);
    bool operator()(const funcbody &value);
    bool operator()(const functioncall &value);
    bool operator()(const args &value);
    bool operator()(const var &value);
    bool operator()(const var_wrapper &value);
    bool operator()(const varlist &value);
    bool operator()(const var_assign_or_list &value);
    bool operator()(const whiledo &value);
    bool operator()(const repeatuntil &value);
    bool operator()(const doblock &value);
    bool operator()(const forexp &value);
    bool operator()(const local_attnamelist_assign &value);
    bool operator()(const attnamelist &value);
    bool operator()(const name_attrib_pair &value);
    bool operator()(const ifelse &value);
    bool operator()(const ifelse_wrapper &value);
    bool operator()(const goto_stmt &value);
    bool operator()(const unary &value);
    bool operator()(const binary &value);
    bool operator()(const keyword_stmt &value);
    bool operator()(const numeral &value);
    bool operator()(const Name &value);
    bool operator()(const std::string &value);
    bool operator()(const double &value);
    bool operator()(const unsigned &value);
    bool operator()(const bool &value);
    bool operator()(const nil &nil);
};
} // namespace sre::lua::ast
