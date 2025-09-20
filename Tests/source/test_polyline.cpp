#include <gtest/gtest.h>
#include "Geometry/Polyline.hpp"

// Тесты конструкторов
TEST(PolylineTest, DefaultConstructor) {
    Polyline<double> polyline;
    EXPECT_EQ(polyline.get_size(), 0);
}

TEST(PolylineTest, CopyConstructor) {
    Polyline<double> polyline1;
    polyline1.add_point({0, 0, 0}, 'A');
    polyline1.add_point({1, 1, 1}, 'B');
    
    Polyline<double> polyline2(polyline1);
    
    EXPECT_EQ(polyline2.get_size(), 2);
    EXPECT_EQ(polyline2.get_point_name(0), 'A');
    EXPECT_EQ(polyline2.get_point_name(1), 'B');
}

TEST(PolylineTest, MoveConstructor) {
    Polyline<double> polyline1;
    polyline1.add_point({0, 0, 0}, 'A');
    polyline1.add_point({1, 1, 1}, 'B');
    
    Polyline<double> polyline2(std::move(polyline1));
    
    EXPECT_EQ(polyline2.get_size(), 2);
    EXPECT_EQ(polyline2.get_point_name(0), 'A');
    EXPECT_EQ(polyline2.get_point_name(1), 'B');
    EXPECT_EQ(polyline1.get_size(), 0); // После перемещения исходный объект пуст
}

// Тесты добавления точек (с проверкой resize)
TEST(PolylineTest, AddPointWithResize) {
    Polyline<double> polyline;
    
    // Добавим больше точек, чем начальная емкость (5)
    for (int i = 0; i < 10; ++i) {
        polyline.add_point({static_cast<double>(i), static_cast<double>(i), static_cast<double>(i)}, 'A' + i);
    }
    
    EXPECT_EQ(polyline.get_size(), 10);
    
    // Проверим, что все точки добавлены корректно
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(polyline.get_point_name(i), 'A' + i);
        EXPECT_DOUBLE_EQ(polyline.begin()[i].at(0, 0), i);
        EXPECT_DOUBLE_EQ(polyline.begin()[i].at(0, 1), i);
        EXPECT_DOUBLE_EQ(polyline.begin()[i].at(0, 2), i);
    }
}

// Тесты операций с rvalue
TEST(PolylineTest, MoveAssignment) {
    Polyline<double> polyline1;
    polyline1.add_point({0, 0, 0}, 'A');
    polyline1.add_point({1, 1, 1}, 'B');
    
    Polyline<double> polyline2;
    polyline2 = std::move(polyline1);
    
    EXPECT_EQ(polyline2.get_size(), 2);
    EXPECT_EQ(polyline2.get_point_name(0), 'A');
    EXPECT_EQ(polyline2.get_point_name(1), 'B');
    EXPECT_EQ(polyline1.get_size(), 0); // После перемещения исходный объект пуст
}

// Тесты соединения ломанных линий
TEST(PolylineTest, MergeLines) {
    Polyline<double> polyline1;
    polyline1.add_point({0, 0, 0}, 'A');
    polyline1.add_point({1, 1, 1}, 'B');
    
    Polyline<double> polyline2;
    polyline2.add_point({2, 2, 2}, 'C');
    polyline2.add_point({3, 3, 3}, 'D');
    
    polyline1.merge_line(polyline2);
    
    EXPECT_EQ(polyline1.get_size(), 4);
    EXPECT_EQ(polyline1.get_point_name(0), 'A');
    EXPECT_EQ(polyline1.get_point_name(1), 'B');
    EXPECT_EQ(polyline1.get_point_name(2), 'C');
    EXPECT_EQ(polyline1.get_point_name(3), 'D');
}

TEST(PolylineTest, MergeLinesRealloc) {
    Polyline<double> polyline1;
    polyline1.add_point({0, 0, 0}, 'A');
    polyline1.add_point({1, 1, 1}, 'B');
    polyline1.add_point({2, 2, 12}, 'C');
    
    Polyline<double> polyline2;
    polyline2.add_point({2, 2, 2}, 'D');
    polyline2.add_point({3, 3, 3}, 'E');
    polyline2.add_point({4, 4, 4}, 'F');
    
    polyline1.merge_line(polyline2);
    EXPECT_EQ(polyline1.get_size(), 6);
    EXPECT_EQ(polyline1.get_point_name(0), 'A');
    EXPECT_EQ(polyline1.get_point_name(1), 'B');
    EXPECT_EQ(polyline1.get_point_name(2), 'C');
    EXPECT_EQ(polyline1.get_point_name(3), 'D');
    EXPECT_EQ(polyline1.get_point_name(4), 'E');
    EXPECT_EQ(polyline1.get_point_name(5), 'F');
}
TEST(PolylineTest, MergeMoveLinesInFirst) {
    Polyline<double> polyline1;
    polyline1.add_point({0, 0, 0}, 'A');
    polyline1.add_point({1, 1, 1}, 'B');
    
    Polyline<double> polyline2;
    polyline2.add_point({2, 2, 2}, 'C');
    polyline2.add_point({3, 3, 3}, 'D');
    
    polyline1.merge_line(std::move(polyline2));
    
    EXPECT_EQ(polyline1.get_size(), 4);
    EXPECT_EQ(polyline1.get_point_name(0), 'A');
    EXPECT_EQ(polyline1.get_point_name(1), 'B');
    EXPECT_EQ(polyline1.get_point_name(2), 'C');
    EXPECT_EQ(polyline1.get_point_name(3), 'D');
}

