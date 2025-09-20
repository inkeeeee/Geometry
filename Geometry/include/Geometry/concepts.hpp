#ifndef GEOM_CONCEPTS_HPP
#define GEOM_CONCEPTS_HPP

#include <type_traits>

/**
 * @brief Concept for numeric types
 * @tparam T Type to check
 * 
 * Requires that T is an arithmetic type (integral or floating-point).
 * Used throughout the library to ensure type safety for mathematical operations.
 */
template<typename T> 
concept Numeric = std::is_arithmetic_v<T>;

#endif
