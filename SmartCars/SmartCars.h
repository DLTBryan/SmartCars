#pragma once

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include "osmextract.h"
#include "Voiture.h"
#include "Cell.h"

#include <vector>

using namespace std;

// Classe permettant la simulation
class SmartCars : public QMainWindow
{
    Q_OBJECT

public:
    SmartCars(std::vector<Rue> rues, std::vector<Noeud*> noeuds, std::vector<Voiture*> voitures, QWidget* parent = Q_NULLPTR);

    // Fonction heritee de QMainWindow
    virtual void paintEvent(QPaintEvent* event);

    // Recuperer les dimensions de la map
    int getHexMeshWidth();
    int getHexMeshHeight();
    // Permet de creer le maillage de la map
    void createMesh();
    // Recuperer la cellule depuis un point
    Cell* getCellFromCoord(Point point);
    // recuperer la dimension des cellules de la simulation
    inline int getCellWidth() { return cellWidth; }
    // Recuperer les voisins d'une cellule
    vector<Cell*> getVoisins(Cell* cell);
    // Recuperer l'ensemble des cellules sous forme d'un vecteur
    vector<Cell*> getAllCells() { return allCells; }
    // Recuperer l'ensemble des voitures
    vector<Voiture*> getVoitures() { return v_voitures; }
    // Recuperer la vitesse de simulation
    double getVitesse() { return v_vitesse; }
    // Recuperer les rues
    vector<Rue> getRues() { return v_rues; }
    // Recuperer les noeuds
    vector<Noeud*> getNoeuds() { return v_noeuds; }
    // Definir la vitesse de la simulation
    void setVitesse(double v) { v_vitesse = v; }
    // Mettre a jour les voitures en paramètre
    void setVoitures(vector<Voiture*> voitures) { v_voitures = voitures; }
    // Afficher ou masquer la range de frequence
    bool* showRange = new bool(true);

private:
    // Rues de la simulation
    vector<Rue> v_rues;
    // Noeuds de la simulation
    vector<Noeud*> v_noeuds;
    // Voitures de la simulation
    vector<Voiture*> v_voitures;
    // Vitesse de la simulation
    double v_vitesse;
    // Vecteur de vecteur de cellules (pour representer les 2 dimensions)
    vector<vector<Cell*>> cells;
    // Vecteur des cellules
    vector<Cell*> allCells;

    // Dimensions de la carte
    int widget_width = 806;
    int widget_height = 641;

    // Valeurs utilisees pour le calcul des coordonnees
    double xmax;
    double xmin;
    double ymin;
    double ymax;

    int cellWidth = 50; // est le diametre sur une representation en cercle où chaque sommet touche le cercle
};
