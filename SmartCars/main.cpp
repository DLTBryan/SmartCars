#include "SmartCars.h"

#include <QtWidgets/QApplication>
#include "tinyxml2.h"

#include "voiture.h"

#include "rue.h"
#include "OsmProcessor.h"
#include "TestOsmProcessor.h"
#include <chrono>

#include <vector>
#include <map>
#include <iostream>
#include <conio.h>

using namespace tinyxml2;
using namespace std;
using namespace std::chrono;

#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

int main(int argc, char* argv[]) {
    auto start = high_resolution_clock::now();
    OsmProcessor* osm = new OsmProcessor("map.osm");
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "------- " << duration.count() << " ------------" << endl;
    //TestOsmProcessor testOsm(osm);
    //testOsm.testParsingOsm();
    //testOsm.tracePlan();
    Voiture a = Voiture("1", 2, osm->ways().at(0)->noeuds().at(0));
    cout << "-- Debut de la simulation --" << endl
        << "Touches : " << endl
        << "  'space' : avancer" << endl 
        << "  '+' : augmenter la vitesse" << endl 
        << "  '-' : baisser la vitesse" << endl
        << "  autre : quitter" << endl << endl;
    cout << endl << "-- Coordonnees de depart --" << endl;
    a.affichage();
    int vitesse = 1;
    while (1) {
        char input = getch();
        if (input == ' ') {
            cout << endl << "Avancer de 1. Vitesse : " << vitesse << endl;
            a.avancer(vitesse);
            a.affichage();
        }
        else if (input == '+') {
            cout << endl << "Vitesse de simulation : " << ++vitesse << endl;
        }
        else if (input == '-') {
            vitesse--;
            if (vitesse < 1) vitesse = 1;
            cout << endl << "Vitesse de simulation : " << vitesse << endl;

        }
        else break;
    }
    cout << endl << "-- Coordonnees finales --" << endl;
    a.affichage();
    cout << endl << "-- Fin de la simulation --" << endl;
    return 0;
    // Je gette tous les noeuds
    vector<Noeud*> noeuds;
    for (Rue* rue : osm->ways()) {
        for (Noeud* noeud : rue->noeuds())
            noeuds.push_back(noeud);
    }
    cout << noeuds.size() << endl;
    QApplication a(argc, argv);
    SmartCars w(noeuds);
    //w.drawLine(10, 10, 110, 110);
    w.show();
    return a.exec();
}
