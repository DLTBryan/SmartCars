#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/qpushbutton.h>
#include "SmartCars.h"
#include <thread>

class Application : public QMainWindow
{
	Q_OBJECT

public:
	Application(SmartCars* sc, QWidget* parent = Q_NULLPTR);

private:
	QPushButton* avancer;
	QPushButton* arreter;
	QLineEdit* nbrevoituresagenerer;
	SmartCars* smart_cars;
	bool continuer;
	char caractere;
	static void executer(bool* continuer, char* caractere);
	std::thread threadsimulation;

private slots:
	void handleAvancer();
	void handleArreter();
	void handleGenerateCars();	
};

