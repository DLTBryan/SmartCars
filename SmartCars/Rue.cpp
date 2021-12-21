#include "Rue.h"

int Rue::nombrenoeuds() const {
    return (int)v_noeuds.size();
}

void Rue::ajouteNoeud(Noeud* n, int indicenoeud) {
    v_noeuds[indicenoeud] = n;
}

void Rue::creerLiens() {
    for (size_t i = 0; i < v_noeuds.size() - 1; i++) {
        v_noeuds[i]->ajouteVoisin(v_noeuds[i + 1]);
        v_noeuds[i + 1]->ajouteVoisin(v_noeuds[i]);
    }
}
