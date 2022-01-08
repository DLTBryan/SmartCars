#pragma once

#include <QtWidgets/QMainWindow>
#include<QPainter>
#include "noeud.h"
#include "ui_SmartCars.h"
#include "voiture.h"

class SmartCars : public QMainWindow
{
    Q_OBJECT

public:
    SmartCars(std::vector<Noeud*> noeuds, std::vector<Voiture*> voitures, QWidget* parent = Q_NULLPTR);
    virtual void paintEvent(QPaintEvent* event);
    vector<Voiture*> getVoitures() { return v_voitures; }

private:
    Ui::SmartCarsClass ui;
    vector<Noeud*> v_noeuds;
    vector<Voiture*> v_voitures;
};
