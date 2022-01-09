#pragma once

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include "osmextract.h"
#include "Voiture.h"

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

private:
    vector<Rue> v_rues;
    vector<Voiture*> v_voitures;

    struct Cell {
        Cell(const QPolygon& p, int l, int r, int c)
            : polygon(p), id_cell(l), num_row(r), num_col(c) {}
        QPolygon polygon;
        int id_cell;
        int num_row;
        int num_col;
    };
    std::vector<std::vector<Cell>> cells;
    int widget_width = 806;
    int widget_height = 641;
};
