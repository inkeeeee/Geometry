#ifndef GEOM_VECTOR_HPP
#define GEOM_VECTOR_HPP

#include "Point.hpp"
#include <cmath>

/**
 * @brief Mathematical vector in n-dimensional space
 * @tparam T Numeric type of vector components
 * @tparam dimension Dimension of the vector space
 * 
 * Represents a mathematical vector with operations for length calculation
 * and normalization. Implemented as a 1×dimension matrix.
 */
template<Numeric T, size_t dimension>
class Vector {
private: 
    RectangularMatrix<T, 1, dimension> data; ///< Internal data storage as a matrix

public:
    /// @brief Default constructor
    Vector() = default;
    
    /// @brief Constructor from matrix
    /// @param data_ Matrix containing vector components
    Vector(const RectangularMatrix<T, 1, dimension>& data_) : data(data_) {};

    /// @brief Constructor from initializer list
    /// @param data_ Initializer list with vector components
    Vector(std::initializer_list<T> data_) {
        data = RectangularMatrix<T, 1, dimension>(data_);
    }
    
    /// @brief Constructor from two points (direction vector)
    /// @param from Starting point
    /// @param to Ending point
    Vector(const Point<T, dimension>& from, const Point<T, dimension>& to) : data(from - to) {};
    
    /// @brief Calculate vector length (magnitude)
    /// @return Length of the vector as double
    double length() const {
        double square = (data * data.transposed()).at(0, 0);
        return sqrt(square);
    }
    
    /// @brief Get internal matrix data (non-const)
    /// @return Reference to internal matrix
    RectangularMatrix<T, 1, dimension>& get_data() { return data; }
    
    /// @brief Get internal matrix data (const)
    /// @return Const reference to internal matrix
    const RectangularMatrix<T, 1, dimension>& get_data() const { return data; }

    /// @brief Get specific coordinate of the vector
    /// @param index Coordinate index (0-based)
    /// @return Value of the coordinate at the specified index
    T get_coord(size_t index) const {
        return data.at(0, index);
    }

    /// @brief Create a normalized version of the vector
    /// @return New vector with same direction but length 1
    /// @note Returns zero vector if original length is zero
    Vector<double, dimension> normalize() const {
        Vector<double, dimension> result {};
        double len = length();
        
        if (len == 0) {
            return Vector<double, dimension>{};
        }
        
        std::transform(data.cbegin(), data.cend(), result.get_data().begin(), 
                       [len](T value) { return static_cast<double>(value) / len; });
        
        return result;
    }
};

#endif
