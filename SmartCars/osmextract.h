#ifndef OSMEXTRACT_H
#define OSMEXTRACT_H

#include "tinyxml2.h"
#include "WGS84toCartesian.hpp"

#include <map>
#include <iostream>
#include <vector>

using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif

class Point {
    friend class Noeud;
    public:
        // Constructeurs
        Point(double x, double y) : v_x{x}, v_y{y} {}
    private:
        // Attributs
        double v_x, v_y;
};

class Noeud {
    public:
        // Constructeurs
        Noeud(double x, double y) : v_coordonnees{Point(x, y)}, v_noeudsvoisins{} {}
        // Déstructeurs
        ~Noeud() = default;
        // Accesseurs
        std::vector<Noeud*> noeudsvoisins() const {
            return v_noeudsvoisins;
        }
        // Méthodes
        double x() const {
            return v_coordonnees.v_x;
        }
        double y() const {
            return v_coordonnees.v_y;
        }
        void ajouteVoisin(Noeud* v) {
            v_noeudsvoisins.push_back(v);
        }
    private:
        // Attributs
        Point v_coordonnees;
        std::vector<Noeud*> v_noeudsvoisins;
};

class Rue {
    public:
        // Constructeurs
        Rue(std::string reference, Noeud* noeud, int nombrenoeuds) : v_reference{reference}, v_noeuds{std::vector<Noeud*>(nombrenoeuds, noeud)} {};
        // Accesseurs
        std::vector<Noeud*> noeuds() const {
            return v_noeuds;
        }
        std::string reference() const {
            return v_reference;
        }
        // Méthodes
        int nombrenoeuds() const {
            return (int) v_noeuds.size();
        }
        void ajouteNoeud(Noeud* n, int indicenoeud) {
            v_noeuds[indicenoeud] = n;
        }
        void creerLiens() {
            for (size_t i = 0; i < v_noeuds.size()-1; i++) {
                v_noeuds[i]->ajouteVoisin(v_noeuds[i+1]);
                v_noeuds[i+1]->ajouteVoisin(v_noeuds[i]);
            }
        }
    private:
        // Attributs
        std::string v_reference;
        std::vector<Noeud*> v_noeuds;
};

