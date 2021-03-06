#include "SmartCars.h"

#include <QDebug>
#include <random>
#include <QMouseEvent>

SmartCars::SmartCars(std::vector<Rue> rues, std::vector<Noeud*> noeuds, std::vector<Voiture*> voitures, QWidget *parent)
    : QMainWindow(parent)
{
    v_vitesse = 1.;
    v_rues = rues;
	v_noeuds = noeuds;
    v_voitures = voitures;

    createMesh();
}

// Recuperer les dimensions de la map
int SmartCars::getHexMeshWidth() {
    return widget_width;
}
int SmartCars::getHexMeshHeight() {
    return widget_height;
}

// Creation du maillage hexagonal de la map
void SmartCars::createMesh() {
    cells.clear();
	allCells.clear();
    const int cellWidth = getCellWidth();
    const int rows = widget_height / (sqrt(3) * cellWidth / 2);
    const int columns = widget_width / (0.75 * cellWidth);
    const int margin = 2; // => borderColor: pour que le maillage soit colle aux bords haut et gauche

    std::random_device dev;
    std::mt19937 engine(dev());

    const double pi = 3.14159265358979323846264338327950288;
    double x = cellWidth * std::sin(pi / 6), y = cellWidth * std::cos(pi / 6);
    int i = 0;

    for (int row = 0; row < rows; ++row) {
        double cx = margin + cellWidth / 2;
        std::vector<Cell*> oneRowCells;
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
			Cell* c = new Cell(polygon, i, row, column, cx, cy);
            oneRowCells.push_back(c);
			allCells.push_back(c);
            i++;
            cx += (x + cellWidth) / 2;
        }
        cells.push_back(oneRowCells);
    }
}

