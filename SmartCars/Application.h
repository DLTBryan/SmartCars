#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qpushbutton.h>
#include "SmartCars.h"

class Application : public QMainWindow
{
	Q_OBJECT

public:
	Application(SmartCars* sc, QWidget* parent = Q_NULLPTR);

private:
	QPushButton* avancer;
	SmartCars* smart_cars;

private slots:
	void handleAvancer();
};

