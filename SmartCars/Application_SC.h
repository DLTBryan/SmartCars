#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qpushbutton.h>
#include <qlayout.h>
#include <Windows.h>
#include <QtWidgets/qscrollarea.h>

#include "SmartCars.h"
#include "Rue.h"
#include "Voiture.h"

class Application_SC : public QMainWindow
{
	Q_OBJECT
public:
	Application_SC(vector<Rue> rues, vector<Voiture*> voitures, QWidget* parent = Q_NULLPTR);

private:
	QPushButton* avancer;
	SmartCars* smart_cars;

private slots:
	void handleAvancer();
};

