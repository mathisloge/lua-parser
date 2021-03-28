#pragma once
#include <set>
#include <tuple>
#include <variant>
#include <vector>
#include <map>
#include <ast/ast.hpp>
namespace sre::lua::ast
{
using Unit = std::variant<const chunk *,
                          const block *,
                          const stat *,
                          const exp *,
                          const explist *,
                          const prefixexp *,
                          const expression *,
                          const primaryexpression *,
                          const assign_or_call *,
                          const label *,
                          const funcname *,
                          const function *,
                          const local_function *,
                          const namelist *,
                          const functiondef *,
                          const field *,
                          const fieldlist *,
                          const tableconstructor *,
                          const for_namelist *,
                          const funcbody *,
                          const functioncall *,
                          const args *,
                          const var *,
                          const var_wrapper *,
                          const varlist *,
                          const var_assign_or_list *,
                          const whiledo *,
                          const repeatuntil *,
                          const doblock *,
                          const forexp *,
                          const local_attnamelist_assign *,
                          const attnamelist *,
                          const name_attrib_pair *,
                          const ifelse *,
                          const ifelse_wrapper *,
                          const goto_stmt *,
                          const unary *,
                          const binary *,
                          const keyword_stmt *,
                          const numeral *>;

using MassVal = std::size_t;
using HashVal = std::size_t;
using BucketItem = std::tuple<MassVal, HashVal, Unit>;
using BucketList = std::vector<BucketItem>;

using ClonePair = std::pair<Unit, Unit>;
using Clones = std::vector<ClonePair>;
using Cit = Clones::iterator;
using Ccit = Clones::const_iterator;

using SeqItem = std::pair<Unit, HashVal>;
using Sequence = std::set<SeqItem>;
using AllSequences = std::vector<Sequence>;
using SequenceClones = std::multimap<int, Sequence>;
} // namespace sre::lua::ast
