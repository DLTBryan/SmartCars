#ifndef RUE_H
#define RUE_H

#include "noeud.h"

#include <vector>
#include <iostream>

class Rue {
public:
    // Constructeurs
    Rue(std::string reference, Noeud* noeud) : v_reference{ reference }, v_noeuds{ {noeud} } {};
    // Accesseurs
    std::vector<Noeud*> noeuds() const {
        return v_noeuds;
    }
    // Méthodes
    std::string reference() const {
        return v_reference;
    }
    int nombrenoeuds() const {
        return (int)v_noeuds.size();
    }
    void ajouteNoeud(Noeud* noeud);
private:
    // Attributs
    std::string v_reference;
    std::vector<Noeud*> v_noeuds;
};


#endif
