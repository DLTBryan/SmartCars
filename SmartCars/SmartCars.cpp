#include "SmartCars.h"

#include <QDebug>
#include <random>
#include <QMouseEvent>

SmartCars::SmartCars(std::vector<Noeud*> noeuds, QWidget* parent)
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
	const int cellWidth = getCellWidth();
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
	QPoint point = event->pos();
	qDebug() << "mouseReleaseEvent" << event;
	qDebug() << "point" << point << " x:" << point.x() << " y:" << point.y();
	getCellFromCoord(point.x(), point.y());
}

void SmartCars::getCellFromCoord(double x_coord, double y_coord) {
	bool out_of_range_of_mesh = false;

	// segment [a,b] de l'hexagone => cf createMesh() -> polygon.setPoint()

	// valeurs génériques
	const double pi = 3.14159265358979323846264338327950288;
	const int cellWidth = getCellWidth();
	double decalage_bas_col_imp = cellWidth * std::cos(pi / 6) / 2;
	double demi_hauteur_de_cell_sinus = cellWidth * std::sin(pi / 6) / 2;
	double demi_hauteur_de_cell = cellWidth / 2;
	const int rows = widget_height / (sqrt(3) * cellWidth / 2);
	const int columns = widget_width / (0.75 * cellWidth);

	//premier calcul maille en carré pour avoir une idée
	int oneH_col = (x_coord - 1) / (0.75 * 50);
	int oneH_row = 0;
	double pY = y_coord;
	if (oneH_col % 2 == 1) // soustraction dans le cas colonnes impaires car plus basse
		pY -= decalage_bas_col_imp;
	oneH_row = (pY - 1) / (sqrt(3) * 50 / 2);
	qDebug() << "oneH_col " << oneH_col << "oneH_row " << oneH_row;

	//je cherche à obtenir les valeurs exactes (row, col) tout en rejetant les coordonnées extérieurs à la hex mesh
	Cell* cell_temp;

	/*
	line from A=(x1,y1) to B=(x2,y2) and a point P=(x,y)
	d=(x-x1)*(y2-y1)-(y-y1)*(x2-x1)
	If d<0 then the point lies on one side of the line, and if d>0 then it lies on the other side
	*/
	// lambda expression
	auto d_value = [](double x, double y, double x1, double x2, double y1, double y2)
	{
		return (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1);
	};

	if (oneH_col < 0 || oneH_row < 0) {
		out_of_range_of_mesh = true;
	}

	// CAS zone de décalage vers le bas sur la 1ere ligne top des colonnes impaires
	else if (oneH_col % 2 == 1 && y_coord < decalage_bas_col_imp) {
		qDebug() << "IMPAIRE 1er RANG zone out of range";
		cell_temp = &cells[oneH_row][oneH_col - 1];
		double cx = cell_temp->centerX;
		double cy = cell_temp->centerY;
		double zeroX = cx + demi_hauteur_de_cell_sinus;
		double zeroY = cy - decalage_bas_col_imp;
		double unX = cx + demi_hauteur_de_cell;
		double unY = cy;
		//comparaison du point avec le segment [0,1] de l'hexagone
		double d = d_value(x_coord, y_coord, zeroX, unX, zeroY, unY);
		qDebug() << "entre 0 et 1 " << d;
		if (d > 0) {
			out_of_range_of_mesh = true;
		}
	}

	// CAS bordure droite
	else if (oneH_col == columns && oneH_row <= rows) {
		qDebug() << "bordure droite ";
		// 1ere ligne zone du decalage_bas_col_imp
		if (y_coord < decalage_bas_col_imp) {
			qDebug() << "IMPAIRE 1er RANG zone out of range";
			cell_temp = &cells[oneH_row][oneH_col - 1];
			double cx = cell_temp->centerX;
			double cy = cell_temp->centerY;
			double zeroX = cx + demi_hauteur_de_cell_sinus;
			double zeroY = cy - decalage_bas_col_imp;
			double unX = cx + demi_hauteur_de_cell;
			double unY = cy;
			//comparaison du point avec le segment [0,1] de l'hexagone
			double d = d_value(x_coord, y_coord, zeroX, unX, zeroY, unY);
			qDebug() << "entre 0 et 1 " << d;
			if (d > 0) {
				out_of_range_of_mesh = true;
			}
		}
		// cell hors mesh bas droite
		else if (oneH_row == rows) {
			out_of_range_of_mesh = true;
		}
		else {
			// colonne 21 vers colonne 20 sur meme ligne (haut gauche)
			qDebug() << "colonne 21 vers colonne 20";
			cell_temp = &cells[oneH_row][oneH_col - 1];
			// utiliser une cell_temp existante pour savoir si notre point est sur la meme ligne ou une ligne plus basse
			double cx = cell_temp->centerX;
			double cy = cell_temp->centerY;
			double unX = cx + demi_hauteur_de_cell;
			double deuxY = cy + decalage_bas_col_imp;

			//le point x sort à droite de la hex mesh
			if (x_coord > unX) {
				out_of_range_of_mesh = true;
			}
			else {
				// utiliser la cellule heut gauche => garder cellule actuellement sélectionné et calculer valeurs manquantes
				if (y_coord < deuxY) {
					qDebug() << "cellule haut gauche";
					double unY = cy;
					double deuxX = cx + demi_hauteur_de_cell_sinus;
					//comparaison du point avec le segment [1,2] de l'hexagone
					double d = d_value(x_coord, y_coord, unX, deuxX, unY, deuxY);
					qDebug() << "entre 1 et 2 " << d;
					if (d > 0) {
						out_of_range_of_mesh = true;
					}
				}
				// utiliser la cellule bas gauche => changer de cell_temp
				else {
					qDebug() << "cellule bas gauche";
					if (oneH_row + 1 == rows) {
						out_of_range_of_mesh = true;
					}
					else {
						cell_temp = &cells[oneH_row + 1][oneH_col - 1];
						double cx = cell_temp->centerX;
						double cy = cell_temp->centerY;
						double zeroX = cx + demi_hauteur_de_cell_sinus;
						double zeroY = cy - decalage_bas_col_imp;
						double unX = cx + demi_hauteur_de_cell;
						double unY = cy;
						//comparaison du point avec le segment [0,1] de l'hexagone
						double d = d_value(x_coord, y_coord, zeroX, unX, zeroY, unY);
						qDebug() << "entre 0 et 1 " << d;
						if (d > 0) {
							out_of_range_of_mesh = true;
						}
					}
				}
			}
		}
	}

	// CAS zone de décalage vers le bas sur la derniere ligne top des colonnes paires
	else if (oneH_col % 2 == 0 && oneH_col < columns && oneH_col > 0 && oneH_row == rows) {
		qDebug() << "dernière ligne clic bas gauche d'une cell pas pris en compte";
		// utiliser la cellule haut gauche
		cell_temp = &cells[oneH_row - 1][oneH_col - 1];
		double cx = cell_temp->centerX;
		double cy = cell_temp->centerY;
		double unX = cx + demi_hauteur_de_cell;
		double unY = cy;
		double deuxX = cx + demi_hauteur_de_cell_sinus;
		double deuxY = cy + decalage_bas_col_imp;
		//comparaison du point avec le segment [1,2] de l'hexagone
		double d = d_value(x_coord, y_coord, unX, deuxX, unY, deuxY);
		qDebug() << "entre 1 et 2 " << d;
		if (d > 0) {
			out_of_range_of_mesh = true;
		}
	}

	// CAS cell interne qui fonctionne aussi pour CAS pour bordure gauche
	else if (oneH_col < columns && oneH_row < rows) {
		cell_temp = &cells[oneH_row][oneH_col];
		double cx = cell_temp->centerX;
		double cy = cell_temp->centerY;
		double troisX = cx - demi_hauteur_de_cell_sinus;
		double troisY = cy + decalage_bas_col_imp;
		double quatreX = cx - demi_hauteur_de_cell;
		double quatreY = cy;
		double cinqX = cx - demi_hauteur_de_cell_sinus;
		double cinqY = cy - decalage_bas_col_imp;
		qDebug() << "trois " << troisX << troisY << " quatre " << quatreX << quatreY << " cinq " << cinqX << cinqY;

		if (x_coord < troisX) {
			if (y_coord < quatreY) {
				//comparaison du point avec le segment [4,5] de l'hexagone
				double d = d_value(x_coord, y_coord, quatreX, cinqX, quatreY, cinqY);
				qDebug() << "entre 4 et 5 " << d;
				if (d > 0) {
					if (oneH_col % 2 == 1) {
						if (oneH_col >= 1) {
							cell_temp = &cells[oneH_row][oneH_col - 1];
						}
						else {
							out_of_range_of_mesh = true;
						}
					}
					else {
						if (oneH_row >= 1 && oneH_col >= 1) {
							cell_temp = &cells[oneH_row - 1][oneH_col - 1];
						}
						else {
							out_of_range_of_mesh = true;
						}
					}

				}
			}
			else if (y_coord > quatreY) {
				//comparaison du point avec le segment [3,4] de l'hexagone
				double d = d_value(x_coord, y_coord, troisX, quatreX, troisY, quatreY);
				qDebug() << "entre 3 et 4 " << d;
				if (d > 0) {
					if (oneH_col % 2 == 1) {
						if (oneH_col >= 1 && oneH_row + 1 < rows) {
							cell_temp = &cells[oneH_row + 1][oneH_col - 1];
						}
						else {
							out_of_range_of_mesh = true;
						}
					}
					else {
						if (oneH_col >= 1) {
							cell_temp = &cells[oneH_row][oneH_col - 1];
						}
						else {
							out_of_range_of_mesh = true;
						}
					}
				}
			}
		}
	}

	else {
		out_of_range_of_mesh = true;
	}

	if (!out_of_range_of_mesh) {
		cell_temp->selected = !cell_temp->selected;
		update(cell_temp->polygon.boundingRect());
	}
}