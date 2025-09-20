#ifndef GEOM_RECTANGULAR_MATRIX_HPP
#define GEOM_RECTANGULAR_MATRIX_HPP

#include <algorithm>
#include <array>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <mdspan>
#include <format>
#include "concepts.hpp"

/**
 * @brief A fixed-size rectangular matrix with mathematical operations
 * @tparam T Numeric type of matrix elements
 * @tparam rows Number of rows in the matrix
 * @tparam columns Number of columns in the matrix
 * 
 * This class provides a comprehensive implementation of mathematical matrices
 * with support for basic operations, iterators, and matrix multiplication.
 * The size is fixed at compile-time for efficiency and type safety.
 */

template<Numeric T, size_t rows, size_t columns>
    requires (rows * columns <= std::numeric_limits<std::ptrdiff_t>::max())
class RectangularMatrix {
private:
    std::array<T, rows * columns> data; ///< Internal data storage

    /// @brief Create a multi-dimensional view of the data
    auto view() { 
        return std::mdspan<T, std::extents<size_t, rows, columns>> {data.data()}; 
    }

    /// @brief Create a const multi-dimensional view of the data
    auto view() const { 
        return std::mdspan<const T, std::extents<size_t, rows, columns>> {data.data()}; 
    }

public:
    // Type aliases for iterator support
    using iterator = std::array<T, rows * columns>::iterator;
    using reverse_iterator = std::array<T, rows * columns>::reverse_iterator;
    using const_iterator = std::array<T, rows * columns>::const_iterator;
    using const_reverse_iterator = std::array<T, rows * columns>::const_reverse_iterator;
    using value_type = T; 
    
    /// @brief Default constructor (initializes with default values)
    RectangularMatrix() : data{} {};
     
    /// @brief Constructor that fills matrix with a single value
    /// @param value The value to fill the matrix with
    RectangularMatrix(T value) {
        std::fill(data.begin(), data.end(), value);
    } 
    
    /// @brief Constructor from initializer list
    /// @param init_list Initializer list with matrix values (row-major order)
    RectangularMatrix(std::initializer_list<T> init_list) : RectangularMatrix(init_list.begin(), init_list.end()) {}

    /// @brief Constructor from iterator range
    /// @tparam Iter Forward iterator type
    /// @param first Beginning of range
    /// @param last End of range
    /// @throws std::invalid_argument if range size exceeds matrix capacity
    template<std::forward_iterator Iter>
    RectangularMatrix(Iter first, Iter last) {
        auto dist = std::distance(first, last);
        if (dist > rows * columns)
            throw std::invalid_argument("Invalid range size");
        
        std::copy(first, last, data.begin());
        std::fill(data.begin() + dist, data.end(), 0);
    }

    /// @brief Assignment from initializer list
    /// @param init_list Initializer list with matrix values
    /// @return Reference to this matrix
    /// @throws std::invalid_argument if initializer list size exceeds matrix capacity
    RectangularMatrix& operator=(std::initializer_list<T> init_list) {
        if (init_list.size() > rows * columns)
            throw std::invalid_argument("Invalid size of initializer_list");
        
        std::copy(init_list.begin(), init_list.end(), data.begin());
        std::fill(data.begin() + init_list.size(), data.end(), 0);
        return *this;
    }
    
    /// @brief Copy assignment operator
    RectangularMatrix& operator=(const RectangularMatrix &other) = default;
    
    /// @brief Matrix addition
    /// @tparam U Numeric type of other matrix elements
    /// @param other Matrix to add
    /// @return New matrix containing the sum
    template<Numeric U>
    RectangularMatrix operator+(const RectangularMatrix<U, rows, columns>& other) const {
        RectangularMatrix result {};
        std::transform(data.begin(), data.end(), other.data.begin(), result.data.begin(), std::plus<>{});
        
        return result;
    }

    /// @brief Matrix subtraction
    /// @tparam U Numeric type of other matrix elements
    /// @param other Matrix to subtract
    /// @return New matrix containing the difference
    template<Numeric U>
    RectangularMatrix operator-(const RectangularMatrix<U, rows, columns>& other) const {
        RectangularMatrix result {};
        std::transform(data.begin(), data.end(), other.data.begin(), result.data.begin(), std::minus<>{});
        
        return result;
    }
    
    /// @brief Matrix addition assignment
    /// @tparam U Numeric type of other matrix elements
    /// @param other Matrix to add
    /// @return Reference to this matrix
    template<Numeric U>
    RectangularMatrix operator+=(const RectangularMatrix<U, rows, columns>& other) {
        *this = *this + other; 
        return *this;
    }

