#include "SmartCars.h"

#include <QtWidgets/QApplication>
#include "tinyxml2.h"

#include "rue.h"

#include <vector>
#include <map>
#include <iostream>

using namespace tinyxml2;

#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

int main(int argc, char *argv[]) {
    //QApplication a(argc, argv);
    //SmartCars w;
    //w.show();
    //return a.exec();
    // https://overpass-api.de/api/map?bbox=7.30370,47.72729,7.31900,47.73389
    XMLDocument map;
    XMLError eResult = map.LoadFile("map.osm");
    XMLCheckResult(eResult);

    // Une map associant un élément "way" à des références de "node"
    std::map<XMLElement*, std::vector<std::string>> linkswayrefnode;
    // Une map associant une référence de "node" à des éléments "way"
    std::map<std::string, std::vector<XMLElement*>> linksrefnodeway;

    // L'élément principal
    XMLNode* root = map.FirstChildElement("osm");
    if (root == nullptr)
        return XML_ERROR_FILE_READ_ERROR;

    // Premier parcours de tous les éléments "way" pour savoir quels sont les "node" concernés et auquel élément "way" ils appartiennent
    XMLElement* wayactuel = root->FirstChildElement("way");
    if (wayactuel == nullptr)
        return XML_ERROR_PARSING_ELEMENT;
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
                if (stringattributktagactuel == "highway" && stringattributvtagactuel == "primary") {
                    /*XMLPrinter printer;
                    wayactuel->Accept(&printer);
                    std::cout << printer.CStr() << std::endl;*/
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
                    linkswayrefnode.insert(std::pair<XMLElement*, std::vector<std::string>>(wayactuel, stringsattributsrefnds));
                }
            }
            tagactuel = tagactuel->NextSiblingElement("tag");
        }
        wayactuel = wayactuel->NextSiblingElement("way");
    }

    // On inverse la map pour savoir quels "node" sont associés auxquels "way"
    for (auto i = linkswayrefnode.begin(); i != linkswayrefnode.end(); ++i) {
        for (std::string refnode : i->second) {
            // On vérifie si la clé existe déjà dans la nouvelle map
            if (linksrefnodeway.find(refnode) != linksrefnodeway.end()) {
                for (auto j = linksrefnodeway.begin(); j != linksrefnodeway.end(); ++j)
                    if (j->first == refnode)
                        j->second.push_back(i->first);
            }
            else
                linksrefnodeway.insert(std::pair<std::string, std::vector<XMLElement*>>(refnode, { i->first }));
        }
    }

    // On affiche le nombre des éléments "way" stockés pour chaque attribut "ref" de "nd"
    for (auto i = linksrefnodeway.begin(); i != linksrefnodeway.end(); ++i)
        std::cout << i->first << " : " << i->second.size() << std::endl;

    // "node" par "node" on commence à créer nos objets
    std::vector<Rue> rues;
    for (auto i = linksrefnodeway.begin(); i != linksrefnodeway.end(); ++i) {
        // On retrouve le "node" correspondant avec l'attribut "id"
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
                        std::cout << i->first << " : " << stringattributlatnodeactuel << ", " << stringattributlonnodeactuel << std::endl;
                        // On crée un nouveau noeud dans la mémoire
                        Noeud* nouveaunoeud = new Noeud(std::stof(stringattributlatnodeactuel), std::stof(stringattributlonnodeactuel));
                        // On rajoute ce nouveau noeud dans chaque rue auxquelle il appartient
                        for (XMLElement* wayactuel : i->second) {
                            // On récupère l'attribut "id" de "way"
                            const char* attributidwayactuel = nullptr;
                            attributidwayactuel = wayactuel->Attribute("id");
                            if (attributidwayactuel != nullptr) {
                                std::string stringattributidwayactuel = attributidwayactuel;
                                std::cout << i->first << " : " << stringattributidwayactuel << std::endl;
                                // Si la "way" existe déjà on rajoute juste le "node"
                                int indicerueexistante = -1;
                                for (size_t j = 0; j < rues.size(); j++) {
                                    if (rues[j].reference() == stringattributidwayactuel)
                                        indicerueexistante = j;
                                }
                                /* TODO : gérer les liens entre les rues (quand le noeud est attribuée à plus d'une rue) */
                                if (indicerueexistante != -1)
                                    rues[indicerueexistante].ajouteNoeud(nouveaunoeud);
                                else
                                    rues.push_back(Rue(stringattributidwayactuel, nouveaunoeud));
                            }
                        }
                    }
                }
            }
            nodeactuel = nodeactuel->NextSiblingElement("node");
        }
    }

    // J'affiche les routes récupérées
    std::cout << std::endl << "### RUES ###" << std::endl;
    for (Rue rue : rues) {
        std::cout << rue.reference() << " avec les nodes :" << std::endl;
        for (Noeud* noeud : rue.noeuds())
            std::cout << noeud->x() << ", " << noeud->y() << std::endl;
    }

    return XML_SUCCESS;
}
