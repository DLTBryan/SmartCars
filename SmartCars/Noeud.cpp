#include "Noeud.h"

void Noeud::ajouteVoisin(Noeud* v) {
    v_noeudsvoisins.push_back(v);
}

void Noeud::estVoiturable(bool voiturable) {
    v_voiturable = voiturable;
}

std::vector<Noeud*> Noeud::noeudsVoisinsVoiturables() const {
    std::vector<Noeud*> noeudsvoisinsvoiturables;
    for (Noeud* noeudvoisin : v_noeudsvoisins)
        if (noeudvoisin->estVoiturable())
            noeudsvoisinsvoiturables.push_back(noeudvoisin);
    return noeudsvoisinsvoiturables;
}
