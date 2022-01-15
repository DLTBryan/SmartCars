#include "Noeud.h"

void Noeud::ajouteVoisin(Noeud* v) {
    v_noeudsvoisins.push_back(v);
}

void Noeud::estVoiturable(bool voiturable) {
    v_voiturable = voiturable;
}
