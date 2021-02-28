#pragma once
#include <boost/fusion/include/adapt_struct.hpp>
#include "ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::Name, name)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::binary, /*lhs_,*/ operator_, rhs_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::unary, operator_, rhs_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::expression, first_, rest_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::funcname, name_, chain_, self_chain_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::function, funcname_, parameters_, block_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::local_function, funcname_, parameters_, block_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::functiondef, parameters_, block_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::for_namelist, name_list_, exp_list_, block_)


BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::field, first, second)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::tableconstructor, first_, fields_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::block, stat_, retstat_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::chunk, block_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::label, name)
