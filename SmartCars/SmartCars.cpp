#include "SmartCars.h"

SmartCars::SmartCars(std::vector<Noeud*> noeuds, QWidget *parent)
    : QMainWindow(parent)
{
    v_noeuds = noeuds;
    ui.setupUi(this);
}

void SmartCars::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setBrush(QBrush("red"));
    
    QPen pen;
    pen.setWidth(1);

    painter.setPen(pen);
    // Je recherche les xmin, xmax, ymin, ymax
    double xmin = v_noeuds[0]->x();
    double xmax = xmin;
    double ymin = v_noeuds[0]->y();
    double ymax = ymin;
    for (Noeud* noeud : v_noeuds) {
        if (noeud->x() < xmin)
            xmin = noeud->x();
        if (noeud->x() > xmax)
            xmax = noeud->x();
        if (noeud->y() < ymin)
            ymin = noeud->y();
        if (noeud->y() > ymax)
            ymax = noeud->y();
    }
    // Je dessine tous les noeuds et les liaisons entre eux
    for (Noeud* noeud : v_noeuds) {
        for (Noeud* noeudvoisin : noeud->voisins())
            painter.drawLine(QLine((xmax - noeud->x()) * 1000 / (xmax - xmin), (ymax - noeud->y()) * 800 / (ymax - ymin), (xmax - noeudvoisin->x()) * 1000 / (xmax - xmin), (ymax - noeudvoisin->y()) * 800 / (ymax - ymin)));
    }
}
