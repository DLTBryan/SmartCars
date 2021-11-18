#ifndef NOEUD_H
#define NOEUD_H

#include "point.h"

#include <vector>
#include <string>

using namespace std;

class Noeud {
public:
   
    // Constructeurs
    Noeud(float x, float y, string id) : d_coordonnees{ Point(x, y) }, d_id{id}{}
    // Destructor
    ~Noeud() { d_noeudvoisins.clear(); d_idsOfNeighboorsOnOtherRue.clear(); };
    // M�thodes
    float x() const { return d_coordonnees.v_x; }
    
    float y() const { return d_coordonnees.v_y; }

    Point coordonnees() const { return d_coordonnees;}

    string id() const { return d_id; }

    bool isProcessed() const { return d_processed; }

    Noeud* voisin(int i) const { return d_noeudvoisins.at(i); };

    vector<Noeud*> voisins() const { return d_noeudvoisins; };

    void processed(bool process) {
        d_processed = process;
    }

    void ajouteVoisin(Noeud* v) {
        d_noeudvoisins.push_back(v);
    };


    void addIdNeighbooringNode(string id) { //
        if(!count(d_idsOfNeighboorsOnOtherRue.begin(), d_idsOfNeighboorsOnOtherRue.end(), id))
            d_idsOfNeighboorsOnOtherRue.push_back(id);
    };

    vector<string> neighboorsOnOtherRue() const {
        return d_idsOfNeighboorsOnOtherRue;
    }

    void removeNeighbooringNode(string id) {
        remove(d_idsOfNeighboorsOnOtherRue.begin(), d_idsOfNeighboorsOnOtherRue.end(), id);
    }
    

private:
    // Attributes
    Point d_coordonnees;
    string d_id;
    vector<Noeud*> d_noeudvoisins;

    //Vector containing ids of neighbooring nodes that are on other routes
    //if the node is used in other routes
    vector<string> d_idsOfNeighboorsOnOtherRue;
    bool d_processed = false;

    
};

#endif