class osmextract {
    public:
        // Constructeurs
        osmextract(std::string nomfichier) : v_nomfichier{nomfichier} {}
        // Accesseurs
        std::vector<Noeud*> noeuds() const {
            return v_noeuds;
        }
        std::vector<Rue> rues() const {
            return v_rues;
        }
        // Méthodes
        void extraire() {
            XMLDocument map;
            XMLError eResult = map.LoadFile(v_nomfichier.c_str());
            XMLCheckResult(eResult);

            // Une map associant un id de "way" à des références de "node"
            std::map<std::string, std::vector<std::string>> linkswayrefnode;
            // Une map associant une référence de "node" à des ensembles d'informations sur les "way"
            std::map<std::string, std::vector<std::vector<double>>> linksrefnodeway;

            // L'élément principal
            XMLNode* root = map.FirstChildElement("osm");
            if (root != nullptr) {
                // Premier parcours de tous les éléments "way" pour savoir quels sont les "node" concernés et auquel élément "way" ils appartiennent
                XMLElement* wayactuel = root->FirstChildElement("way");
                if (wayactuel != nullptr) {
                    while (wayactuel != nullptr) {
                        // On parcours les "tag"
                        XMLElement* tagactuel = wayactuel->FirstChildElement("tag");
                        while (tagactuel != nullptr) {
                            // On stocke les "way" ayant k="highway" et v="primary"
                            const char* attributktagactuel = nullptr;
                            attributktagactuel = tagactuel->Attribute("k");
                            const char* attributvtagactuel = nullptr;
                            attributvtagactuel = tagactuel->Attribute("v");
                            if (attributktagactuel != nullptr && attributvtagactuel != nullptr) {
                                std::string stringattributktagactuel = attributktagactuel;
                                std::string stringattributvtagactuel = attributvtagactuel;
                                if (stringattributktagactuel == "highway") {
                                    std::vector<std::string> stringsattributsrefnds;
                                    // On parcourt les "nd"
                                    XMLElement* ndactuel = wayactuel->FirstChildElement("nd");
                                    while (ndactuel != nullptr) {
                                        const char* attributrefndactuel = nullptr;
                                        attributrefndactuel = ndactuel->Attribute("ref");
                                        if (attributrefndactuel != nullptr) {
                                            std::string stringattributrefndactuel = attributrefndactuel;
                                            stringsattributsrefnds.push_back(stringattributrefndactuel);
                                        }
                                        ndactuel = ndactuel->NextSiblingElement("nd");
                                    }
                                    // On stocke les attributs "ref" de tous les "nd" dans une map
                                    const char* attributidwayactuel = nullptr;
                                    attributidwayactuel = wayactuel->Attribute("id");
                                    if (attributidwayactuel != nullptr) {
                                        std::string stringattributidwayactuel = attributidwayactuel;
                                        linkswayrefnode.insert(std::pair<std::string, std::vector<std::string>>(stringattributidwayactuel, stringsattributsrefnds));
                                    }
                                }
                            }
                            tagactuel = tagactuel->NextSiblingElement("tag");
                        }
                        wayactuel = wayactuel->NextSiblingElement("way");
                    }
                    // On inverse la map pour savoir quels "node" sont associés auxquels "way"
                    for (auto i = linkswayrefnode.begin(); i != linkswayrefnode.end(); ++i) {
                        for (size_t j = 0; j < i->second.size(); j++) {
                            // On vérifie si la clé existe déjà dans la nouvelle map
                            if (linksrefnodeway.find(i->second[j]) != linksrefnodeway.end()) {
                                for (auto k = linksrefnodeway.begin(); k != linksrefnodeway.end(); ++k)
                                    if (k->first == i->second[j]) {
                                        k->second.push_back({stod(i->first), (double) j, (double) i->second.size()});
                                        break;
                                    }
                            } else
                                linksrefnodeway.insert(std::pair<std::string, std::vector<std::vector<double>>>(i->second[j], {
                                    {stod(i->first), (double) j, (double) i->second.size()}
                                }));
                        }
                    }
                    // "node" par "node" on commence à créer nos objets
                    for (auto i = linksrefnodeway.begin(); i != linksrefnodeway.end(); ++i) {
                        // On retrouve le "node" correspondant avec l'attribut "id" qui est la référence d'un noeud
                        XMLElement* nodeactuel = root->FirstChildElement("node");
                        while (nodeactuel != nullptr) {
                            const char* attributidnodeactuel = nullptr;
                            attributidnodeactuel = nodeactuel->Attribute("id");
                            if (attributidnodeactuel != nullptr) {
                                std::string stringattributidnodeactuel = attributidnodeactuel;
                                if (stringattributidnodeactuel == i->first) {
                                    // On récupère les coordonnées de cet "node"
                                    const char* attributlatnodeactuel = nullptr;
                                    attributlatnodeactuel = nodeactuel->Attribute("lat");
                                    const char* attributlonnodeactuel = nullptr;
                                    attributlonnodeactuel = nodeactuel->Attribute("lon");
                                    if (attributlatnodeactuel != nullptr && attributlonnodeactuel != nullptr) {
                                        std::string stringattributlatnodeactuel = attributlatnodeactuel;
                                        std::string stringattributlonnodeactuel = attributlonnodeactuel;
                                        // Conversion en Lambert93
                                        std::array<double, 2> coordonneeslambert = wgs84::toCartesian({47.7334, 7.30404}, {stod(stringattributlatnodeactuel), -stod(stringattributlonnodeactuel)});
                                        // On crée un nouveau noeud dans la mémoire et on rajoute son adresse mémoire dans le tableau v_noeuds
                                        Noeud* nouveaunoeud = new Noeud(coordonneeslambert[0], coordonneeslambert[1]);
                                        v_noeuds.push_back(nouveaunoeud);
                                        // On rajoute ce nouveau noeud dans chaque rue auxquelle il appartient
                                        for (std::vector<double> ensembleinformationsway : i->second) {
                                            // Si la "way" existe déjà on rajoute juste le "node"
                                            int indicerueexistante = -1;
                                            for (size_t j = 0; j < v_rues.size(); j++) {
                                                if (v_rues[j].reference() == std::to_string(ensembleinformationsway[0])) {
                                                    indicerueexistante = j;
                                                    break;
                                                }
                                            }
                                            if (indicerueexistante != -1)
                                                v_rues[indicerueexistante].ajouteNoeud(nouveaunoeud, (int) ensembleinformationsway[1]);
                                            else
                                                v_rues.push_back(Rue(std::to_string(ensembleinformationsway[0]), nouveaunoeud, (int) ensembleinformationsway[2]));
                                        }
                                    }
                                    break;
                                }
                            }
                            nodeactuel = nodeactuel->NextSiblingElement("node");
                        }
                    }
                    // Je rajoute des liens
                    for (Rue rue : v_rues)
                        rue.creerLiens();
                }
            }
        }
    private:
        // Attributs
        std::string v_nomfichier;
        std::vector<Noeud*> v_noeuds;
        std::vector<Rue> v_rues;
};

#endif
