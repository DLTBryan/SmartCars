#ifndef RUE_H
#define RUE_H

#include "noeud.h"

#include <vector>
#include <iostream>

class Rue {
    public:
        // Constructeurs
        Rue(std::string reference) : v_reference{reference} {};
        // Accesseurs
        std::vector<Noeud*> noeuds() const {
            return v_noeuds;
        }
        // Méthodes
        int nombrenoeuds() const {
            return (int) v_noeuds.size();
        }
    private:
        // Coordonnées
        std::vector<Noeud*> v_noeuds;
        std::string v_reference;
};


#endif
