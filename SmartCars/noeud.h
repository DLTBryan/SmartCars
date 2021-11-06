#ifndef NOEUD_H
#define NOEUD_H

#include "point.h"

#include <vector>

class Noeud {
    friend class Rue;
    private:
        // Construit un noeud avec les coordonn�es de p
        Noeud(const Point &p) : v_coordonnees(p) {};
        // Destructeur de noeud priv�
        ~Noeud() = default;
        // M�thodes
        void ajouteVoisin(Noeud* v);
        // Attributs
        Point v_coordonnees;
        std::vector<Noeud*> v_noeudvoisins;
};

#endif
