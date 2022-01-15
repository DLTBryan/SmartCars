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
    std::vector<Noeud*> noeudsVoisins() const {
        return v_noeudsvoisins;
    }
    double x() const {
        return v_coordonnees.v_x;
    }
    double y() const {
        return v_coordonnees.v_y;
    }
    Point coordonnees() const {
        return v_coordonnees;
    }
    bool estVoiturable() const {
        return v_voiturable;
    }
    // Méthodes
    void ajouteVoisin(Noeud* v);
    void estVoiturable(bool voiturable);
    std::vector<Noeud*> noeudsVoisinsVoiturables() const;
private:
    // Attributs
    Point v_coordonnees;
    std::vector<Noeud*> v_noeudsvoisins;
    bool v_voiturable = false;
};

#endif
