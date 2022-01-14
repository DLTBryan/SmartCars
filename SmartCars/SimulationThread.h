#include <iostream>
#include <chrono>

#include <QObject>
#include <QThread>
#include <QString>

#include "SmartCars.h"
#include "Voiture.h"

class SimulationThread : public QThread
{
    Q_OBJECT
    void run() override {
        QString result = "test";
        while (1) {
            if (*active) {
                for (Voiture* v : smart_cars->getVoitures()) v->avancer(smart_cars->getVitesse());
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