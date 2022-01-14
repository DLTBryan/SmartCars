#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include "SmartCars.h"
#include "SimulationThread.h"
#include "SetupSelectionHelper.h"
#include <QLabel>
#include <QtWidgets/QApplication>

class Application : public QMainWindow
{
	Q_OBJECT

public:
	Application(SmartCars* sc, QWidget* parent = Q_NULLPTR);

private:
	QPushButton* avancer;
	QPushButton* buttoninitialisation;
	QLineEdit* nbrevoituresagenerer;
	QPushButton* plus;
	QPushButton* moins;
	QLabel* vitesseLabel;
	SmartCars* smart_cars;
	SetupSelectionHelper* setupHelper;
	bool* active = new bool(false);

private slots:
	void handleAvancer();
	void handleGenerateCars();
	void handleSpeedSimulation();
	void handleSlowSimulation();
	void handleSelectCar();
	void handleChangeSpeed();
	void repaintSmartCars();
};

