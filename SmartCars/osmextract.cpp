#include "osmextract.h"

#include <string>

// Extrait les r�f�rences des noeuds constituant la route
std::vector<std::string> osmextract::extraireNdsRoute(XMLElement* wayactuel) {
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
    return stringsattributsrefnds;
}

// Renvoie une paire associant une route � ses noeuds
std::pair<std::string, std::vector<std::string>> osmextract::construirePaireRouteNds(XMLElement* wayactuel, std::vector<std::string> stringsattributsrefnds) {
    // On stocke les attributs "ref" de tous les "nd" dans une map
    const char* attributidwayactuel = nullptr;
    attributidwayactuel = wayactuel->Attribute("id");
    if (attributidwayactuel != nullptr) {
        std::string stringattributidwayactuel = attributidwayactuel;
        return std::pair<std::string, std::vector<std::string>>(stringattributidwayactuel, stringsattributsrefnds);
    }
    return std::pair<std::string, std::vector<std::string>>();
}

// D�termine si la route est valide ou pas
bool osmextract::estRouteValide(XMLElement* wayactuel) {
    std::string stringsattributvpermis[] = { "motorway", "motorway_link", "trunk", "trunk_link", "primary", "primary_link", "secondary", "secondary_link", "tertiary", "tertiary_link", "unclassified", "residential", "service", "living_street" };
    // On parcours les "tag"
    XMLElement* tagactuel = wayactuel->FirstChildElement("tag");
    while (tagactuel != nullptr) {
        // On stocke les "way" ayant k="highway" et v appartenant au tableau d�fini au d�but de la fonction
        const char* attributktagactuel = nullptr;
        attributktagactuel = tagactuel->Attribute("k");
        const char* attributvtagactuel = nullptr;
        attributvtagactuel = tagactuel->Attribute("v");
        if (attributktagactuel != nullptr && attributvtagactuel != nullptr) {
            std::string stringattributktagactuel = attributktagactuel;
            std::string stringattributvtagactuel = attributvtagactuel;
            if (stringattributktagactuel == "highway")
                return true;
            /*if (stringattributktagactuel == "highway" && std::find(std::begin(stringsattributvpermis), std::end(stringsattributvpermis), stringattributvtagactuel) != std::end(stringsattributvpermis))
                return true;*/
        }
        tagactuel = tagactuel->NextSiblingElement("tag");
    }
    return false;
}

// Fonction renvoyant une map invers�e : un noeud sera associ� � ses routes (pour chaque route associ�e on sp�cifie son identifiant, l'emplacement du noeud associ� dans cette route et le nombre total de noeuds dans cette route)
std::map<std::string, std::vector<std::vector<double>>> osmextract::inverserMapRouteNds(std::map<std::string, std::vector<std::string>> linkswayrefnode) {
    std::map<std::string, std::vector<std::vector<double>>> linksrefnodeway;
    for (auto i = linkswayrefnode.begin(); i != linkswayrefnode.end(); ++i) {
        for (size_t j = 0; j < i->second.size(); j++) {
            // On v�rifie si la cl� existe d�j� dans la nouvelle map
            if (linksrefnodeway.find(i->second[j]) != linksrefnodeway.end()) {
                for (auto k = linksrefnodeway.begin(); k != linksrefnodeway.end(); ++k)
                    if (k->first == i->second[j]) {
                        k->second.push_back({ stod(i->first), (double)j, (double)i->second.size() });
                        break;
                    }
            }
            else
                linksrefnodeway.insert(std::pair<std::string, std::vector<std::vector<double>>>(i->second[j], {
                    {stod(i->first), (double)j, (double)i->second.size()}
                    }));
        }
    }
    return linksrefnodeway;
}

// Va r�cup�rer les coordonn�es WSG84 depuis le fichier
std::array<double, 2> osmextract::recupererCoordonneesNd(XMLNode* root, std::string refnode) {
    std::array<double, 2> coordonneeswsg84;
    // On retrouve le "node" correspondant avec l'attribut "id" qui est la r�f�rence d'un noeud
    XMLElement* nodeactuel = root->FirstChildElement("node");
    while (nodeactuel != nullptr) {
        const char* attributidnodeactuel = nullptr;
        attributidnodeactuel = nodeactuel->Attribute("id");
        if (attributidnodeactuel != nullptr) {
            std::string stringattributidnodeactuel = attributidnodeactuel;
            if (stringattributidnodeactuel == refnode) {
                // On r�cup�re les coordonn�es de cet "node"
                const char* attributlatnodeactuel = nullptr;
                attributlatnodeactuel = nodeactuel->Attribute("lat");
                const char* attributlonnodeactuel = nullptr;
                attributlonnodeactuel = nodeactuel->Attribute("lon");
                if (attributlatnodeactuel != nullptr && attributlonnodeactuel != nullptr) {
                    std::string stringattributlatnodeactuel = attributlatnodeactuel;
                    std::string stringattributlonnodeactuel = attributlonnodeactuel;
                    coordonneeswsg84[0] = stod(stringattributlatnodeactuel);
                    coordonneeswsg84[1] = stod(stringattributlonnodeactuel);
                }
                break;
            }
        }
        nodeactuel = nodeactuel->NextSiblingElement("node");
    }
    return coordonneeswsg84;
}

