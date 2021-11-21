#ifndef POINT_H
#define POINT_H


#include <iostream>
#include <string>

using namespace std;

class Point {
    friend class Noeud;
    public:
        // Constructeurs
        Point(float x, float y) : d_x{ x }, d_y{ y } {}

        friend ostream& operator<<(ostream& output, const Point& point) {
            output << "Coordinates of the point : " << endl;
            output << "x = " << point.d_x << " | y = " << point.d_y << endl;
            return output;
        }

    private:
        // Attributs
        float d_x, d_y;
};

#endif
