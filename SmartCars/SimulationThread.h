#include <iostream>
#include <chrono>

#include <QObject>
#include <QThread>
#include <QString>
#include <QCheckBox>

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
                for (Cell* c : cells) c->setSelected(0);
                for (Voiture* v : smart_cars->getVoitures()) {
                    v->avancer(smart_cars->getVitesse());
                    Cell* c = smart_cars->getCellFromCoord(v->getCoordonnees());
                    if (c != nullptr && v->getSelected()) {
                        c->setSelected(1);
                        vector<Cell*> voisins = smart_cars->getVoisins(c);
                        for (Cell* element : voisins) element->setSelected(2);
                    }
                }
                for (Voiture* v : smart_cars->getVoitures()) {
                    Cell* c = smart_cars->getCellFromCoord(v->getCoordonnees());
                    v->setVoisin(c != nullptr && c->getSelected() > 0 && !v->getSelected());
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