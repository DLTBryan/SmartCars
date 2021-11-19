#include "SmartCars.h"

#include <QtWidgets/QApplication>
#include "tinyxml2.h"

#include "rue.h"
#include "OsmProcessor.h"
#include "TestOsmProcessor.h"
#include <chrono>

#include <vector>
#include <map>
#include <iostream>

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
