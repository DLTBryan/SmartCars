#include "hexagonal_mesh.h"

#include "SmartCars.h"

#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qscrollarea.h>

#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    auto start = high_resolution_clock::now();
    osmextract extracteur("map_test.osm");
    extracteur.extraire();
    std::vector<Rue> rues = extracteur.rues();
    std::cout << "Size rues : " << rues.size() << std::endl;
    std::vector<Noeud*> noeuds = extracteur.noeuds();
    std::cout << "Size noeuds : " << noeuds.size() << std::endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Temps d'exécution : " << duration.count() << " !" << endl;
    
    QApplication a(argc, argv);

    //QMainWindow mainWindow;

    QWidget* window = new QWidget;


    Hexagonal_mesh* hexMesh = new Hexagonal_mesh();
    hexMesh->setFixedSize(hexMesh->getHexMeshWidth(), hexMesh->getHexMeshHeight());
    QPushButton* button2 = new QPushButton("Two");

    QScrollArea* scrollArea = new QScrollArea();
    int scrollbarWidth = 25; // +25px pour scrollbar
    int scrollMaxWidth = hexMesh->getHexMeshWidth() + scrollbarWidth;
    int scrollMaxHeight = hexMesh->getHexMeshHeight() + scrollbarWidth;
    scrollArea->setWidget(hexMesh);
    scrollArea->setMaximumSize(scrollMaxWidth, scrollMaxHeight); 
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QHBoxLayout* layout = new QHBoxLayout(window);
    layout->addWidget(scrollArea);
    layout->addWidget(button2);

    window->setLayout(layout);

    //mainWindow.setCentralWidget(window);
    //window->resize(640, 480);
    //mainWindow.show();

    window->show();
    
    SmartCars w(noeuds);
    w.show();
    return a.exec();
}
