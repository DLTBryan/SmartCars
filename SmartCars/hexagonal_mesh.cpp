#include "hexagonal_mesh.h"

#include <QtWidgets>
#include <QDebug>
#include <random>

Hexagonal_mesh::Hexagonal_mesh(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle(tr("Honeycombs"));
	qDebug() << "Hexagonal_mesh";

    createMesh();
}

Hexagonal_mesh::~Hexagonal_mesh()
{
}

void Hexagonal_mesh::createMesh() {
    cells.clear();
    qDebug() << "createMesh " << cells.size();
    const int cellWidth = 100; // est le diametre sur une représentation en cercle où chaque sommet touche le cercle
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
            cells.push_back(Cell(polygon, i, row, column));
            i++;
            cx += (x + cellWidth) / 2;
        }
    }
}

void Hexagonal_mesh::paintEvent(QPaintEvent* event) {
    qDebug() << "paintEvent";
    //RGB
    const QColor backgroundColor(255, 255, 255);
    const QColor borderColor(0, 0, 0);
    const QColor cellColor(255, 255, 0);
    const QColor textColor(255, 0, 0);
    const QColor selectedCellColor(255, 0, 255);
    const QColor selectedTextColor(0, 0, 0);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), backgroundColor);
    QFont font("Helvetica");
    font.setPixelSize(40);
    painter.setFont(font);
    for (const Cell& cell : cells) {
        QPainterPath path;
        path.addPolygon(cell.polygon);
        QPen pen(borderColor, 2);
        painter.setPen(pen);
        painter.setBrush(cell.selected ? selectedCellColor : cellColor);
        painter.drawPath(path);
        painter.setPen(cell.selected ? selectedTextColor : textColor);
        std::string str = std::to_string(cell.nb_col) + "," + std::to_string(cell.nb_row);
        QString qstr = QString::fromStdString(str);
        painter.drawText(cell.polygon.boundingRect(),
            Qt::AlignCenter, QString(qstr));
    }

}

void Hexagonal_mesh::mouseReleaseEvent(QMouseEvent* event) {
    QPoint point = event->pos();
    //std::cout << 'TEST' << event->pos() << std::endl;
    qDebug() << "mouseReleaseEvent" << event;
    qDebug() << "point" << point;

    auto cell = std::find_if(cells.begin(), cells.end(),
        [point](const Cell& c) {
            return c.polygon.containsPoint(point, Qt::OddEvenFill);
        }
    );
    if (cell != cells.end()) {
        cell->selected = !cell->selected;
        update(cell->polygon.boundingRect());
    }
}

void Hexagonal_mesh::resizeEvent(QResizeEvent* event) {
    auto size = event->size();
    qDebug() << "resize: " << size; //width, height
    widget_height = size.height();
    widget_width = size.width();
    createMesh();
}