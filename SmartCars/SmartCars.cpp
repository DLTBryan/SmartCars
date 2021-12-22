#include "SmartCars.h"

#include <QDebug>
#include <random>
#include <QMouseEvent>

SmartCars::SmartCars(std::vector<Noeud*> noeuds, QWidget *parent)
    : QMainWindow(parent)
{
    v_noeuds = noeuds;

    qDebug() << "Hexagonal_mesh";
    createMesh();
}

int SmartCars::getHexMeshWidth() {
    return widget_width;
}
int SmartCars::getHexMeshHeight() {
    return widget_height;
}

void SmartCars::createMesh() {
    cells.clear();
    qDebug() << "createMesh " << cells.size();
    const int cellWidth = 50; // est le diametre sur une représentation en cercle où chaque sommet touche le cercle
    const int rows = widget_height / (sqrt(3) * cellWidth / 2);
    const int columns = widget_width / (0.75 * cellWidth);
    qDebug() << rows << " = " << widget_height << " / " << cellWidth;
    qDebug() << columns << " = " << widget_width << " / " << cellWidth;
    const int margin = 2; // => borderColor: pour que le maillage soit collé aux bords haut et gauche

    std::random_device dev;
    std::mt19937 engine(dev());

    const double pi = 3.14159265358979323846264338327950288;
    double x = cellWidth * std::sin(pi / 6), y = cellWidth * std::cos(pi / 6);
    int i = 0;

    for (int row = 0; row < rows; ++row) {
        double cx = margin + cellWidth / 2;
        std::vector<Cell> oneRowCells;
        for (int column = 0; column < columns; ++column) {
            double cy = margin + y / 2 + row * y;
            if (column % 2 == 1)
                cy += y / 2;
            QPolygon polygon(7);
            polygon.setPoint(0, cx + x / 2, cy - y / 2);
            polygon.setPoint(1, cx + cellWidth / 2, cy);
            polygon.setPoint(2, cx + x / 2, cy + y / 2);
            polygon.setPoint(3, cx - x / 2, cy + y / 2);
            polygon.setPoint(4, cx - cellWidth / 2, cy);
            polygon.setPoint(5, cx - x / 2, cy - y / 2);
            polygon.setPoint(6, cx + x / 2, cy - y / 2);
            oneRowCells.push_back(Cell(polygon, i, row, column));
            i++;
            cx += (x + cellWidth) / 2;
        }
        cells.push_back(oneRowCells);
    }

    for (std::vector<Cell> row : cells)
    {
        QString str = "";
        for (Cell cell : row) {
            QVariant var(cell.id_cell);
            QString stringValue = var.toString();
            str += stringValue + " ";
        }
        qDebug() << str;
    }
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
        for (Noeud* noeudvoisin : noeud->noeudsvoisins())
            painter.drawLine(QLine((xmax - noeud->x()) * widget_width / (xmax - xmin), (ymax - noeud->y()) * widget_height / (ymax - ymin), (xmax - noeudvoisin->x()) * widget_width / (xmax - xmin), (ymax - noeudvoisin->y()) * widget_height / (ymax - ymin)));
    }

    //-------------------------

    qDebug() << "paintEvent: ";
    //RGB
    QColor borderColor(200, 200, 200);
    borderColor.setAlphaF(0.3);
    const QColor textColor(255, 0, 0);
    const QColor selectedTextColor(0, 0, 255);

    QColor cellColor(255, 255, 255);
    cellColor.setAlphaF(0); // 0 is transparent -> 1 is opaque
    QColor selectedCellColor(255, 0, 255);
    selectedCellColor.setAlphaF(0.5);

    int widWidth = this->width();
    int widHeight = this->height();

    painter.setRenderHint(QPainter::Antialiasing);
    QFont font("Helvetica");
    font.setPixelSize(10);
    painter.setFont(font);
    for (std::vector<Cell>& row : cells)
    {
        for (Cell cell : row) {
            QPainterPath path;
            path.addPolygon(cell.polygon);
            QPen pen(borderColor, 2);
            painter.setPen(pen);

            painter.setBrush(cell.selected ? selectedCellColor : cellColor);

            painter.drawPath(path);
            painter.setPen(cell.selected ? selectedTextColor : textColor);
            std::string str = std::to_string(cell.num_col) + "," + std::to_string(cell.num_row);
            QString qstr = QString::fromStdString(str);
            painter.drawText(cell.polygon.boundingRect(),
                Qt::AlignCenter, QString(qstr));
        }
    }
}

void SmartCars::mouseReleaseEvent(QMouseEvent* event) {
    QPoint point = event->pos();
    qDebug() << "mouseReleaseEvent" << event;
    qDebug() << "point" << point;

    for (std::vector<Cell> row : cells)
    {
        QString str = "";
        for (Cell cell : row) {
            QVariant var(cell.id_cell); //Qt's QVariant helps to convert an integer value to a QString.
            QString stringValue = var.toString(); //toString() method of QVariant returns the variant as a QString, if the variant has type() Int.
            str += stringValue + " ";
        }
        qDebug() << str;
    }


    for (std::vector<Cell>& row : cells)
    {
        auto cell = std::find_if(row.begin(), row.end(),
            [point](Cell& c) {
                return c.polygon.containsPoint(point, Qt::OddEvenFill);
            }
        );
        if (cell != row.end()) {
            qDebug() << cell->id_cell << " " << cell->num_col << " " << cell->num_row;

            cell->selected = !cell->selected;
            update(cell->polygon.boundingRect());
        }
    }
}