#pragma once

#include <QPainter>

class Cell
{
public:
	Cell(const QPolygon& p, int l, int r, int c, double cx, double cy)
		: polygon(p), id_cell(l), num_row(r), num_col(c), centerX(cx), centerY(cy), selected(false) {}
	QPolygon polygon;
	inline int getIdCell() { return id_cell; }
	inline int getNumRow() { return num_row; }
	inline int getNumCol() { return num_col; }
	inline bool getSelected() { return selected; }
	inline void setSelected() { selected = !selected; }
	inline double getCenterX() { return centerX; }
	inline double getCenterY() { return centerY; }

private:
	//std::vector<Voiture> voitures;
	int id_cell;
	int num_row;
	int num_col;
	bool selected;
	double centerX;
	double centerY;
};

