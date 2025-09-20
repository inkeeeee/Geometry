#ifndef COMMAND_INTERFACE_HPP
#define COMMAND_INTERFACE_HPP

#include <vector>
#include <string>
#include "Geometry/Polyline.hpp"
#include "Graphics/SpatialBuffer.hpp"

/**
 * @class CommandInterface
 * @brief Interactive command-line interface for 3D polyline manipulation and rendering
 * 
 * Provides a comprehensive command system for creating, modifying, and visualizing
 * 3D polylines with ASCII rendering capabilities. Supports geometric operations
 * including translation, rotation, merging, and length calculation.
 */
class CommandInterface {
private:
    std::vector<Polyline<double>> lines;                  ///< Container for all user-created polylines
    RectangularMatrix<double, 3, 2> projection_matrix;    ///< 3D to 2D projection matrix for rendering
    static constexpr size_t BUFFER_WIDTH = 100;           ///< Width of the rendering buffer in characters
    static constexpr size_t BUFFER_HEIGHT = 100;          ///< Height of the rendering buffer in characters

    /// @brief Display available commands and usage information
    void print_help() const;
    
    /// @brief Create a new empty polyline
    void create_line();
    
    /// @brief Add a point to specified polyline
    /// @param tokens Command tokens: ["add", "point", index, x, y, z, name]
    void add_point(const std::vector<std::string>& tokens);
    
    /// @brief Merge two polylines and remove the second one
    /// @param tokens Command tokens: ["merge", index1, index2]
    void merge_lines(const std::vector<std::string>& tokens);
    
    /// @brief Render all polylines to ASCII art using spatial buffer
    void render();
    
    /// @brief Calculate and display length of specified polyline
    /// @param tokens Command tokens: ["get", "length", index]
    void get_length(const std::vector<std::string>& tokens) const;
    
    /// @brief Translate polyline by specified vector
    /// @param tokens Command tokens: ["shift", index, x, y, z]
    void shift_line(const std::vector<std::string>& tokens);
    
    /// @brief Rotate polyline around specified axis by given angle
    /// @param tokens Command tokens: ["rotate", index, x, y, z, angle_deg]
    void rotate_line(const std::vector<std::string>& tokens);
    
    /// @brief Display list of all polylines with their points and names
    void get_lines() const;
    
    /// @brief Delete specified polyline from collection
    /// @param tokens Command tokens: ["del", "line", index]
    void delete_line(const std::vector<std::string>& tokens);
    
    /// @brief Process and execute user command
    /// @param tokens Tokenized command input from user
    void process_command(const std::vector<std::string>& tokens);

public:
    /**
     * @brief Construct a new Command Interface object
     * 
     * Initializes projection matrix for 3D to 2D rendering and prepares
     * the command system for user interaction.
     */
    CommandInterface();
    
    /**
     * @brief Start the command interface main loop
     * 
     * Enters interactive mode, displaying prompt and processing user commands
     * until exit condition is met. Handles input validation and error reporting.
     */
    void run();
};

#endif