    /// @brief Matrix subtraction assignment
    /// @tparam U Numeric type of other matrix elements
    /// @param other Matrix to subtract
    /// @return Reference to this matrix
    template<Numeric U>
    RectangularMatrix operator-=(const RectangularMatrix<U, rows, columns>& other) {
        *this = *this - other; 
        return *this;
    }
    
    /// @brief Equality comparison operator
    bool operator==(const RectangularMatrix& other) const = default;

    /// @brief Get number of rows
    /// @return Number of rows in the matrix
    constexpr size_t get_rows() const { return rows; }
    
    /// @brief Get number of columns
    /// @return Number of columns in the matrix
    constexpr size_t get_columns() const { return columns; }

    /// @brief Access element with bounds checking
    /// @param i Row index (0-based)
    /// @param j Column index (0-based)
    /// @return Reference to element at position (i, j)
    /// @throws std::out_of_range if indices are out of bounds
    T& at(size_t i, size_t j) {
        if (i >= rows || j >= columns) 
            throw std::out_of_range(std::format("Matrix index [{}, {}] is out of bounds for matrix {}x{}", i, j, rows, columns)); 
        auto v = view();
        return v[i, j];
    }
    
    /// @brief Const access element with bounds checking
    /// @param i Row index (0-based)
    /// @param j Column index (0-based)
    /// @return Const reference to element at position (i, j)
    /// @throws std::out_of_range if indices are out of bounds
    const T& at(size_t i, size_t j) const {
        if (i >= rows || j >= columns) 
            throw std::out_of_range(std::format("Matrix index [{}, {}] is out of bounds for matrix {}x{}", i, j, rows, columns)); 
        auto v = view();
        return v[i, j];
    }

    // Iterator methods
    iterator begin() { return data.begin(); }  
    const_iterator begin() const { return data.begin(); }  
    reverse_iterator rbegin() { return data.rbegin(); }  
    const_reverse_iterator rbegin() const { return data.rbegin(); }  
    const_iterator cbegin() const { return data.cbegin(); }  
    const_reverse_iterator crbegin() const { return data.crbegin(); }  
    iterator end() { return data.end(); }  
    const_iterator end() const { return data.end(); }  
    reverse_iterator rend() { return data.rend(); }  
    const_reverse_iterator rend() const { return data.rend(); }  
    const_iterator cend() const { return data.cend(); }  
    const_reverse_iterator crend() const { return data.crend(); }  
    
    /// @brief Get iterator to beginning of a specific row
    /// @param index Row index (0-based)
    /// @return Iterator to first element of the row
    /// @throws std::out_of_range if index is out of bounds
    iterator row_begin(size_t index) {
        if (index >= rows)
            throw std::out_of_range(std::format("Matrix row index must be less than {} for matrix {}x{}", rows, rows, columns)); 
        return begin() + index * columns;
    }

    /// @brief Get const iterator to beginning of a specific row
    /// @param index Row index (0-based)
    /// @return Const iterator to first element of the row
    /// @throws std::out_of_range if index is out of bounds
    const_iterator row_begin(size_t index) const {
        if (index >= rows)
            throw std::out_of_range(std::format("Matrix row index must be less than {} for matrix {}x{}", rows, rows, columns)); 
        return begin() + index * columns;
    }
    
    /// @brief Get iterator to end of a specific row
    /// @param index Row index (0-based)
    /// @return Iterator to one past the last element of the row
    /// @throws std::out_of_range if index is out of bounds
    iterator row_end(size_t index) {
        if (index >= rows)
            throw std::out_of_range(std::format("Matrix row index must be less than {} for matrix {}x{}", rows, rows, columns)); 
        return row_begin(index) + columns;
    }

    /// @brief Get const iterator to end of a specific row
    /// @param index Row index (0-based)
    /// @return Const iterator to one past the last element of the row
    /// @throws std::out_of_range if index is out of bounds
    const_iterator row_end(size_t index) const {
        if (index >= rows)
            throw std::out_of_range(std::format("Matrix row index must be less than {} for matrix {}x{}", rows, rows, columns)); 
        return row_begin(index) + columns;
    }

