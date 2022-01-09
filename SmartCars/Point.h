#ifndef POINT_H
#define POINT_H

class Point {
    friend class Noeud;
public:
    // Constructeurs
    Point(double x, double y) : v_x{ x }, v_y{ y } {}
    // Déstructeurs
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
