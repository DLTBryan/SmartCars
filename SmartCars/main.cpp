#include "SmartCars.h"

#include <QtWidgets/QApplication>
#include "tinyxml2.h"

#include <map>
#include <vector>

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
    XMLDocument map;
    XMLError eResult = map.LoadFile("map.osm");
    XMLCheckResult(eResult);

    // Une map associant un élément "way" à des éléments "node"
    std::map<XMLElement*, std::vector<std::string>> links;

    // L'élément principal
    XMLNode* root = map.FirstChildElement("osm");
    if (root == nullptr)
        return XML_ERROR_FILE_READ_ERROR;

    // Premier parcours de tous les éléments "way"
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
                    XMLPrinter printer;
                    wayactuel->Accept(&printer);
                    std::cout << printer.CStr() << std::endl;
                    // On stocke les attributs "ref" de tous les "nd" dans une map
                    std::vector<std::string> stringsattributsrefnds;
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
                    links.insert(std::pair<XMLElement*, std::vector<std::string>>(wayactuel, stringsattributsrefnds));
                    // On affiche le nombre des attributs "ref" stockés pour chaque élément "way"
                    if (links.find(wayactuel) != links.end()) {
                        for (auto i = links.begin(); i != links.end(); ++i)
                            if (i->first == wayactuel)
                                std::cout << i->second.size() << std::endl;
                    }
                }
            }
            tagactuel = tagactuel->NextSiblingElement("tag");
        }
        wayactuel = wayactuel->NextSiblingElement("way");
    }

    return XML_SUCCESS;
}
