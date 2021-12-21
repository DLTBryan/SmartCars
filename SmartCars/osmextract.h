#ifndef OSMEXTRACT_H
#define OSMEXTRACT_H

#include "tinyxml2.h"
#include "WGS84toCartesian.hpp"

#include "Point.h"
#include "Noeud.h"
#include "Rue.h"

#include <map>
#include <iostream>
#include <vector>

using namespace tinyxml2;

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif

class osmextract {
public:
    // Constructeurs
    osmextract(std::string nomfichier) : v_nomfichier{ nomfichier } {}
    // Déstructeurs
    ~osmextract() = default;
    // Accesseurs
    std::vector<Noeud*> noeuds() const {
        return v_noeuds;
    }
    std::vector<Rue> rues() const {
        return v_rues;
    }
    // Méthodes
    void extraire();
private:
    // Attributs
    std::string v_nomfichier;
    std::vector<Noeud*> v_noeuds;
    std::vector<Rue> v_rues;
    // Méthodes
    std::vector<std::string> extraireNdsRoute(XMLElement* wayactuel);
    std::pair<std::string, std::vector<std::string>> construirePaireRouteNds(XMLElement* wayactuel, std::vector<std::string> stringsattributsrefnds);
    bool estRouteValide(XMLElement* wayactuel);
    std::map<std::string, std::vector<std::vector<double>>> inverserMapRouteNds(std::map<std::string, std::vector<std::string>> linkswayrefnode);
    std::array<double, 2> recupererCoordonneesNd(XMLNode* root, std::string refnode);
};

#endif
