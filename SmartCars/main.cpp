#include "SmartCars.h"
#include "Voiture.h"
#include "Application.h"

#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qscrollarea.h>

#include <iostream>
#include <Windows.h>

using namespace std;
SmartCars* roads;

int main(int argc, char* argv[]) {
    osmextract extracteur("map_test.osm");
    extracteur.extraire();
    std::vector<Rue> rues = extracteur.rues();

    // Generation de toutes les voitures
    vector<Voiture*> voitures;
    for (int i = 0; i < 100; i++) {
        int rue = 0 + rand() % (rues.size() - 0);
        int noeud = 0 + rand() % (rues.at(rue).noeuds().size() - 0);
        int vitesse = 10 + rand() % (30 - 10) + 1;
        voitures.push_back(new Voiture("Voiture " + i, vitesse, rues.at(rue).noeuds().at(noeud)));
    }
    
    QApplication a(argc, argv);

    SmartCars* sc = new SmartCars(rues, voitures);
    roads->setFixedSize(roads->getHexMeshWidth(), roads->getHexMeshHeight());

    Application* app = new Application(sc);

    app->show();

    return a.exec();
}