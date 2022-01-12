#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qpushbutton.h>
#include "SmartCars.h"
#include <qlabel.h>

class Application : public QMainWindow
{
	Q_OBJECT

public:
	Application(SmartCars* sc, QWidget* parent = Q_NULLPTR);

private:
	QPushButton* avancer;
	QPushButton* plus;
	QPushButton* moins;
	QLabel* vitesseLabel;
	SmartCars* smart_cars;
	int vitesse = 200;

private slots:
	void handleAvancer();
	void handleSpeedSimulation();
	void handleSlowSimulation();
};

