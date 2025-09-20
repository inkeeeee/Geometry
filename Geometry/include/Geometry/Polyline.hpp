#ifndef GEOM_POLYLINE_HPP
#define GEOM_POLYLINE_HPP

#include "Vector.hpp"
#include <iostream>
/**
 * @brief 3D polyline with named points
 * @tparam T Numeric type for point coordinates
 * 
 * Represents a 3D polyline (connected series of points) with support for
 * geometric operations like rotation, translation, and length calculation.
 * Each point can have an associated character name for rendering.
 */
template<Numeric T>
class Polyline {
private:
    Point<T, 3> *points = nullptr;    ///< Array of 3D points
    char *names = nullptr;            ///< Array of point names
    size_t capacity = 0;              ///< Current capacity of arrays
    size_t size = 0;                  ///< Current number of points
    static const size_t buffsize = 5;        ///< Size increment for resizing

    /// @brief reallocate the array
    /// @param new_capacity New capacity for arrays
    void reallocate(size_t new_capacity) {
        if (capacity == new_capacity) return;
         
        Point<T, 3> *new_points = nullptr;
        char *new_names = nullptr;

        try {
            new_points = new Point<T, 3>[new_capacity];
            new_names = new char[new_capacity];
        } catch (std::bad_alloc& e) {
            delete [] new_points;
            throw;
        }
        
        std::copy(begin(), end(), new_points);
        std::copy(names_begin(), names_end(), new_names);

        delete [] points;
        delete [] names;

        points = new_points;
        names = new_names;
        capacity = new_capacity;
    }

public:
    /// @brief Default constructor
    Polyline() = default;
    
    /// @brief Copy constructor
    /// @param other Polyline to copy
    Polyline(const Polyline<T>& other) {
        if (other.size > 0) {
            reallocate(other.size);
            std::copy(other.begin(), other.end(), begin());
            std::copy(other.names_begin(), other.names_end(), names_begin());
            size = other.size;
        }
    }

    /// @brief Move constructor
    /// @param other Polyline to move from
    Polyline(Polyline<T>&& other) {
        swap(*this, other);
    }
    
    /// @brief Destructor
    ~Polyline() {
        delete[] points;
        delete[] names;
    }
    
    /// @brief Copy assignment operator
    /// @param other Polyline to copy
    /// @return Reference to this polyline
    Polyline& operator=(const Polyline<T>& other) { 
        Polyline tmp(other);
        swap(*this, tmp);

        return *this;
    }

    /// @brief Move assignment operator
    /// @param other Polyline to move from
    /// @return Reference to this polyline
    Polyline& operator=(Polyline<T>&& other) {
        swap(*this, other);
        return *this;
    }
    
    /// @brief Swap two polylines
    /// @param first First polyline
    /// @param second Second polyline
    friend void swap(Polyline<T>& first, Polyline<T>& second) {
        std::swap(first.capacity, second.capacity);
        std::swap(first.size, second.size);
        std::swap(first.points, second.points);
        std::swap(first.names, second.names);
    }
    
    /// @brief Get number of points in the polyline
    /// @return Number of points
    size_t get_size() const { return size; }

    /// @brief Add a point to the polyline
    /// @param point 3D point to add
    /// @param name Character name for the point
    void add_point(const Point<T, 3>& point, const char name) {
        if (size == capacity)
            reallocate(capacity + buffsize);
        
        points[size] = point;
        names[size] = name;
        size++;
    }
    
    /// @brief Equality comparison operator
    /// @param other Polyline to compare with
    /// @return True if polylines are equal
    bool operator==(const Polyline<T>& other) {
        if (size != other.size) return false;
        return std::equal(begin(), end(), other.begin());
    }

    /// @brief Calculate total length of the polyline
    /// @return Length as double
    double length() const {
        if (size <= 1) return 0;

        return std::inner_product(begin(), end() - 1, begin() + 1, double{}, 
                           std::plus<>(), [](const Point<T, 3>& a, const Point<T, 3>& b) {
                                  Vector<T, 3> vec(a, b); 
                                  return vec.length();  });
    }
    
    /// @brief Shift all points by a vector
    /// @param diff Translation vector
    void shift(const Vector<T, 3> &diff) {
        std::transform(begin(), end(), begin(), [diff](const Point<T, 3>& a) {return a + diff.get_data();});
    }
    
