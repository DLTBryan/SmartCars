#pragma once

#include <QWidget>
#include "ui_SmartCars.h"

class SmartCars : public QWidget
{
	Q_OBJECT

public:
	SmartCars(QWidget *parent = Q_NULLPTR);
	~SmartCars();

private:
	Ui::SmartCars ui;
};
