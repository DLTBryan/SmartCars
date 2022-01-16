#include "Noeud.h"

void Noeud::ajouteVoisin(Noeud* v) {
    v_noeudsvoisins.push_back(v);
}

void Noeud::ajouteVoisinVoiturable(Noeud* v) {
    v_noeudsvoisinsvoiturables.push_back(v);
}

bool Noeud::estVoiturable() const {
    if (v_noeudsvoisinsvoiturables.size() > 0)
        return true;
    return false;
}
