#ifndef INCLUDE_VALIANT_HPP
#define INCLUDE_VALIANT_HPP
#ifdef BOOST_VARIANT_HPP
// boost::variant
#else
// std::variant
#include "variant/map/table.hpp"
#include "variant/operator.hpp"
#include "variant/operator_io.hpp"
#endif
// both
#endif // INCLUDE_VALIANT_HPP
