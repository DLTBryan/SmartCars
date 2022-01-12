#include "SmartCars.h"
#include "Voiture.h"
#include "Application_SC.h"

#include <QtWidgets/QApplication>

using namespace std;

int main(int argc, char* argv[]) {
    osmextract extracteur("map_test.osm");
    extracteur.extraire();
    
    vector<Rue> rues = extracteur.rues();

    // Generation de toutes les voitures
    vector<Voiture*> voitures;
    for (int i = 0; i < 100; i++) {
        int rue = 0 + rand() % (rues.size() - 0);
        int noeud = 0 + rand() % (rues.at(rue).noeuds().size() - 0);
        int vitesse = 10 + rand() % (30 - 10) + 1;
        voitures.push_back(new Voiture("Voiture " + i, vitesse, rues.at(rue).noeuds().at(noeud)));
    }
    
    QApplication a(argc, argv);

    Application_SC* app = new Application_SC(rues, voitures);

    app->show();
    return a.exec();
}