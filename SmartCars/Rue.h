#ifndef RUE_H
#define RUE_H

#include "Noeud.h"

#include <iostream>
#include <vector>

class Rue {
public:
    // Constructeurs
    Rue(std::string reference, Noeud* noeud, int nombrenoeuds) : v_reference{ reference }, v_noeuds{ std::vector<Noeud*>(nombrenoeuds, noeud) } {};
    // Déstructeurs
    ~Rue() = default;
    // Accesseurs
    std::vector<Noeud*> noeuds() const {
        return v_noeuds;
    }
    std::string reference() const {
        return v_reference;
    }
    // Méthodes
    int nombrenoeuds() const;
    void ajouteNoeud(Noeud* n, int indicenoeud);
    void creerLiens();
private:
    // Attributs
    std::string v_reference;
    std::vector<Noeud*> v_noeuds;
};

#endif
