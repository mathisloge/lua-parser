#pragma once
#include <boost/fusion/include/adapt_struct.hpp>
#include "ast.hpp"

    BOOST_FUSION_ADAPT_STRUCT(sre::lua::ast::chunk, block)
