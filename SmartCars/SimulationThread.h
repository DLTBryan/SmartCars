#include <iostream>
#include <chrono>

#include <QObject>
#include <QThread>
#include <QString>

#include "SmartCars.h"
#include "Voiture.h"
#include "Cell.h"
#include "Point.h"

class SimulationThread : public QThread
{
    Q_OBJECT
    void run() override {
        QString result = "test";
        while (1) {
            if (*active) {
                vector<Cell*> cells = smart_cars->getAllCells();
                for (Cell* c : cells) c->setSelected(false);
                for (Voiture* v : smart_cars->getVoitures()) {
                    v->avancer(smart_cars->getVitesse());
                    Cell* c = smart_cars->getCellFromCoord(v->getCoordonnees());
                    if (c != nullptr && v->getSelected()) {
                        c->setSelected(true);
                        vector<Cell*> voisins = smart_cars->getVoisins(c);
                        for (Cell* element : voisins) element->setSelected(true);
                    }
                }
                emit needRepaint(result);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

public:
    SmartCars* smart_cars;
    bool* active;

signals:
    void needRepaint(const QString& s);
};