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
	QPushButton* lancerSimulation;
	QPushButton* generateNewSimulation;
	QLineEdit* inputSizeGeneration;
	QPushButton* decrementSpeed;
	QPushButton* incrementSpeed;
	QLabel* vitesseLabel;
	SmartCars* smart_cars;
	SetupSelectionHelper* setupHelper;
	QGroupBox* commandPrompt;
	bool* active = new bool(false);
	bool isNumber(const string& str);

private slots:
	void handleAvancer();
	void handleGenerateCars();
	void handleSpeedSimulation();
	void handleSlowSimulation();
	void handleSelectCar();
	void handleChangeSpeed();
	void repaintSmartCars();
};