TEST(PolylineTest, MergeMoveLinesInSecond) {
    Polyline<double> polyline1;
    polyline1.add_point({0, 0, 0}, 'A');
    polyline1.add_point({1, 1, 1}, 'B');
    polyline1.add_point({2, 2, 12}, 'C');
    
    Polyline<double> polyline2;
    polyline2.add_point({2, 2, 2}, 'D');
    polyline2.add_point({3, 3, 3}, 'E');
    polyline2.add_point({4, 4, 4}, 'F');
    polyline2.add_point({5, 5, 5}, 'G');
    polyline2.add_point({6, 6, 6}, 'H');
    polyline2.add_point({70, 70, 70}, 'I');

    polyline2.remove_most_isolated_point(); //del I
    
    polyline1.merge_line(std::move(polyline2));
    
    EXPECT_EQ(polyline1.get_size(), 8);
    EXPECT_EQ(polyline1.get_point_name(0), 'A');
    EXPECT_EQ(polyline1.get_point_name(1), 'B');
    EXPECT_EQ(polyline1.get_point_name(2), 'C');
    EXPECT_EQ(polyline1.get_point_name(3), 'D');
    EXPECT_EQ(polyline1.get_point_name(4), 'E');
    EXPECT_EQ(polyline1.get_point_name(5), 'F');
    EXPECT_EQ(polyline1.get_point_name(6), 'G');
    EXPECT_EQ(polyline1.get_point_name(7), 'H');
}

TEST(PolylineTest, MergeMoveLinesRealloc) {
    Polyline<double> polyline1;
    polyline1.add_point({0, 0, 0}, 'A');
    polyline1.add_point({1, 1, 1}, 'B');
    polyline1.add_point({2, 2, 12}, 'C');
    
    Polyline<double> polyline2;
    polyline2.add_point({2, 2, 2}, 'D');
    polyline2.add_point({3, 3, 3}, 'E');
    polyline2.add_point({4, 4, 4}, 'F');
    
    polyline1.merge_line(std::move(polyline2));
    
    EXPECT_EQ(polyline1.get_size(), 6);
    EXPECT_EQ(polyline1.get_point_name(0), 'A');
    EXPECT_EQ(polyline1.get_point_name(1), 'B');
    EXPECT_EQ(polyline1.get_point_name(2), 'C');
    EXPECT_EQ(polyline1.get_point_name(3), 'D');
    EXPECT_EQ(polyline1.get_point_name(4), 'E');
    EXPECT_EQ(polyline1.get_point_name(5), 'F');
}

// Тесты вычисления длины
TEST(PolylineTest, LengthCalculation) {
    Polyline<double> polyline;
    polyline.add_point({0, 0, 0}, 'A');
    polyline.add_point({1, 0, 0}, 'B'); // Расстояние = 1
    polyline.add_point({1, 1, 0}, 'C'); // Расстояние = 1
    
    EXPECT_DOUBLE_EQ(polyline.length(), 2.0);
}

// Тесты преобразований
TEST(PolylineTest, Shift) {
    Polyline<double> polyline;
    polyline.add_point({0, 0, 0}, 'A');
    polyline.add_point({1, 1, 1}, 'B');
    
    Vector<double, 3> shift_vec{1, 2, 3};
    polyline.shift(shift_vec);
    
    EXPECT_DOUBLE_EQ(polyline.begin()[0].at(0, 0), 1);
    EXPECT_DOUBLE_EQ(polyline.begin()[0].at(0, 1), 2);
    EXPECT_DOUBLE_EQ(polyline.begin()[0].at(0, 2), 3);
    
    EXPECT_DOUBLE_EQ(polyline.begin()[1].at(0, 0), 2);
    EXPECT_DOUBLE_EQ(polyline.begin()[1].at(0, 1), 3);
    EXPECT_DOUBLE_EQ(polyline.begin()[1].at(0, 2), 4);
}

// Тесты удаления точек
TEST(PolylineTest, RemoveMostIsolatedPoint) {
    Polyline<double> polyline;
    polyline.add_point({0, 0, 0}, 'A');
    polyline.add_point({1, 0, 0}, 'B'); 
    polyline.add_point({3, 0, 0}, 'C'); //изолированная
    
    polyline.remove_most_isolated_point();
    
    EXPECT_EQ(polyline.get_size(), 2);
    EXPECT_EQ(polyline.get_point_name(0), 'A');
    EXPECT_EQ(polyline.get_point_name(1), 'B');
}
