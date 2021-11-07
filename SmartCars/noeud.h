#ifndef NOEUD_H
#define NOEUD_H

#include "point.h"

#include <vector>

class Noeud {
public:
    // Constructeurs
    Noeud(float x, float y) : v_coordonnees{ Point(x, y) }, v_noeudvoisins{ {} } {}
    // D�structeurs
    ~Noeud() = default;
    // M�thodes
    float x() const {
        return v_coordonnees.v_x;
    }
    float y() const {
        return v_coordonnees.v_y;
    }
    void ajouteVoisin(Noeud* v);
private:
    // Attributs
    Point v_coordonnees;
    std::vector<Noeud*> v_noeudvoisins;
};

#endif
