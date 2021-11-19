#ifndef RUE_H
#define RUE_H

#include "noeud.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Rue {
public:
    // Constructeurs
    Rue(string idRue, Noeud* noeud) : d_rueId{ idRue }, d_noeuds{ {noeud} } {};
    Rue(string idRue) : d_rueId{ idRue } {};

    // Accesseurs
    vector<Noeud*> noeuds() const {
        return d_noeuds;
    }
    // Méthodes
    string id() const {
        return d_rueId;
    }
    int nombrenoeuds() const {
        return (int)d_noeuds.size();
    }

    void ajouteNoeud(Noeud* noeud);
    
    void findNoeud(string id);

    friend ostream& operator<<(ostream& output, const Rue& rue) {
        int nbVoisin = 1;
        for (Noeud* n : rue.d_noeuds) {
            
            output << "Rue id =>" << n->id() << endl;
            output << "Total number of voisins => " << n->voisins().size() << endl;
            output << "Voisins :" << endl;
            for (Noeud* nVoisin : n->voisins()) {
                output << "  Voisin " << nbVoisin << " = " << nVoisin->id() << endl;
                nbVoisin++;
            }
            output << "------------" << endl;
        }
        return output;
    }

private:
    // Attributs
    string d_rueId;
    vector<Noeud*> d_noeuds;
};


#endif
