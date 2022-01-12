#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/qpushbutton.h>
#include <QThread>
#include "SmartCars.h"
#include "WorkerController.h"

class Application : public QMainWindow
{
	Q_OBJECT

public:
	Application(SmartCars* sc, QWidget* parent = Q_NULLPTR);

private:
	QPushButton* avancer;
	QLineEdit* nbrevoituresagenerer;
	SmartCars* smart_cars;

private slots:
	void handleAvancer();
	void handleGenerateCars();	
};