    /// @brief Rotate all points around an axis
    /// @param axis Rotation axis vector
    /// @param rad Rotation angle in radians
    void rotate(const Vector<T, 3> &axis, const double rad) {
        double len = axis.length(); 
        
        double x = axis.get_coord(0) / len, y = axis.get_coord(1) / len, z = axis.get_coord(2) / len;
        double cos_ = cos(rad), sin_ = sin(rad);
        
        RectangularMatrix<double, 3, 3> rotate_matrix = {
            cos_ + x * x * (1 - cos_), y * x * (1 - cos_) + z * sin_, z * x * (1 - cos_) - y * sin_,
            x * y * (1 - cos_) - z * sin_, cos_ + y * y * (1 - cos_), z * y * (1 - cos_) + x * sin_,
            x * z * (1 - cos_) + y * sin_, y * z * (1 - cos_) - x * sin_, cos_ + z * z * (1 - cos_)
        };
        
        std::transform(begin(), end(), begin(), [rotate_matrix](Point<T, 3>& point) { return point * rotate_matrix; });
    }
    

    /// @brief Merge another polyline into this one
    /// @param other Polyline to merge (const reference)
    void merge_line(const Polyline& other) {
        if (other.size == 0) return;
        if (capacity < size + other.size)
            reallocate(capacity + other.size);
        
        std::copy(other.begin(), other.end(), end());
        std::copy(other.names_begin(), other.names_end(), names_end());
        size += other.size;
    }

    /// @brief Merge another polyline into this one (move version)
    /// @param other Polyline to merge (rvalue reference)
    void merge_line(Polyline&& other) {
        if (other.size == 0) return;

        if (capacity >= size + other.size) {
            std::move(other.begin(), other.end(), end());
            std::move(other.names_begin(), other.names_end(), names_end());
            size += other.size;
            return;
        }
        
        if (other.capacity >= size + other.size) {
            std::move_backward(other.begin(), other.end(), other.begin() + size + other.size);
            std::move_backward(other.names_begin(), other.names_end(), other.names_begin() + size + other.size);
            
            std::move(begin(), end(), other.begin());
            std::move(names_begin(), names_end(), other.names_begin());
            
            swap(*this, other);
            size += other.size;
            return;
        }
        
        reallocate(size + other.size);
        std::move(other.begin(), other.end(), end());
        std::move(other.names_begin(), other.names_end(), names_end());
        size += other.size;
        
    }

    /// @brief Remove the point that is most isolated from its neighbors
    void remove_most_isolated_point() {
        if (size <= 2) return;

        auto [dist, index] = std::reduce(begin() + 1, end() - 1, std::pair<double, size_t>{0, 0},
            [this](std::pair<double, size_t>result, const Point<T, 3>& point) {
                auto it = &point;
                size_t index = it - begin();
                Vector<T, 3> vec1(*(it - 1), point);
                Vector<T, 3> vec2(point, *(it + 1));
                double dist = std::min(vec1.length(), vec2.length());
                
                auto [cur_dist, cur_index] = result;
                return dist > cur_dist ? std::pair<double, size_t>{dist, index} : result;
            });

        Vector<T, 3> vec1(points[0], points[1]);
        Vector<T, 3> vec2(points[size - 2], points[size - 1]);
        double len1 = vec1.length();
        double len2 = vec2.length();

        if (len1 > dist) {
            dist = len1;
            index = 0;
        }

        if (len2 > dist) {
            dist = len2;
            index = size - 1;
        }

        std::copy(begin() + index + 1, end(), begin() + index);
        std::copy(names_begin() + index + 1, names_end(), names_begin() + index);
        
        size--;
    }
    
    /// @brief Get the name of a point
    /// @param index Point index (0-based)
    /// @return Character name of the point
    /// @throws std::out_of_range if index is invalid
    char get_point_name(size_t index) const {
        if (index >= size)
            throw std::out_of_range("");

        return names[index];
    }

    // Iterator methods
    const Point<T, 3> *begin() const { return points; }
    const Point<T, 3> *end() const { return points + size; }
    Point<T, 3> *begin() { return points; }
    Point<T, 3> *end() { return points + size; }
    char *names_begin() { return names; }
    const char *names_begin() const { return names; }
    char *names_end() { return names + size; }
    const char *names_end() const { return names + size; }
};

#endif
