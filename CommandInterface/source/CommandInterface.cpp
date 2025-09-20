#include "CommandInterface.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
CommandInterface::CommandInterface() {
    // Аксонометрическая проекция:
    // - Ось X: прямо вправо
    // - Ось Z: прямо вверх
    // - Ось Y: вглубь под углом 35° к оси X
    
    const double angle = 40.0 * M_PI / 180.0; // 35 градусов в радианах
    const double cos_ = cos(angle);
    const double sin_ = sin(angle);
    
    // Матрица проекции:
    // X -> полностью на экранный X
    // Y -> cos35 * Y на экранный X, sin35 * Y на экранный Y (вниз)
    // Z -> полностью на экранный Y (вверх)
    
    projection_matrix = {
        1.0,   0.0,    // X: X -> X, 0
        cos_, -sin_,  // Y: Y*cos35 -> X, -Y*sin35 -> Y (минус для правильного направления)
        0.0,   -1.0     // Z: 0 -> X, Z -> Y
    };
    // Масштабируем для лучшего отображения
}

void CommandInterface::run() {
    std::string input;
    print_help();

    while (true) {
        std::cout << "> ";
        
        // Проверяем на Ctrl+D (EOF)
        if (!std::getline(std::cin, input)) {
            if (std::cin.eof()) {
                std::cout << "\nExiting program..." << std::endl;
                break;
            }
            std::cin.clear();
            continue;
        }
        
        if (input.empty()) continue;
        
        std::vector<std::string> tokens;
        std::istringstream iss(input);
        std::string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }
        
        if (tokens[0] == "exit") {
            break;
        }
        
        process_command(tokens);
    }
}
void CommandInterface::process_command(const std::vector<std::string>& tokens) {
    const std::string& command = tokens[0];
    
    if (command == "create") {
        if (tokens.size() > 1 && tokens[1] == "line") {
            create_line();
        } else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }
    else if (command == "add") {
        if (tokens.size() > 1 && tokens[1] == "point") {
            add_point(tokens);
        } else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }
    else if (command == "merge") {
        merge_lines(tokens);
    }
    else if (command == "render") {
        render();
    }
    else if (command == "get") {
        if (tokens.size() > 1) {
            if (tokens[1] == "length") {
                get_length(tokens);
            } else if (tokens[1] == "lines") {
                get_lines();
            } else {
                std::cout << "Unknown command. Type 'help' for available commands.\n";
            }
        } else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }
    else if (command == "shift") {
        shift_line(tokens);
    }
    else if (command == "rotate") {
        rotate_line(tokens);
    }
    else if (command == "help") {
        print_help();
    }
    else if (command == "del") {
        if (tokens.size() > 1 && tokens[1] == "line") {
            delete_line(tokens);
        } else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }
    else {
        std::cout << "Unknown command. Type 'help' for available commands.\n";
    }
}

void CommandInterface::print_help() const {
    std::cout << "Available commands:\n"
         << "1. create line - create new polyline\n"
         << "2. add point <line_index> <x y z> <point_name> - add point to polyline\n"
         << "3. merge <line_index1> <line_index2> - merge two polylines\n"
         << "4. render - render all polylines\n"
         << "5. get length <line_index> - get polyline length\n"
         << "6. shift <line_index> <x y z> - shift polyline by vector\n"
         << "7. rotate <line_index> <x y z> <angle_deg> - rotate polyline around axis\n"
         << "8. help - show this help\n"
         << "9. get lines - show all polylines\n"
         << "10. del line <line_index> - delete polyline\n"
         << "11. exit - exit program\n";
}

void CommandInterface::create_line() {
    lines.emplace_back();
    std::cout << "Created new line with index: " << lines.size() - 1 << std::endl;
}

void CommandInterface::add_point(const std::vector<std::string>& tokens) {
    try {
        if (tokens.size() < 7) {
            std::cout << "Error: Not enough arguments\n";
            return;
        }
        
        size_t index = std::stoul(tokens[2]);
        if (index >= lines.size()) {
            std::cout << "Error: Invalid line index\n";
            return;
        }

        double x = std::stod(tokens[3]);
        double y = std::stod(tokens[4]);
        double z = std::stod(tokens[5]);
        char name = tokens[6][0];

        Point<double, 3> point = {x, y, z};
        lines[index].add_point(point, name);
        std::cout << "Point added to line " << index << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid arguments\n";
    }
}

