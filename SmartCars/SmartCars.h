#pragma once

#include <QtWidgets/QMainWindow>
#include<QPainter>
#include "noeud.h"
#include "ui_SmartCars.h"

class SmartCars : public QMainWindow
{
    Q_OBJECT

public:
    SmartCars(std::vector<Noeud*> noeuds, QWidget *parent = Q_NULLPTR);
    virtual void paintEvent(QPaintEvent* event);

private:
    Ui::SmartCarsClass ui;
    vector<Noeud*> v_noeuds;
};
