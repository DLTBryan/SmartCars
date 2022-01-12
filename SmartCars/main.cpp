#include "SmartCars.h"

#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qgroupbox.h>
#include <QTranslator>

#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    osmextract extracteur("map_test.osm");
    extracteur.extraire();
    std::vector<Rue> rues = extracteur.rues();
    
    QApplication a(argc, argv);

    QWidget* window = new QWidget;

    SmartCars* roads = new SmartCars(rues);
    roads->setFixedSize(roads->getHexMeshWidth(), roads->getHexMeshHeight());
    
    QPushButton* button2 = new QPushButton("Two");
    QPushButton* button3 = new QPushButton("Three");


    QGroupBox* metaBox = new QGroupBox();
    metaBox->setTitle(QString("Paramètres"));

    QGroupBox* boxGeneration = new QGroupBox();
    boxGeneration->setTitle(QString("Génération"));
    QVBoxLayout* vboxGeneration = new QVBoxLayout;
    vboxGeneration->addWidget(button3);
    vboxGeneration->addStretch(1);
    boxGeneration->setLayout(vboxGeneration);


    QGroupBox* boxSelection = new QGroupBox();
    boxSelection->setTitle(QString("Selection"));
    QVBoxLayout* vboxSelection = new QVBoxLayout;
    vboxSelection->addWidget(button2);
    vboxSelection->addStretch(1);
    boxSelection->setLayout(vboxSelection);

    QVBoxLayout* vboxMeta = new QVBoxLayout;
    vboxMeta->addWidget(boxGeneration);
    vboxMeta->addWidget(boxSelection);
    vboxMeta->addStretch(1);
    metaBox->setLayout(vboxMeta);

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
    layout->addWidget(metaBox);

    window->setLayout(layout);

    window->show();

    return a.exec();
}