void CommandInterface::merge_lines(const std::vector<std::string>& tokens) {
    try {
        if (tokens.size() < 3) {
            std::cout << "Error: Not enough arguments\n";
            return;
        }
        
        size_t idx1 = std::stoul(tokens[1]);
        size_t idx2 = std::stoul(tokens[2]);
        
        if (idx1 >= lines.size() || idx2 >= lines.size()) {
            std::cout << "Error: Invalid line index\n";
            return;
        }

        lines[idx1].merge_line(std::move(lines[idx2]));
        lines.erase(lines.begin() + idx2);
        std::cout << "Lines merged. Line " << idx2 << " removed.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid arguments\n";
    }
}

void CommandInterface::render() {
    SpatialBuffer<double, BUFFER_WIDTH, BUFFER_HEIGHT> buffer(projection_matrix);
    buffer.clear();
    
    for (auto& line : lines) {
        buffer << line;
    }
    
    std::cout << buffer << std::endl;
}

void CommandInterface::get_length(const std::vector<std::string>& tokens) const {
    try {
        if (tokens.size() < 3) {
            std::cout << "Error: Not enough arguments\n";
            return;
        }
        
        size_t index = std::stoul(tokens[2]);
        if (index >= lines.size()) {
            std::cout << "Error: Invalid line index\n";
            return;
        }
        
        std::cout << "Length of line " << index << ": " << lines[index].length() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid arguments\n";
    }
}

void CommandInterface::shift_line(const std::vector<std::string>& tokens) {
    try {
        if (tokens.size() < 5) {
            std::cout << "Error: Not enough arguments\n";
            return;
        }
        
        size_t index = std::stoul(tokens[1]);
        if (index >= lines.size()) {
            std::cout << "Error: Invalid line index\n";
            return;
        }

        double x = std::stod(tokens[2]);
        double y = std::stod(tokens[3]);
        double z = std::stod(tokens[4]);
        
        Vector<double, 3> shift_vec = {x, y, z};
        lines[index].shift(shift_vec);
        std::cout << "Line " << index << " shifted\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid arguments\n";
    }
}

void CommandInterface::rotate_line(const std::vector<std::string>& tokens) {
    try {
        if (tokens.size() < 6) {
            std::cout << "Error: Not enough arguments\n";
            return;
        }
        
        size_t index = std::stoul(tokens[1]);
        if (index >= lines.size()) {
            std::cout << "Error: Invalid line index\n";
            return;
        }

        double x = std::stod(tokens[2]);
        double y = std::stod(tokens[3]);
        double z = std::stod(tokens[4]);
        double angle_deg = std::stod(tokens[5]);
        
        Vector<double, 3> axis = {x, y, z};
        double angle_rad = angle_deg * M_PI / 180.0;
        
        lines[index].rotate(axis, angle_rad);
        std::cout << "Line " << index << " rotated\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid arguments\n";
    }
}

void CommandInterface::get_lines() const {
    std::cout << "Total lines: " << lines.size() << std::endl;
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << "Line " << i << " (points: " << lines[i].get_size() << "): ";
        for (size_t j = 0; j < lines[i].get_size(); ++j) {
            auto point = lines[i].begin()[j];
            std::cout << lines[i].get_point_name(j) << "(" 
                 << point.at(0, 0) << ", "
                 << point.at(0, 1) << ", "
                 << point.at(0, 2) << ") ";
        }
        std::cout << std::endl;
    }
}

void CommandInterface::delete_line(const std::vector<std::string>& tokens) {
    try {
        if (tokens.size() < 3) {
            std::cout << "Error: Not enough arguments\n";
            return;
        }
        
        size_t index = std::stoul(tokens[2]);
        if (index >= lines.size()) {
            std::cout << "Error: Invalid line index\n";
            return;
        }
        
        lines.erase(lines.begin() + index);
        std::cout << "Line " << index << " deleted\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid arguments\n";
    }
}
