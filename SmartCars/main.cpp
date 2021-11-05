#include "SmartCars.h"
#include <QtWidgets/QApplication>
#include "tinyxml2.h"

#include <iostream>

using namespace tinyxml2;

#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    SmartCars w;
    w.show();
    XMLDocument map;
    XMLError eResult = map.LoadFile("map.osm");
    XMLCheckResult(eResult);
    return a.exec();
}
