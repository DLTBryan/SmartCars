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
    int getHexMeshWidth();
    int getHexMeshHeight();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void createMesh();

private:
    struct Cell {
        Cell(const QPolygon& p, int l, int r, int c)
            : polygon(p), id_cell(l), num_row(r), num_col(c), selected(false) {}
        QPolygon polygon;
        int id_cell;
        int num_row;
        int num_col;
        bool selected;
    };
    std::vector<std::vector<Cell>> cells;
    int widget_height;
    int widget_width;
    std::string pathToBckImage = "./img/backgroundImage.png";
};
