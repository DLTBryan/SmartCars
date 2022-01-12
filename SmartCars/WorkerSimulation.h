#include "SmartCars.h"
#include "Voiture.h"
#include <QThread>

class WorkerSimulation : public QObject
{
    Q_OBJECT

public slots:
    void doWork(const QString& parameter) {
        QString result;
        std::cout << "thread"  << std::endl;
        /*while (1) {
            for (Voiture* v : smart_cars->getVoitures()) v->avancer(200);
            smart_cars->repaint();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }*/
        emit resultReady(result);
    }
signals:
    void resultReady(const QString& result);
public:
    SmartCars* smart_cars;
};
