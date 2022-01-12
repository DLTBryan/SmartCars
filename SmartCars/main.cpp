#include "SmartCars.h"
#include "Voiture.h"

#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qscrollarea.h>

#include <iostream>
#include "Application.h"

using namespace std;

int main(int argc, char* argv[]) {
    osmextract extracteur("map.osm");
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

    QWidget* window = new QWidget;

    SmartCars* roads = new SmartCars(rues, voitures);
    roads->setFixedSize(roads->getHexMeshWidth(), roads->getHexMeshHeight());
    
    Application* app = new Application(roads);

    app->show();

    return a.exec();
}
