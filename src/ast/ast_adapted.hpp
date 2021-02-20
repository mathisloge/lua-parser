#pragma once
#include <boost/fusion/include/adapt_struct.hpp>
#include "ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::binary, /*lhs_,*/ operator_, rhs_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::unary, operator_, rhs_)

//BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::operation, operator_, operand_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::expression, first_, rest_)
//BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::expression, op_)
//BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::label, name)
//BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::block, stat_, retstat_)

BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::stat, test)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::block, stat_, retstat_)
BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::chunk, block_)
