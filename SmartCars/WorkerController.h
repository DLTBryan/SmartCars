#pragma once
#include <QObject>
#include "WorkerSimulation.h"
#include "SmartCars.h"

class WorkerController : public QObject
{
    Q_OBJECT
        QThread workerThread;
public:
    WorkerController() {
        WorkerSimulation* workerSimulation = new WorkerSimulation;
        workerSimulation->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, workerSimulation, &QObject::deleteLater);
        connect(this, &WorkerController::operate, workerSimulation, &WorkerSimulation::doWork);
        connect(workerSimulation, &WorkerSimulation::resultReady, this, &WorkerController::handleResults);
        workerThread.start();
    }
    ~WorkerController() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const QString&) {};
signals:
    void operate(const QString&);
};
