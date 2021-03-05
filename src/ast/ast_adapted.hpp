#pragma once
#include <boost/fusion/include/adapt_struct.hpp>
#include "ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::Name, name)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::binary, /*lhs_,*/ operator_, rhs_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::unary, operator_, rhs_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::expression, first_, rest_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::primaryexpression, first_, rest_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::assign_or_call, primaryexp_, var_action_)


BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::funcname, name_, chain_, self_chain_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::function, funcname_, parameters_, block_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::local_function, funcname_, parameters_, block_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::functiondef, parameters_, block_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::for_namelist, name_list_, exp_list_, block_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::var_wrapper, var_, next_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::varlist, rest_, explist_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::doblock, block_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::repeatuntil,  block_, exp_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::whiledo, exp_, block_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::field, first, second)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::tableconstructor, first_, fields_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::block, stat_, retstat_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::chunk, block_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::label, name)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::name_attrib_pair, name_, attrib_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::attnamelist, first_, rest_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::functioncall, name_, args_, prefix_exp_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::ifelse_wrapper, exp_, block_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::ifelse, first_, rest_, else_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::forexp, name_, exp_first_, exp_second_, exp_third_, block_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::local_attnamelist_assign, attnamelist_, explist_)
