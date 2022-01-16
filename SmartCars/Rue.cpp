#include "Rue.h"

int Rue::nombreNoeuds() const {
    return (int)v_noeuds.size();
}

void Rue::ajouteNoeud(Noeud* n, int indicenoeud) {
    v_noeuds[indicenoeud] = n;
}

void Rue::creerLiens() {
    for (size_t i = 0; i < v_noeuds.size() - 1; i++) {
        v_noeuds[i]->ajouteVoisin(v_noeuds[i + 1]);
        v_noeuds[i + 1]->ajouteVoisin(v_noeuds[i]);
        if (estVoiturable()) {
            v_noeuds[i]->ajouteVoisinVoiturable(v_noeuds[i + 1]);
            v_noeuds[i + 1]->ajouteVoisinVoiturable(v_noeuds[i]);
        }
    }
}

bool Rue::estVoiturable() const {
    std::string stringsattributvpermis[] = { "motorway", "motorway_link", "trunk", "trunk_link", "primary", "primary_link", "secondary", "secondary_link", "tertiary", "tertiary_link", "unclassified", "residential", "service", "living_street" };
    if (std::find(std::begin(stringsattributvpermis), std::end(stringsattributvpermis), v_type) != std::end(stringsattributvpermis))
        return true;
    return false;
}
