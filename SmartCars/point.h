#ifndef POINT_H
#define POINT_H

class Point {
    public:
        // Constructeurs
        Point() : v_x{0}, v_y{0} {}
        Point(int x, int y) : v_x{x}, v_y{y} {}
        // Accesseurs
        int x() const {
            return v_x;
        }
        int y() const {
            return v_y;
        }
    private:
        // Coordonnées
        int v_x, v_y;
};

#endif
