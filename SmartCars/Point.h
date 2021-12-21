#ifndef POINT_H
#define POINT_H

class Point {
    friend class Noeud;
public:
    // Constructeurs
    Point(double x, double y) : v_x{ x }, v_y{ y } {}
    // Déstructeurs
    ~Point() = default;
private:
    // Attributs
    double v_x, v_y;
};

#endif