// Fonction pour repeindre (= mettre a jour) la fenetre et afficher les infos
void SmartCars::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
	painter.fillRect(rect(), QColor(205, 235, 176));
    
    QPen pen;
    pen.setWidth(5);
    pen.setColor("black");

    painter.setPen(pen);
    // Je recherche les xmin, xmax, ymin, ymax
    vector<Noeud*> v_noeuds;
    for (Rue rue : v_rues) {
        vector<Noeud*> noeudsrue = rue.noeuds();
        v_noeuds.insert(v_noeuds.end(), noeudsrue.begin(), noeudsrue.end());
    }
    xmin = v_noeuds[0]->x();
    xmax = xmin;
    ymin = v_noeuds[0]->y();
    ymax = ymin;
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
    // Je dessine une par une des rues
    for (Rue rue : v_rues) {
        std::string typeroute = rue.type();
		QColor couleurroute;
		int tailleroute;
        if (typeroute == "motorway" || typeroute == "motorway_link") {
			tailleroute = 13;
			couleurroute = QColor(233, 144, 160);
        } else if (typeroute == "trunk" || typeroute == "trunk_link") {
			tailleroute = 13;
			couleurroute = QColor(251, 192, 172);
        } else if (typeroute == "primary" || typeroute == "primary_link") {
			tailleroute = 13;
			couleurroute = QColor(253, 215, 161);
        } else if (typeroute == "secondary" || typeroute == "secondary_link") {
			tailleroute = 11;
			couleurroute = QColor(246, 250, 187);
        } else if (typeroute == "tertiary" || typeroute == "tertiary_link") {
			tailleroute = 10;
			couleurroute = QColor(196, 195, 194);
        } else if (typeroute == "unclassified") {
			tailleroute = 9;
			couleurroute = QColor(208, 207, 206);
        } else if (typeroute == "residential") {
			tailleroute = 9;
			couleurroute = QColor(220, 219, 218);
		} else if (typeroute == "service") {
			tailleroute = 5;
			couleurroute = QColor(237, 237, 237);
		} else if (typeroute == "living_street") {
			tailleroute = 5;
			couleurroute = QColor(237, 237, 237);
        } else {
			tailleroute = 2;
			couleurroute = QColor(211, 185, 159);
        }
        vector<Noeud*> noeudsrue = rue.noeuds();
        for (Noeud* noeud : noeudsrue) {
			for (Noeud* noeudvoisin : noeud->noeudsVoisins()) {
				// Dessin de la "bordure"
				/*pen.setWidth(tailleroute + 1);
				pen.setColor(QColor(0, 0, 0));
				painter.setPen(pen);
				painter.drawLine(QLine(((xmax - noeud->x()) * widget_width / (xmax - xmin)), ((ymax - noeud->y()) * widget_height / (ymax - ymin)), ((xmax - noeudvoisin->x()) * widget_width / (xmax - xmin)), ((ymax - noeudvoisin->y()) * widget_height / (ymax - ymin))));*/
				// Dessin de la route lui-meme
				pen.setWidth(tailleroute);
				pen.setColor(couleurroute);
				painter.setPen(pen);
				painter.drawLine(QLine((xmax - noeud->x()) * widget_width / (xmax - xmin), (ymax - noeud->y()) * widget_height / (ymax - ymin), (xmax - noeudvoisin->x()) * widget_width / (xmax - xmin), (ymax - noeudvoisin->y()) * widget_height / (ymax - ymin)));
			}
        }
    }

    //-------------------------
    // hexagonal mesh part
    //RGB
    QColor borderColor(0, 0, 0);
    borderColor.setAlphaF(0.07);

    QColor cellColor(255, 255, 255);
    cellColor.setAlphaF(0); // 0 is transparent -> 1 is opaque

	QColor cellSelectedColor(255, 0, 255);
	cellSelectedColor.setAlphaF(0.5);

	QColor cellVoisinColor(255, 0, 255);
	cellVoisinColor.setAlphaF(0.25);

    painter.setRenderHint(QPainter::Antialiasing);
		for (std::vector<Cell*>& row : cells)
		{
			for (Cell* cell : row) {
				QPainterPath path;
				path.addPolygon(cell->polygon);
				QPen pen(borderColor, 2);
				painter.setPen(pen);
				painter.setBrush(cellColor);
				if (*showRange) {
					if (cell->getSelected() == 1) painter.setBrush(cellSelectedColor);
					if (cell->getSelected() == 2) painter.setBrush(cellVoisinColor);
				}
				painter.drawPath(path);
			}
		}

	// Dessin de l'ensemble des voitures
	pen.setWidth(1);
	painter.setPen(pen);
	for (Voiture* v : v_voitures) {
		// Couleur bleue si selectionnee, rouge si voisine, verte sinon
		if (v->getSelected()) painter.setBrush(QBrush("blue"));
		else if(v->getVoisin()) painter.setBrush(QBrush("red"));
		else painter.setBrush(QBrush("green"));
		float x = (xmax - v->getCoordonnees().x()) * widget_width / (xmax - xmin);
		float y = (ymax - v->getCoordonnees().y()) * widget_height / (ymax - ymin);
		painter.drawEllipse(QPointF(x, y), 5, 5);
	}
}

