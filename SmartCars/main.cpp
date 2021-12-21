#include "SmartCars.h"

#include <QtWidgets/QApplication>

#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    auto start = high_resolution_clock::now();
    osmextract extracteur("map.osm");
    extracteur.extraire();
    std::vector<Rue> rues = extracteur.rues();
    std::cout << "Size rues : " << rues.size() << std::endl;
    std::vector<Noeud*> noeuds = extracteur.noeuds();
    std::cout << "Size noeuds : " << noeuds.size() << std::endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Temps d'ex�cution : " << duration.count() << " !" << endl;
    
    QApplication a(argc, argv);
    
    SmartCars w(noeuds);
    w.show();
    return a.exec();
}