// Va r�cup�rer le type de la route depuis le fichier
std::string osmextract::recupererTypeRoute(XMLNode* root, std::string refroute) {
    // On retrouve le "way" correspondant avec l'attribut "id" qui est la r�f�rence d'une route
    XMLElement* wayactuel = root->FirstChildElement("way");
    while (wayactuel != nullptr) {
        const char* attributidwayactuel = nullptr;
        attributidwayactuel = wayactuel->Attribute("id");
        if (attributidwayactuel != nullptr) {
            std::string stringattributidnodeactuel = attributidwayactuel;
            if (stringattributidnodeactuel == refroute) {
                // On parcours les "tag"
                XMLElement* tagactuel = wayactuel->FirstChildElement("tag");
                while (tagactuel != nullptr) {
                    const char* attributktagactuel = nullptr;
                    attributktagactuel = tagactuel->Attribute("k");
                    const char* attributvtagactuel = nullptr;
                    attributvtagactuel = tagactuel->Attribute("v");
                    if (attributktagactuel != nullptr && attributvtagactuel != nullptr) {
                        std::string stringattributktagactuel = attributktagactuel;
                        std::string stringattributvtagactuel = attributvtagactuel;
                        if (stringattributktagactuel == "highway")
                            return stringattributvtagactuel;
                    }
                    tagactuel = tagactuel->NextSiblingElement("tag");
                }
                return "";
            }
        }
        wayactuel = wayactuel->NextSiblingElement("way");
    }
}

void osmextract::extraire() {
    XMLDocument map;
    XMLError eResult = map.LoadFile(v_nomfichier.c_str());
    XMLCheckResult(eResult);

    // Une map associant un id de "way" � des r�f�rences de "node"
    std::map<std::string, std::vector<std::string>> linkswayrefnode;
    // Une map associant une r�f�rence de "node" � des ensembles d'informations sur les "way"
    std::map<std::string, std::vector<std::vector<double>>> linksrefnodeway;

    // L'�l�ment principal (v�rification si le fichier fourni est bien en XML)
    XMLNode* root = map.FirstChildElement("osm");
    if (root == nullptr)
        exit(1);
    // On v�rifie que le fichier fournit contient des routes
    XMLElement* wayactuel = root->FirstChildElement("way");
    if (wayactuel == nullptr)
        exit(1);
    // Premier parcours de tous les �l�ments "way" pour savoir quels sont les "node" concern�s et auquel �l�ment "way" ils appartiennent
    while (wayactuel != nullptr) {
        if (estRouteValide(wayactuel)) {
            std::vector<std::string> stringsattributsrefnds = extraireNdsRoute(wayactuel);
            linkswayrefnode.insert(construirePaireRouteNds(wayactuel, stringsattributsrefnds));
        }
        wayactuel = wayactuel->NextSiblingElement("way");
    }
    // On inverse la map pour savoir quels "node" sont associ�s auxquels "way"
    linksrefnodeway = inverserMapRouteNds(linkswayrefnode);
    // "node" par "node" on commence � cr�er nos objets
    for (auto i = linksrefnodeway.begin(); i != linksrefnodeway.end(); ++i) {
        std::array<double, 2> coordonneeswsg84 = recupererCoordonneesNd(root, i->first);
        // Conversion en Lambert93
        std::array<double, 2> coordonneeslambert = wgs84::toCartesian({ 47.7334, 7.30404 }, { coordonneeswsg84[0], -coordonneeswsg84[1] });
        // On cr�e un nouveau noeud dans la m�moire et on rajoute son adresse m�moire dans le tableau v_noeuds
        Noeud* nouveaunoeud = new Noeud(coordonneeslambert[0], coordonneeslambert[1]);
        v_noeuds.push_back(nouveaunoeud);
        // On rajoute ce nouveau noeud dans chaque rue auxquelle il appartient
        for (std::vector<double> ensembleinformationsway : i->second) {
            // Recherche si la "way" existe d�j�
            int indicerueexistante = -1;
            for (size_t j = 0; j < v_rues.size(); j++) {
                if (v_rues[j].reference() == std::to_string(ensembleinformationsway[0])) {
                    indicerueexistante = j;
                    break;
                }
            }
            // Si oui, alors on y ajoute juste le noeud
            if (indicerueexistante != -1)
                v_rues[indicerueexistante].ajouteNoeud(nouveaunoeud, (int)ensembleinformationsway[1]);
            else {
                // Peut �tre mieux de r�cup�rer ces informations directement au d�but (quand validation de la route) ?
                std::string typeroute = recupererTypeRoute(root, std::to_string((int)ensembleinformationsway[0]));
                v_rues.push_back(Rue(std::to_string(ensembleinformationsway[0]), typeroute, nouveaunoeud, (int)ensembleinformationsway[2]));
            }
        }
    }
    // On rajoute des liens entre les noeuds
    for (Rue rue : v_rues)
        rue.creerLiens();
}
