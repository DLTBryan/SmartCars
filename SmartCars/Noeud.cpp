#include "Noeud.h"

// Ajoute un voisin
void Noeud::ajouteVoisin(Noeud* v) {
    v_noeudsvoisins.push_back(v);
}

// Ajoute un voisin qui peut être accédé avec une voiture
void Noeud::ajouteVoisinVoiturable(Noeud* v) {
    v_noeudsvoisinsvoiturables.push_back(v);
}

// Si un noeud a des voisins "voiturables" alors il est aussi "voiturable"
bool Noeud::estVoiturable() const {
    if (v_noeudsvoisinsvoiturables.size() > 0)
        return true;
    return false;
}
