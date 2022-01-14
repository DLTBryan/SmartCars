#ifndef VOITURE_H
#define VOITURE_H

#include <string>
#include <iostream>

#include "Point.h"
#include "Noeud.h"
#include "Rue.h"

using namespace std;

class Voiture {
public:
	Voiture(std::string nom, int vitesse, Noeud* depart);

	void avancer(int multiplicateur, float distanceRestante = 0.0);

	void affichage();

	Noeud* getDepart() {
		return noeudDepart;
	}

	Noeud* getArrivee() {
		return noeudArrivee;
	}

	Point getCoordonnees() {
		return coordonnees;
	}

	int getVitesse() {
		return vitesse;
	}

	bool getSelected() {
		return selected;
	}

	void setSelected(bool value) {
		selected = value;
	}

	void setVitesse(int value) {
		vitesse = value;
	}

	void setVoisin(bool value) {
		voisin = value;
	}

	bool getVoisin() { return voisin; }

	string getNom() { return nom; }

private:
	std::string nom;
	int vitesse;
	Point coordonnees;

	bool selected;
	bool voisin = false;

	Noeud* noeudDepart;
	Noeud* noeudArrivee;

	float getDistance(Point a, Point b);
	Point getCoordsFromPercentage(Point depart, Noeud* arrivee, float pourcentage);
	int random(int a, int b);
};

#endif
