#pragma once

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include "osmextract.h"

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

private:
    vector<Noeud*> v_noeuds;

    struct Cell {
        Cell(const QPolygon& p, int l, int r, int c, double cx, double cy)
            : polygon(p), id_cell(l), num_row(r), num_col(c), centerX(cx), centerY(cy), selected(false) {}
        QPolygon polygon;
        int id_cell;
        int num_row;
        int num_col;
        bool selected;
        double centerX;
        double centerY;
    };
    std::vector<std::vector<Cell>> cells;
    int widget_width = 806;
    int widget_height = 641;
};
