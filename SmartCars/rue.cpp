#include "rue.h"

void Rue::ajouteNoeud(Noeud* n) {
    v_noeuds[v_noeuds.size() - 1]->ajouteVoisin(n);
    v_noeuds.push_back(n);
}
