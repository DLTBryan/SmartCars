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
    std::vector<Noeud*> noeuds = extracteur.noeuds();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Temps d'exécution : " << duration.count() << " !" << endl;
    
    QApplication a(argc, argv);
    
    SmartCars w(noeuds);
    w.show();
    return a.exec();
}
