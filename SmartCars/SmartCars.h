#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SmartCars.h"

class SmartCars : public QMainWindow
{
    Q_OBJECT

public:
    SmartCars(QWidget *parent = Q_NULLPTR);

private:
    Ui::SmartCarsClass ui;
};
