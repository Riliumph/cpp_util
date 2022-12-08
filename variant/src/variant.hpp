#ifndef UTIL_VALIANT_HPP
#define UTIL_VALIANT_HPP
#ifdef BOOST_VARIANT_HPP
// boost::variant
#else
// std::variant
#include "variant/operator.hpp"
#include "variant/operator_io.hpp"
#include "variant/table.hpp"
#endif
// both
#endif // UTIL_VALIANT_HPP
