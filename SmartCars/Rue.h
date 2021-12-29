#ifndef RUE_H
#define RUE_H

#include "Noeud.h"

#include <iostream>
#include <vector>

class Rue {
public:
    // Constructeurs
    Rue(std::string reference, std::string type, Noeud* noeud, int nombrenoeuds) : v_reference{ reference }, v_type{ type }, v_noeuds{ std::vector<Noeud*>(nombrenoeuds, noeud) } {};
    // D�structeurs
    ~Rue() = default;
    // Accesseurs
    std::vector<Noeud*> noeuds() const {
        return v_noeuds;
    }
    std::string reference() const {
        return v_reference;
    }
    // M�thodes
    int nombrenoeuds() const;
    void ajouteNoeud(Noeud* n, int indicenoeud);
    void creerLiens();
private:
    // Attributs
    std::string v_reference;
    std::string v_type;
    std::vector<Noeud*> v_noeuds;
};

#endif
