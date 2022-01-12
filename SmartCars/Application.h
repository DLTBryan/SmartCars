#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qpushbutton.h>
#include <qlayout.h>
#include <Windows.h>
#include <QtWidgets/qscrollarea.h>

#include "SmartCars.h"

class Application : public QMainWindow
{
	Q_OBJECT
public:
	Application(SmartCars* smart_cars, QWidget* parent = Q_NULLPTR);
	//virtual void paintEvent(QPaintEvent* event);

private:
	QPushButton* avancer;
	SmartCars* smart_cars;
	bool active = false;

private slots:
	void handleAvancer();
};