    /// @brief Get const iterator to beginning of a specific row
    /// @param index Row index (0-based)
    /// @return Const iterator to first element of the row
    /// @throws std::out_of_range if index is out of bounds
    const_iterator row_cbegin(size_t index) const {
        if (index >= rows)
            throw std::out_of_range(std::format("Matrix row index must be less than {} for matrix {}x{}", rows, rows, columns)); 
        return cbegin() + index * columns;
    }
    
    /// @brief Get const iterator to end of a specific row
    /// @param index Row index (0-based)
    /// @return Const iterator to one past the last element of the row
    /// @throws std::out_of_range if index is out of bounds
    const_iterator row_cend(size_t index) const {
        if (index >= rows)
            throw std::out_of_range(std::format("Matrix row index must be less than {} for matrix {}x{}", rows, rows, columns)); 
        return row_cbegin(index) + columns;
    }

    /**
     * @brief Base class for column iterators
     * @tparam IsConst Whether this is a const iterator
     * 
     * This iterator traverses the matrix in column-major order,
     * providing access to each column sequentially.
     */
    template<bool IsConst>
    class column_iterator_base {
    public:
        using value_type = T;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<IsConst, const T*, T*>;
        using reference = std::conditional_t<IsConst, const T&, T&>;

        /// @brief Default constructor
        column_iterator_base() = default;
        
        /// @brief Constructor from pointer and position
        /// @param p Pointer to matrix data
        /// @param number Linear position in column-major order
        column_iterator_base(pointer p, size_t number)
            : ptr(p), number(number) {}

        /// @brief Dereference operator
        /// @return Reference to current element
        reference operator*() const { 
            size_t row = number % rows;
            size_t column = number / rows;
            return ptr[row * columns + column]; 
        }

        /// @brief Arrow operator
        /// @return Pointer to current element
        pointer operator->() const {
            size_t row = number % rows;
            size_t column = number / rows;
            return ptr + row * columns + column; 
        }
        
        /// @brief Subscript operator
        /// @param n Offset from current position
        /// @return Reference to element at offset
        reference operator[](difference_type n) const {
            size_t row = (number + n) % rows;
            size_t column = (number + n) / rows;
            return ptr[row * columns + column]; 
        }
        
        /// @brief Pre-increment operator
        /// @return Reference to incremented iterator
        column_iterator_base& operator++() {
            number++;
            return *this;
        }

        /// @brief Post-increment operator
        /// @return Copy of iterator before incrementing
        column_iterator_base operator++(int) {
            column_iterator_base tmp = *this;
            ++(*this);
            return tmp;
        }

        /// @brief Pre-decrement operator
        /// @return Reference to decremented iterator
        column_iterator_base& operator--() {
            number--;
            return *this;
        }

        /// @brief Post-decrement operator
        /// @return Copy of iterator before decrementing
        column_iterator_base operator--(int) {
            column_iterator_base tmp = *this;
            --(*this);
            return tmp;
        }

        /// @brief Compound addition assignment
        /// @param n Offset to add
        /// @return Reference to modified iterator
        column_iterator_base& operator+=(difference_type n) {
            number += n;
            return *this;
        }

        /// @brief Compound subtraction assignment
        /// @param n Offset to subtract
        /// @return Reference to modified iterator
        column_iterator_base& operator-=(difference_type n) {
            return *this += -n;
        }

        /// @brief Addition operator
        /// @param n Offset to add
        /// @return New iterator at offset position
        column_iterator_base operator+(difference_type n) const {
            column_iterator_base result = *this;
            result += n;
            return result;
        }
        
        /// @brief Addition operator (friend)
        /// @param n Offset to add
        /// @param it Iterator to add to
        /// @return New iterator at offset position
        friend column_iterator_base operator+(difference_type n, const column_iterator_base& it) {
            return it + n;
        }

        /// @brief Subtraction operator
        /// @param n Offset to subtract
        /// @return New iterator at offset position
        column_iterator_base operator-(difference_type n) const {
            column_iterator_base result = *this;
            result -= n;
            return result;
        }

        /// @brief Conversion to const iterator
        operator column_iterator_base<true>() const {
            return column_iterator_base<true>(ptr, number);
        }

        /// @brief Difference operator
        /// @param other Iterator to subtract
        /// @return Distance between iterators
        difference_type operator-(const column_iterator_base& other) const {
            return static_cast<difference_type>(number) - static_cast<difference_type>(other.number);
        }

        /// @brief Equality comparison
        bool operator==(const column_iterator_base& other) const = default;
        
