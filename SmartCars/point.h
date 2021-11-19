#ifndef POINT_H
#define POINT_H

class Point {
    friend class Noeud;
    public:
        // Constructeurs
        Point() {};
        Point(float x, float y) : v_x{ x }, v_y{ y } {}

        float x() const { return v_x; }
        float y() const { return v_y; }

    private:
        // Attributs
        float v_x, v_y;
};

#endif