// Recuperation d'une cellule depuis des coordonnees
Cell* SmartCars::getCellFromCoord(Point point) {
	double x_coord = (xmax - point.x()) * widget_width / (xmax - xmin);
	double y_coord = (ymax - point.y()) * widget_height / (ymax - ymin);
	bool out_of_range_of_mesh = false;

	// segment [a,b] de l'hexagone => cf createMesh() -> polygon.setPoint()

	// valeurs generiques
	const double pi = 3.14159265358979323846264338327950288;
	const int cellWidth = getCellWidth();
	double decalage_bas_col_imp = cellWidth * std::cos(pi / 6) / 2;
	double demi_hauteur_de_cell_sinus = cellWidth * std::sin(pi / 6) / 2;
	double demi_hauteur_de_cell = cellWidth / 2;
	const int rows = widget_height / (sqrt(3) * cellWidth / 2);
	const int columns = widget_width / (0.75 * cellWidth);

	//premier calcul maille en carre pour avoir une idee de la row et col de la cell
	int oneH_col = (x_coord - 1) / (0.75 * 50);
	int oneH_row = 0;
	double pY = y_coord;
	if (oneH_col % 2 == 1) // soustraction dans le cas colonnes impaires car plus basse
		pY -= decalage_bas_col_imp;
	oneH_row = (pY - 1) / (sqrt(3) * 50 / 2);

	//je cherche a obtenir les valeurs exactes (row, col) tout en rejetant les coordonnees exterieurs a la hex mesh
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

	// CAS zone de decalage vers le bas sur la 1ere ligne top des colonnes impaires
	else if (oneH_col % 2 == 1 && y_coord < decalage_bas_col_imp) {
		cell_temp = cells[oneH_row][oneH_col - 1];
		double cx = cell_temp->getCenterX();
		double cy = cell_temp->getCenterY();
		double zeroX = cx + demi_hauteur_de_cell_sinus;
		double zeroY = cy - decalage_bas_col_imp;
		double unX = cx + demi_hauteur_de_cell;
		double unY = cy;
		//comparaison du point avec le segment [0,1] de l'hexagone
		double d = d_value(x_coord, y_coord, zeroX, unX, zeroY, unY);
		if (d > 0) {
			out_of_range_of_mesh = true;
		}
	}

	// CAS bordure droite
	else if (oneH_col == columns && oneH_row <= rows) {
		// 1ere ligne zone du decalage_bas_col_imp
		if (y_coord < decalage_bas_col_imp) {
			cell_temp = cells[oneH_row][oneH_col - 1];
			double cx = cell_temp->getCenterX();
			double cy = cell_temp->getCenterY();
			double zeroX = cx + demi_hauteur_de_cell_sinus;
			double zeroY = cy - decalage_bas_col_imp;
			double unX = cx + demi_hauteur_de_cell;
			double unY = cy;
			//comparaison du point avec le segment [0,1] de l'hexagone
			double d = d_value(x_coord, y_coord, zeroX, unX, zeroY, unY);
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
			cell_temp = cells[oneH_row][oneH_col - 1];
			// utiliser une cell_temp existante pour savoir si notre point est sur la meme ligne ou une ligne plus basse
			double cx = cell_temp->getCenterX();
			double cy = cell_temp->getCenterY();
			double unX = cx + demi_hauteur_de_cell;
			double deuxY = cy + decalage_bas_col_imp;

			//le point x sort a droite de la hex mesh
			if (x_coord > unX) {
				out_of_range_of_mesh = true;
			}
			else {
				// utiliser la cellule heut gauche => garder cellule actuellement selectionne et calculer valeurs manquantes
				if (y_coord < deuxY) {
					double unY = cy;
					double deuxX = cx + demi_hauteur_de_cell_sinus;
					//comparaison du point avec le segment [1,2] de l'hexagone
					double d = d_value(x_coord, y_coord, unX, deuxX, unY, deuxY);
					if (d > 0) {
						out_of_range_of_mesh = true;
					}
				}
				// utiliser la cellule bas gauche => changer de cell_temp
				else {
					if (oneH_row + 1 == rows) {
						out_of_range_of_mesh = true;
					}
					else {
						cell_temp = cells[oneH_row + 1][oneH_col - 1];
						double cx = cell_temp->getCenterX();
						double cy = cell_temp->getCenterY();
						double zeroX = cx + demi_hauteur_de_cell_sinus;
						double zeroY = cy - decalage_bas_col_imp;
						double unX = cx + demi_hauteur_de_cell;
						double unY = cy;
						//comparaison du point avec le segment [0,1] de l'hexagone
						double d = d_value(x_coord, y_coord, zeroX, unX, zeroY, unY);
						if (d > 0) {
							out_of_range_of_mesh = true;
						}
					}
				}
			}
		}
	}

	// CAS zone de decalage vers le bas sur la derniere ligne top des colonnes paires
	else if (oneH_col % 2 == 0 && oneH_col < columns && oneH_col > 0 && oneH_row == rows) {
		// utiliser la cellule haut gauche
		cell_temp = cells[oneH_row - 1][oneH_col - 1];
		double cx = cell_temp->getCenterX();
		double cy = cell_temp->getCenterY();
		double unX = cx + demi_hauteur_de_cell;
		double unY = cy;
		double deuxX = cx + demi_hauteur_de_cell_sinus;
		double deuxY = cy + decalage_bas_col_imp;
		//comparaison du point avec le segment [1,2] de l'hexagone
		double d = d_value(x_coord, y_coord, unX, deuxX, unY, deuxY);
		if (d > 0) {
			out_of_range_of_mesh = true;
		}
	}

	// CAS cell interne qui fonctionne aussi pour CAS pour bordure gauche
	else if (oneH_col < columns && oneH_row < rows) {
		cell_temp = cells[oneH_row][oneH_col];
		double cx = cell_temp->getCenterX();
		double cy = cell_temp->getCenterY();
		double troisX = cx - demi_hauteur_de_cell_sinus;
		double troisY = cy + decalage_bas_col_imp;
		double quatreX = cx - demi_hauteur_de_cell;
		double quatreY = cy;
		double cinqX = cx - demi_hauteur_de_cell_sinus;
		double cinqY = cy - decalage_bas_col_imp;

		if (x_coord < troisX) {
			if (y_coord < quatreY) {
				//comparaison du point avec le segment [4,5] de l'hexagone
				double d = d_value(x_coord, y_coord, quatreX, cinqX, quatreY, cinqY);
				if (d > 0) {
					if (oneH_col % 2 == 1) {
						if (oneH_col >= 1) {
							cell_temp = cells[oneH_row][oneH_col - 1];
						}
						else {
							out_of_range_of_mesh = true;
						}
					}
					else {
						if (oneH_row >= 1 && oneH_col >= 1) {
							cell_temp = cells[oneH_row - 1][oneH_col - 1];
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
				if (d > 0) {
					if (oneH_col % 2 == 1) {
						if (oneH_col >= 1 && oneH_row + 1 < rows) {
							cell_temp = cells[oneH_row + 1][oneH_col - 1];
						}
						else {
							out_of_range_of_mesh = true;
						}
					}
					else {
						if (oneH_col >= 1) {
							cell_temp = cells[oneH_row][oneH_col - 1];
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
	if (out_of_range_of_mesh) {
		cell_temp = nullptr;
	}
	return cell_temp;
}

// Recuperer toutes les cellules voisines autour d'une rentree en param?tre
vector<Cell*> SmartCars::getVoisins(Cell* cell)
{
	vector<Cell*> voisins;

	const int rows = widget_height / (sqrt(3) * cellWidth / 2);
	const int columns = widget_width / (0.75 * cellWidth);

	int row_init = cell->getNumRow();
	int col_init = cell->getNumCol();

	// commence par le voisin du dessus et continue sens horaire

	if (row_init - 1 >= 0) {
		voisins.push_back(cells[row_init - 1][col_init]);
	}
	if (col_init + 1 < columns) {
		if (col_init % 2 == 1) {
			voisins.push_back(cells[row_init][col_init + 1]);
		}
		else if (row_init - 1 >= 0) {
			voisins.push_back(cells[row_init - 1][col_init + 1]);
		}
	}
	if (col_init + 1 < columns) {
		if (col_init % 2 == 0) {
			voisins.push_back(cells[row_init][col_init + 1]);
		}
		else if (row_init + 1 < rows) {
			voisins.push_back(cells[row_init + 1][col_init + 1]);
		}
	}
	if (row_init + 1 < rows) {
		voisins.push_back(cells[row_init + 1][col_init]);
	}
	if (col_init - 1 >= 0) {
		if (col_init % 2 == 0) {
			voisins.push_back(cells[row_init][col_init - 1]);
		}
		else if (row_init + 1 < rows) {
			voisins.push_back(cells[row_init + 1][col_init - 1]);
		}
	}
	if (col_init - 1 >= 0) {
		if (col_init % 2 == 1) {
			voisins.push_back(cells[row_init][col_init - 1]);
		}
		else if (row_init - 1 >= 0) {
			voisins.push_back(cells[row_init - 1][col_init - 1]);
		}
	}
	return voisins;
}