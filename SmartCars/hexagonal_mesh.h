/// <summary>
/// src: https://rosettacode.org/wiki/Honeycombs#C.2B.2B
/// </summary>
#pragma once

#include <QWidget>
#include <vector>

class Hexagonal_mesh : public QWidget {
	Q_OBJECT

public:
	Hexagonal_mesh(QWidget *parent = nullptr);
	~Hexagonal_mesh();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event);
    void createMesh();

private:
    struct Cell {
        Cell(const QPolygon& p, int l, int r, int c)
            : polygon(p), id_cell(l), nb_row(r), nb_col(c), selected(false) {}
        QPolygon polygon;
        int id_cell;
        int nb_row;
        int nb_col;
        bool selected;
    };
    std::vector<std::vector<Cell>> cells;
    int widget_height;
    int widget_width;
};
