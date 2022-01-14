#pragma once

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include "osmextract.h"
#include "Voiture.h"
#include "Cell.h"

#include <vector>

using namespace std;

class SmartCars : public QMainWindow
{
    Q_OBJECT

public:
    SmartCars(std::vector<Rue> rues, std::vector<Voiture*> voitures, QWidget* parent = Q_NULLPTR);
    virtual void paintEvent(QPaintEvent* event);

    int getHexMeshWidth();
    int getHexMeshHeight();
    void createMesh();
    Cell* getCellFromCoord(Point point);
    inline int getCellWidth() { return cellWidth; }
    vector<Cell*> getVoisin(Cell* cell);

    vector<Voiture*> getVoitures() { return v_voitures; }
    int getVitesse() { return v_vitesse; }
    vector<Rue> getRues() { return v_rues; }
    void setVitesse(int v) { v_vitesse = v; }
    void setVoitures(vector<Voiture*> voitures) { v_voitures = voitures; }
private:
    vector<Rue> v_rues;
    vector<Voiture*> v_voitures;
    int v_vitesse;

    std::vector<std::vector<Cell>> cells;
    int widget_width = 806;
    int widget_height = 641;
    int cellWidth = 50; // est le diametre sur une représentation en cercle où chaque sommet touche le cercle
};
