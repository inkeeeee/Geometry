#ifndef GEOM_SPATIAL_BUFFER_HPP
#define GEOM_SPATIAL_BUFFER_HPP

#include "Geometry/Polyline.hpp"
#include <iostream>

/**
 * @brief ASCII renderer for 3D objects
 * @tparam T Numeric type for coordinates
 * @tparam width Width of the character buffer
 * @tparam height Height of the character buffer
 * 
 * Provides functionality to project 3D points to 2D and render polylines
 * using ASCII characters. Uses a character buffer for efficient rendering.
 */
template <Numeric T, size_t width, size_t height>
class SpatialBuffer {
private:
    RectangularMatrix<char, height, width> buffer;        ///< Character buffer for rendering
    RectangularMatrix<T, 3, 2> projection_matrix;         ///< Projection matrix for 3D→2D conversion
    
    /// @brief Project a 3D point to 2D screen coordinates
    /// @param point 3D point to project
    /// @return 2D point in screen coordinates
    Point<int, 2> project(const Point<T, 3>& point) const {
        auto projected = point * projection_matrix;
        
        int x = static_cast<int>(projected.at(0, 0) * 0.5 + width / 2);
        int y = static_cast<int>(projected.at(0, 1) * 0.5 + height / 2);
        
        x = std::max(0, std::min(static_cast<int>(width) - 1, x));
        y = std::max(0, std::min(static_cast<int>(height) - 1, y));
        
        return {x, y};
    }

    /// @brief Draw a single point in the buffer
    /// @param x X coordinate
    /// @param y Y coordinate
    /// @param symbol Character to use for drawing
    void draw_point(int x, int y, char symbol = '*') {
        if (x >= 0 && x < width - 1 && y >= 0 && y < height && (buffer.at(y, x) == ' ' || buffer.at(y, x) == '*'))
            buffer.at(y, x) = symbol;
    }

    /// @brief Draw a line between two points using Bresenham's algorithm
    /// @param x0 Starting X coordinate
    /// @param y0 Starting Y coordinate
    /// @param x1 Ending X coordinate
    /// @param y1 Ending Y coordinate
    void draw_line(int x0, int y0, int x1, int y1) {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (x0 != x1 || y0 != y1) {
            draw_point(x0, y0);
            
            int e2 = 2 * err;
            
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }

public:
    /// @brief Constructor with projection matrix
    /// @param proj Projection matrix for 3D→2D conversion
    SpatialBuffer(const RectangularMatrix<T, 3, 2>& proj)
        : projection_matrix(proj) {
        clear();
    }

    /// @brief Clear the buffer (fill with spaces)
    void clear() {
        std::fill(buffer.begin(), buffer.end(), ' ');
        std::fill(buffer.column_begin(width - 1), buffer.column_end(width - 1), '\n');
    }

    /// @brief Add a line between two 3D points to the buffer
    /// @param p1 First 3D point
    /// @param p2 Second 3D point
    /// @param n1 Character to use for first point
    /// @param n2 Character to use for second point
    void add_line(const Point<T, 3>& p1, const Point<T, 3>& p2, char n1, char n2) {
        auto start = project(p1);
        auto end = project(p2);
        draw_line(start.at(0, 0), start.at(0, 1), end.at(0, 0), end.at(0, 1));
        
        draw_point(start.at(0, 0), start.at(0, 1), n1);
        draw_point(end.at(0, 0), end.at(0, 1), n2);
    }

    /// @brief Add a polyline to the buffer
    /// @param polyline Polyline to render
    /// @return Reference to this buffer for chaining
    SpatialBuffer& operator<<(const Polyline<T>& polyline) {
        if (polyline.get_size() < 2) return *this;
        
        int index = 0;

        (void) std::adjacent_find(polyline.begin(), polyline.end(),
                    [this, index, polyline](const Point<T, 3>& a, const Point<T, 3>& b) mutable {
                        add_line(a, b, polyline.get_point_name(index), polyline.get_point_name(index + 1));
                        ++index;
                        return false;
                    });

        return *this;
    }

    /// @brief Render the buffer to an output stream
    /// @param os Output stream to render to
    /// @param buf Buffer
    friend std::ostream& operator<<(std::ostream& os, const SpatialBuffer<T, width, height>& buf) {
        std::ostream_iterator<char> out(os);
        std::copy(buf.buffer.begin(), buf.buffer.end(), out);
        return os;
    }
};

#endif
