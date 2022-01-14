#pragma once

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include "osmextract.h"
#include "Cell.h"

#include <vector>

using namespace std;

class SmartCars : public QMainWindow
{
    Q_OBJECT

public:
    SmartCars(std::vector<Noeud*> noeuds, QWidget *parent = Q_NULLPTR);
    virtual void paintEvent(QPaintEvent* event);

    int getHexMeshWidth();
    int getHexMeshHeight();
    void createMesh();
    virtual void mouseReleaseEvent(QMouseEvent* event);
    Cell* getCellFromCoord(/*Point point*/ double x, double y);
    inline int getCellWidth() { return cellWidth; }
    vector<Cell*> getVoisin(Cell* cell);

private:
    vector<Noeud*> v_noeuds;

    std::vector<std::vector<Cell>> cells;
    int widget_width = 806;
    int widget_height = 641;
    int cellWidth = 50; // est le diametre sur une représentation en cercle où chaque sommet touche le cercle
};
