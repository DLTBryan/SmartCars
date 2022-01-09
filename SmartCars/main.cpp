#include "SmartCars.h"
#include "Voiture.h"

#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qscrollarea.h>

#include <iostream>

using namespace std;

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

    QWidget* window = new QWidget;

    SmartCars* roads = new SmartCars(rues, voitures);
    roads->setFixedSize(roads->getHexMeshWidth(), roads->getHexMeshHeight());
    
    QPushButton* button2 = new QPushButton("Two");

    QScrollArea* scrollArea = new QScrollArea();
    int scrollbarWidth = 25; // +25px pour scrollbar
    int scrollMaxWidth = roads->getHexMeshWidth() + scrollbarWidth;
    int scrollMaxHeight = roads->getHexMeshHeight() + scrollbarWidth;
    scrollArea->setWidget(roads);
    scrollArea->setMaximumSize(scrollMaxWidth, scrollMaxHeight); 
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QHBoxLayout* layout = new QHBoxLayout(window);
    layout->addWidget(scrollArea);
    layout->addWidget(button2);

    window->setLayout(layout);

    window->show();

    return a.exec();
}
