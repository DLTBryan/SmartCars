#ifndef POINT_H
#define POINT_H

class Point {
    friend class Noeud;
    public:
        // Constructeurs
        Point(float x, float y) : v_x{ x }, v_y{ y } {}
    private:
        // Attributs
        float v_x, v_y;
};

#endif
