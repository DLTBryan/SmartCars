#include "osmextract.h"

#include <string>

// Extrait les references des noeuds constituant la route
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

// Renvoie une paire associant une route a ses noeuds
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

// Determine si la route est valide ou pas
bool osmextract::estRouteValide(XMLElement* wayactuel) {
    // On parcours les "tag"
    XMLElement* tagactuel = wayactuel->FirstChildElement("tag");
    while (tagactuel != nullptr) {
        // On stocke les "way" ayant k="highway" et v appartenant au tableau defini au debut de la fonction
        const char* attributktagactuel = nullptr;
        attributktagactuel = tagactuel->Attribute("k");
        const char* attributvtagactuel = nullptr;
        attributvtagactuel = tagactuel->Attribute("v");
        if (attributktagactuel != nullptr && attributvtagactuel != nullptr) {
            std::string stringattributktagactuel = attributktagactuel;
            std::string stringattributvtagactuel = attributvtagactuel;
            if (stringattributktagactuel == "highway")
                return true;
        }
        tagactuel = tagactuel->NextSiblingElement("tag");
    }
    return false;
}

// Fonction renvoyant une map inversee : un noeud sera associe a ses routes (pour chaque route associee on specifie son identifiant, l'emplacement du noeud associe dans cette route et le nombre total de noeuds dans cette route)
std::map<std::string, std::vector<std::vector<double>>> osmextract::inverserMapRouteNds(std::map<std::string, std::vector<std::string>> linkswayrefnode) {
    std::map<std::string, std::vector<std::vector<double>>> linksrefnodeway;
    for (auto i = linkswayrefnode.begin(); i != linkswayrefnode.end(); ++i) {
        for (size_t j = 0; j < i->second.size(); j++) {
            // On verifie si la cle existe deja dans la nouvelle map
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

// Va recuperer les coordonnees WSG84 depuis le fichier
std::array<double, 2> osmextract::recupererCoordonneesNd(XMLNode* root, std::string refnode) {
    std::array<double, 2> coordonneeswsg84;
    // On retrouve le "node" correspondant avec l'attribut "id" qui est la reference d'un noeud
    XMLElement* nodeactuel = root->FirstChildElement("node");
    while (nodeactuel != nullptr) {
        const char* attributidnodeactuel = nullptr;
        attributidnodeactuel = nodeactuel->Attribute("id");
        if (attributidnodeactuel != nullptr) {
            std::string stringattributidnodeactuel = attributidnodeactuel;
            if (stringattributidnodeactuel == refnode) {
                // On recupère les coordonnees de cet "node"
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

// Va recuperer le type de la route depuis le fichier
std::string osmextract::recupererTypeRoute(XMLNode* root, std::string refroute) {
    // On retrouve le "way" correspondant avec l'attribut "id" qui est la reference d'une route
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

void osmextract::trierRoutesSelonType(bool descendant) {
    std::vector<std::vector<Rue>> trimanuelroutes(10, std::vector<Rue>());
    for (Rue rue : v_rues) {
        std::string typeroute = rue.type();
        if (typeroute == "motorway" || typeroute == "motorway_link")
            trimanuelroutes[0].push_back(rue);
        else if (typeroute == "trunk" || typeroute == "trunk_link")
            trimanuelroutes[1].push_back(rue);
        else if (typeroute == "primary" || typeroute == "primary_link")
            trimanuelroutes[2].push_back(rue);
        else if (typeroute == "secondary" || typeroute == "secondary_link")
            trimanuelroutes[3].push_back(rue);
        else if (typeroute == "tertiary" || typeroute == "tertiary_link")
            trimanuelroutes[4].push_back(rue);
        else if (typeroute == "unclassified")
            trimanuelroutes[5].push_back(rue);
        else if (typeroute == "residential")
            trimanuelroutes[6].push_back(rue);
        else if (typeroute == "service")
            trimanuelroutes[7].push_back(rue);
        else if (typeroute == "living_street")
            trimanuelroutes[8].push_back(rue);
        else
            trimanuelroutes[9].push_back(rue);
    }
    if (descendant) {
        for (int i = 1; i < 10; i++)
            trimanuelroutes[0].insert(std::end(trimanuelroutes[0]), std::begin(trimanuelroutes[i]), std::end(trimanuelroutes[i]));
        v_rues = trimanuelroutes[0];
    } else {
        for (int i = 8; i >= 0; i--)
            trimanuelroutes[9].insert(std::end(trimanuelroutes[9]), std::begin(trimanuelroutes[i]), std::end(trimanuelroutes[i]));
        v_rues = trimanuelroutes[9];
    }
}

void osmextract::extraire() {
    XMLDocument map;
    XMLError eResult = map.LoadFile(v_nomfichier.c_str());
    XMLCheckResult(eResult);

    // Une map associant un id de "way" a des references de "node"
    std::map<std::string, std::vector<std::string>> linkswayrefnode;
    // Une map associant une reference de "node" a des ensembles d'informations sur les "way"
    std::map<std::string, std::vector<std::vector<double>>> linksrefnodeway;

    // L'element principal (verification si le fichier fourni est bien en XML)
    XMLNode* root = map.FirstChildElement("osm");
    if (root == nullptr)
        exit(1);
    // On verifie que le fichier fournit contient des routes
    XMLElement* wayactuel = root->FirstChildElement("way");
    if (wayactuel == nullptr)
        exit(1);
    // Premier parcours de tous les elements "way" pour savoir quels sont les "node" concernes et auquel element "way" ils appartiennent
    while (wayactuel != nullptr) {
        if (estRouteValide(wayactuel)) {
            std::vector<std::string> stringsattributsrefnds = extraireNdsRoute(wayactuel);
            linkswayrefnode.insert(construirePaireRouteNds(wayactuel, stringsattributsrefnds));
        }
        wayactuel = wayactuel->NextSiblingElement("way");
    }
    // On inverse la map pour savoir quels "node" sont associes auxquels "way"
    linksrefnodeway = inverserMapRouteNds(linkswayrefnode);
    // "node" par "node" on commence a creer nos objets
    for (auto i = linksrefnodeway.begin(); i != linksrefnodeway.end(); ++i) {
        std::array<double, 2> coordonneeswsg84 = recupererCoordonneesNd(root, i->first);
        // Conversion en Lambert93
        std::array<double, 2> coordonneeslambert = wgs84::toCartesian({ 47.7334, 7.30404 }, { coordonneeswsg84[0], -coordonneeswsg84[1] });
        // On cree un nouveau noeud dans la memoire et on rajoute son adresse memoire dans le tableau v_noeuds
        Noeud* nouveaunoeud = new Noeud(coordonneeslambert[0], coordonneeslambert[1]);
        v_noeuds.push_back(nouveaunoeud);
        // On rajoute ce nouveau noeud dans chaque rue auxquelle il appartient
        for (std::vector<double> ensembleinformationsway : i->second) {
            // Recherche si la "way" existe deja
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
                // Peut etre mieux de recuperer ces informations directement au debut (quand validation de la route) ?
                std::string typeroute = recupererTypeRoute(root, std::to_string((int)ensembleinformationsway[0]));
                v_rues.push_back(Rue(std::to_string(ensembleinformationsway[0]), typeroute, nouveaunoeud, (int)ensembleinformationsway[2]));
            }
        }
    }
    // On rajoute des liens entre les noeuds
    for (Rue rue : v_rues)
        rue.creerLiens();
    // On trie les routes selon leurs types
    trierRoutesSelonType(true);
}
