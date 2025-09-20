#ifndef GEOM_POINT_HPP
#define GEOM_POINT_HPP

#include "RectangularMatrix.hpp"

/**
 * @brief Point in n-dimensional space
 * @tparam T Numeric type for point coordinates
 * @tparam dimension Dimension of the space
 * 
 * A point is represented as a 1×dimension matrix, allowing use of
 * all matrix operations while providing geometric semantics.
 */
template<Numeric T, size_t dimension>
using Point = RectangularMatrix<T, 1, dimension>;

#endif
