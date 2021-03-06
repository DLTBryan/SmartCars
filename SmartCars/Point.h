#ifndef POINT_H
#define POINT_H

class Point {
    friend class Noeud;
public:
    // Constructeurs
    Point() : v_x{ 0 }, v_y{ 0 } {}
    Point(double x, double y) : v_x{ x }, v_y{ y } {}
    // Destructeurs
    ~Point() = default;

    double x() {
        return v_x;
    }

    double y() {
        return v_y;
    }

private:
    // Attributs
    double v_x, v_y;
};

#endif
