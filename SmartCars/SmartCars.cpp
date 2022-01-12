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
            polygon.setPoint(0, cx + x / 2, cy - y / 2); //1h
            polygon.setPoint(1, cx + cellWidth / 2, cy); //3h
            polygon.setPoint(2, cx + x / 2, cy + y / 2); //5h
            polygon.setPoint(3, cx - x / 2, cy + y / 2); //7h
            polygon.setPoint(4, cx - cellWidth / 2, cy); //9h
            polygon.setPoint(5, cx - x / 2, cy - y / 2); //11h
            polygon.setPoint(6, cx + x / 2, cy - y / 2); //1h
            oneRowCells.push_back(Cell(polygon, i, row, column, cx, cy));
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
    bool out_of_range_of_mesh = false;
    QPoint point = event->pos();
    qDebug() << "mouseReleaseEvent" << event;
    qDebug() << "point" << point << " x:" << point.x() << " y:" << point.y();

    // trouver id depuis coordonnées
    const double pi = 3.14159265358979323846264338327950288;
    double y = 50 * std::cos(pi / 6);
    double pY = point.y();
    //colonnes paires
    int oneH_col = (point.x() - 1) / (0.75 * 50);
    int oneH_row = 0;
    if (oneH_col % 2 == 1) // colonnes impaires
        pY -= y / 2;
    oneH_row =  (pY - 1) / (sqrt(3) * 50 / 2);
    qDebug() << "oneH_col " << oneH_col << "oneH_row " << oneH_row;
     
    const int rows = widget_height / (sqrt(3) * 50 / 2);
    const int columns = widget_width / (0.75 * 50);

    Cell* cell_temp;
    // CAS bordure droite
    if (oneH_col == columns && oneH_row < rows) {
        qDebug() << "equals ";
        // la dernière colonne de droite est une colonne paire
        if (point.y() < y / 2) {
            qDebug() << "IMPAIRE 1er RANG zone out of range";
            cell_temp = &cells[oneH_row][oneH_col - 1];
            double cx = cell_temp->centerX;
            double cy = cell_temp->centerY;
            double x = 50 * std::sin(pi / 6), y = 50 * std::cos(pi / 6);
            double zeroX = cx + x / 2;
            double zeroY = cy - y / 2;
            double unX = cx + 50 / 2;
            double unY = cy;
            double d = (point.x() - zeroX) * (unY - zeroY) - (point.y() - zeroY) * (unX - zeroX);
            qDebug() << "entre 0 et 1 " << d;
            if (d < 0) {
                cell_temp = &cells[oneH_row][oneH_col - 1];
            }
            else {
                out_of_range_of_mesh = true;
            }
        }
        else {
            // colonne 21 vers colonne 20 sur meme ligne (haut gauche)
            qDebug() << "colonne 21 vers colonne 20";
            cell_temp = &cells[oneH_row][oneH_col - 1];
            // utiliser le point 2 pour savoir si notre point est sur la meme ligne ou une ligne plus basse
            double cx = cell_temp->centerX;
            double cy = cell_temp->centerY;
            double x = 50 * std::sin(pi / 6), y = 50 * std::cos(pi / 6);
            double unX = cx + 50 / 2;
            double deuxY = cy + y / 2;
            if (point.x() > unX) {
                out_of_range_of_mesh = true;
            }
            else {
                if (point.y() < deuxY) {
                    qDebug() << "cellule haut gauche";
                    // comparaison point et segment [1,2]
                    double unY = cy;
                    double deuxX = cx + x / 2;
                    double d = (point.x() - unX) * (deuxY - unY) - (point.y() - unY) * (deuxX - unX);
                    qDebug() << "entre 1 et 2 " << d;
                    if (d > 0) {
                        out_of_range_of_mesh = true;
                    }
                }
                else {
                    if (oneH_row + 1 < rows) {
                        qDebug() << "cellule bas gauche";
                        // comparaison point et segment [0,1]
                        cell_temp = &cells[oneH_row + 1][oneH_col - 1];
                        double cx = cell_temp->centerX;
                        double cy = cell_temp->centerY;
                        double zeroX = cx + x / 2;
                        double zeroY = cy - y / 2;
                        double unY = cy;
                        double d = (point.x() - zeroX) * (unY - zeroY) - (point.y() - zeroY) * (unX - zeroX);
                        qDebug() << "entre 0 et 1 " << d;
                        if (d > 0) {
                            out_of_range_of_mesh = true;
                        }
                    }
                    else {
                        qDebug() << "out en bas";
                        out_of_range_of_mesh = true;
                    }
                }
            }
        }
    }

    // check out of range bottom
    else if(oneH_col < columns && oneH_row < rows ){
        cell_temp = &cells[oneH_row][oneH_col];
        double cx = cell_temp->centerX;
        double cy = cell_temp->centerY;
        double x = 50 * std::sin(pi / 6), y = 50 * std::cos(pi / 6);
        double troisX = cx - x / 2;
        double troisY = cy + y / 2; //7h
        double quatreX = cx - 50 / 2;
        double quatreY = cy; //9h
        double cinqX = cx - x / 2;
        double cinqY = cy - y / 2; //11h
        qDebug() << "trois " << troisX << troisY << " quatre " << quatreX << quatreY << " cinq " << cinqX << cinqY;

        /*
        line from A=(x1,y1) to B=(x2,y2) a point P=(x,y)
            d=(x-x1)*(y2-y1)-(y-y1)*(x2-x1)
        If d<0 then the point lies on one side of the line, and if d>0 then it lies on the other side
        => negatif = dans cell
        */
        double d;
        if (point.y() < quatreY && point.x() < troisX) {
            d = (point.x() - quatreX) * (cinqY - quatreY) - (point.y() - quatreY) * (cinqX - quatreX);
            qDebug() << "entre 4 et 5 " << d;
            if (d > 0) {
                if (oneH_col % 2 == 1) {
                    if (oneH_col < columns && oneH_row < rows && oneH_col >= 1) {
                        cell_temp = &cells[oneH_row][oneH_col - 1];
                    }
                    else {
                        out_of_range_of_mesh = true;
                    }
                }
                else {
                    if (oneH_col < columns && oneH_row < rows && oneH_row >= 1 && oneH_col >= 1) {
                        cell_temp = &cells[oneH_row - 1][oneH_col - 1];
                    }
                    else {
                        out_of_range_of_mesh = true;
                    }
                }
                
            }
        }
        else if (point.y() > quatreY && point.x() < troisX) {
            d = (point.x() - troisX) * (quatreY - troisY) - (point.y() - troisY) * (quatreX - troisX);
            qDebug() << "entre 3 et 4 " << d;
            if (d > 0) {
                if (oneH_col % 2 == 1) {
                    if (oneH_col < columns && oneH_row < rows && oneH_col >= 1) {
                        
                        if (oneH_row == rows - 1 && d > 0) {
                            out_of_range_of_mesh = true;
                        }
                        else {
                            cell_temp = &cells[oneH_row + 1][oneH_col - 1];
                        }
                    }
                    else {
                        out_of_range_of_mesh = true;
                    }
                }
                else {
                    if (oneH_col < columns && oneH_row < rows && oneH_col >= 1) {
                        cell_temp = &cells[oneH_row][oneH_col - 1];
                    }
                    else {
                        out_of_range_of_mesh = true;
                    }
                }
            }
        }

        // check out of range top des colonnes impaires
        if (oneH_col % 2 == 1 && point.y() < y / 2) {
            qDebug() << "IMPAIRE 1er RANG zone out of range";
            cell_temp = &cells[oneH_row][oneH_col - 1];
            double cx = cell_temp->centerX;
            double cy = cell_temp->centerY;
            double x = 50 * std::sin(pi / 6), y = 50 * std::cos(pi / 6);
            double zeroX = cx + x / 2;
            double zeroY = cy - y / 2;
            double unX = cx + 50 / 2;
            double unY = cy;
            d = (point.x() - zeroX) * (unY - zeroY) - (point.y() - zeroY) * (unX - zeroX);
            qDebug() << "entre 0 et 1 " << d;
            if (d < 0) {
                cell_temp = &cells[oneH_row][oneH_col - 1];
            }
            else {
                out_of_range_of_mesh = true;
            }   
        }
    }
    //dernière ligne, colonne impaire, partie segment [1,2] de la cell
    else if (oneH_col > 0 && oneH_col % 2 == 0 && oneH_row == rows) {
    // utiliser la cellule haut gauche par localiser le point
        qDebug() << "petite zone ";
        cell_temp = &cells[oneH_row - 1][oneH_col - 1];
        // comparaison point et segment [1,2]
        double cx = cell_temp->centerX;
        double cy = cell_temp->centerY;
        double x = 50 * std::sin(pi / 6), y = 50 * std::cos(pi / 6);
        double unX = cx + 50 / 2;
        double unY = cy;
        double deuxX = cx + x / 2;
        double deuxY = cy + y / 2;
        double d = (point.x() - unX) * (deuxY - unY) - (point.y() - unY) * (deuxX - unX);
        qDebug() << "entre 1 et 2 " << d;
        if (d > 0) {
            out_of_range_of_mesh = true;
        }
    }
    else {
        out_of_range_of_mesh = true;
    }

    if (!out_of_range_of_mesh) {
        cell_temp->selected = !cell_temp->selected;
        update(cell_temp->polygon.boundingRect());
    }


    /*
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

            //cell->selected = !cell->selected;
            //update(cell->polygon.boundingRect());
        }
    }
    */
}