        /// @brief Three-way comparison operator
        /// @param other Iterator to compare with
        /// @return Ordering relationship
        auto operator<=>(const column_iterator_base& other) const {
            return number <=> other.number;
        }

    private:
        pointer ptr = nullptr;    ///< Pointer to matrix data
        size_t number = 0;        ///< Linear position in column-major order
    };

    using column_iterator = column_iterator_base<false>;          ///< Non-const column iterator
    using const_column_iterator = column_iterator_base<true>;     ///< Const column iterator

    /// @brief Get iterator to beginning of a specific column
    /// @param index Column index (0-based)
    /// @return Iterator to first element of the column
    /// @throws std::out_of_range if index is out of bounds
    column_iterator column_begin(size_t index) {
        if (index >= columns)
            throw std::out_of_range(std::format("Matrix column index must be less than {} for matrix {}x{}", columns, rows, columns)); 

        return column_iterator(data.data(), rows * index);
    }

    /// @brief Get const iterator to beginning of a specific column
    /// @param index Column index (0-based)
    /// @return Const iterator to first element of the column
    /// @throws std::out_of_range if index is out of bounds
    column_iterator column_begin(size_t index) const {
        if (index >= columns)
            throw std::out_of_range(std::format("Matrix column index must be less than {} for matrix {}x{}", columns, rows, columns)); 
        return column_iterator(data.data(), rows * index);
    }
    
    /// @brief Get iterator to end of a specific column
    /// @param index Column index (0-based)
    /// @return Iterator to one past the last element of the column
    /// @throws std::out_of_range if index is out of bounds
    column_iterator column_end(size_t index) {
        if (index >= columns)
            throw std::out_of_range(std::format("Matrix column index must be less than {} for matrix {}x{}", columns, rows, columns)); 
        return column_iterator(data.data(), rows * (index + 1));
    }
    
    /// @brief Get const iterator to end of a specific column
    /// @param index Column index (0-based)
    /// @return Const iterator to one past the last element of the column
    /// @throws std::out_of_range if index is out of bounds
    column_iterator column_end(size_t index) const {
        if (index >= columns)
            throw std::out_of_range(std::format("Matrix column index must be less than {} for matrix {}x{}", columns, rows, columns)); 
        return column_iterator(data.data(), rows * (index + 1));
    }

    /// @brief Get const iterator to beginning of a specific column
    /// @param index Column index (0-based)
    /// @return Const iterator to first element of the column
    /// @throws std::out_of_range if index is out of bounds
    const_column_iterator column_cbegin(size_t index) const {
        if (index >= columns)
            throw std::out_of_range(std::format("Matrix column index must be less than {} for matrix {}x{}", columns, rows, columns)); 
        return const_column_iterator(data.data(), rows * index);
    }

    /// @brief Get const iterator to end of a specific column
    /// @param index Column index (0-based)
    /// @return Const iterator to one past the last element of the column
    /// @throws std::out_of_range if index is out of bounds
    const_column_iterator column_cend(size_t index) const {
        if (index >= columns)
            throw std::out_of_range(std::format("Matrix column index must be less than {} for matrix {}x{}", columns, rows, columns)); 
        return const_column_iterator(data.data(), rows * (index + 1));
    }

    /// @brief Create transposed matrix
    /// @return New matrix with rows and columns swapped
    RectangularMatrix<T, columns, rows> transposed() const {
        return RectangularMatrix<T, columns, rows>(this->column_cbegin(0), this->column_cend(columns - 1));
    } 
    
    /// @brief Matrix multiplication
    /// @tparam U Numeric type of other matrix elements
    /// @tparam other_columns Number of columns in the other matrix
    /// @param other Matrix to multiply with
    /// @return New matrix containing the product
    template<Numeric U, size_t other_columns>
    auto operator*(const RectangularMatrix<U, columns, other_columns>& other) const {
        using ResultType = decltype(std::declval<T>() * std::declval<U>());
        RectangularMatrix<ResultType, rows, other_columns> result {};

        for (auto it = result.begin(); it != result.end(); it++) {
            size_t index = it - result.begin();
            size_t row = index / other_columns;
            size_t column = index % other_columns;

            *it = std::transform_reduce(row_cbegin(row), row_cend(row),
                                                                other.column_cbegin(column), ResultType{}, std::plus<>(), std::multiplies<>());
        }

        return result;
    }
};

/// @brief Static assertion to ensure column_iterator is a random access iterator
static_assert(std::random_access_iterator<RectangularMatrix<double, 5, 5>::column_iterator>);

#endif
