#ifndef NOEUD_H
#define NOEUD_H

#include "point.h"

#include <vector>

class Noeud {
    friend class Rue;
    private:
        // Construit un noeud avec les coordonnées de p
        Noeud(const Point &p) : v_coordonnees(p) {};
        // Destructeur de noeud privé
        ~Noeud() = default;
        // Méthodes
        void ajouteVoisin(Noeud* v);
        // Attributs
        Point v_coordonnees;
        std::vector<Noeud*> v_noeudvoisins;
};

#endif
