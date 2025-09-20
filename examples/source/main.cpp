#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include "Graphics/SpatialBuffer.hpp"

int main() {
    RectangularMatrix<char,2, 2> mat;
    
    auto a = mat.column_begin(0);
    auto b = mat.column_cbegin(0);    
    b = a;

    const double angle = M_PI / 4;
    const int frames = 36;
    const double rotation_step = 2 * M_PI / frames;
    const double fall_step = 40.0 / frames;

    std::cout << "=== Анимация объемной 3D звезды ===" << std::endl;

    // Упрощенная проекционная матрица для лучшего отображения
    RectangularMatrix<double, 3, 2> perspective_proj = {
        1.0, 0.0,
        0.0, 1.0,
        0.0, 0.0
    };

    SpatialBuffer<double, 120, 120> buffer(perspective_proj);
    
    // Исходные параметры звезды - УВЕЛИЧЕНО В 2 РАЗА
    const double outer_radius = 20;  // было 10
    const double inner_radius = 8;   // было 4
    const int points = 5;
    const double center_x = 0;
    const double center_y = 0;
    const double center_z = 0;

    // Создаем базовые точки звезды в локальных координатах
    std::vector<Point<double, 3>> base_points;
    std::vector<char> base_symbols;

    // Создаем вершины звезды (нижняя)
    for (int i = 0; i < points * 2; ++i) {
        double a = M_PI * i / points;
        double r = (i % 2 == 0) ? outer_radius : inner_radius;
        base_points.push_back({r * cos(a), r * sin(a), 0});
        base_symbols.push_back('A' + (i % 26));
    }
    // Замыкаем нижнюю звезду
    base_points.push_back(base_points[0]);
    base_symbols.push_back('A');

    // Верхняя звезда - тоже увеличена
    for (size_t i = 0; i < points * 2; ++i) {
        Point<double, 3> top_point = base_points[i];
        top_point.at(0, 2) = 16;  // было 8
        base_points.push_back(top_point);
        base_symbols.push_back('a' + (i % 26));
    }
    // Замыкаем верхнюю звезду
    Point<double, 3> top_first = base_points[0];
    top_first.at(0, 2) = 16;  // было 8
    base_points.push_back(top_first);
    base_symbols.push_back('a');

    // Боковые соединения (только внешние вершины)
    for (int i = 0; i < points * 2; i += 2) {
        base_points.push_back(base_points[i]);
        base_symbols.push_back(base_symbols[i]);
        Point<double, 3> top_point = base_points[i];
        top_point.at(0, 2) = 16;  // было 8
        base_points.push_back(top_point);
        base_symbols.push_back(base_symbols[i] + 32);
    }

    for (int frame = 0; frame < frames; ++frame) {
        buffer.clear();
        Polyline<double> star;
        
        double current_angle = frame * rotation_step;
        double current_fall = frame * fall_step;
        
        // Преобразование и добавление точек
        for (size_t i = 0; i < base_points.size(); ++i) {
            const auto& p = base_points[i];
            
            // Вращение вокруг оси Y
            double x = p.at(0, 0) * cos(current_angle) + p.at(0, 2) * sin(current_angle);
            double y = p.at(0, 1);
            double z = -p.at(0, 0) * sin(current_angle) + p.at(0, 2) * cos(current_angle);
            
            // Смещение в центр экрана и падение
            Point<double, 3> transformed = {
                x + 40,  // Центр по X
                y + 20 + current_fall,  // Центр по Y + падение
                z
            };
            
            star.add_point(transformed, base_symbols[i]);
        }
        
        star.remove_most_isolated_point();
        buffer << star;
        std::cout << "\033[2J\033[H"; // Очистка экрана (для Unix-like систем)
        std::cout << "Кадр " << frame + 1 << "/" << frames << std::endl;
        std::cout << buffer;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}

