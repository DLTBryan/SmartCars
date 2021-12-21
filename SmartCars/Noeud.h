#ifndef NOEUD_H
#define NOEUD_H

#include "Point.h"

#include <vector>

class Noeud {
public:
    // Constructeurs
    Noeud(double x, double y) : v_coordonnees{ Point(x, y) }, v_noeudsvoisins{} {}
    // Déstructeurs
    ~Noeud() = default;
    // Accesseurs
    std::vector<Noeud*> noeudsvoisins() const {
        return v_noeudsvoisins;
    }
    double x() const {
        return v_coordonnees.v_x;
    }
    double y() const {
        return v_coordonnees.v_y;
    }
    // Méthodes
    void ajouteVoisin(Noeud* v);
private:
    // Attributs
    Point v_coordonnees;
    std::vector<Noeud*> v_noeudsvoisins;
};

#endif
