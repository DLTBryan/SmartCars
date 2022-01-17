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

// Gestionnaire de thread pour faire une boucle de simulation non-bloquante
class SimulationThread : public QThread
{
    Q_OBJECT
    void run() override {
        QString result = "";
        while (1) {
            // Si la simulation est activee
            if (*active) {
                vector<Cell*> cells = smart_cars->getAllCells();
                // Reset l'ensemble des cellules a 0
                for (Cell* c : cells) c->setSelected(0);
                // Pour chaque voiture l'avancer
                for (Voiture* v : smart_cars->getVoitures()) {
                    v->avancer(smart_cars->getVitesse() * 200);
                    // Sauvegarde de la cellule courante et de ses voisines
                    Cell* c = smart_cars->getCellFromCoord(v->getCoordonnees());
                    if (c != nullptr && v->getSelected()) {
                        c->setSelected(1);
                        vector<Cell*> voisins = smart_cars->getVoisins(c);
                        for (Cell* element : voisins) element->setSelected(2);
                    }
                }
                // Sauvegarde des voitures voisines de la voiture courante
                for (Voiture* v : smart_cars->getVoitures()) {
                    Cell* c = smart_cars->getCellFromCoord(v->getCoordonnees());
                    v->setVoisin(c != nullptr && c->getSelected() > 0 && !v->getSelected());
                }
                // Lance le signal pour repeindre smart_cars
                emit needRepaint(result);
            }
            // Simulation toutes les 100 millisecondes
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

public:
    SmartCars* smart_cars;
    bool* active;

signals:
    // Signal pour repeindre smart_cars
    void needRepaint(const QString& s);
};