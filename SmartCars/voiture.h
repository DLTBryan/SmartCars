#ifndef VOITURE_H
#define VOITURE_H

#include <string>
#include <iostream>

#include "point.h"
#include "noeud.h"
#include "rue.h"

using namespace std;

class Voiture {
public:
	Voiture(std::string nom, int vitesse, Noeud* depart);

	void avancer(int multiplicateur, float distanceRestante=0.0);

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

private:
	std::string nom;
	int vitesse;
	Point coordonnees;

	Noeud* noeudDepart;
	Noeud* noeudArrivee;

	float getDistance(Point a, Point b);
	Point getCoordsFromPercentage(Point depart, Noeud* arrivee, float pourcentage);
	int random(int a, int b);
};

#endif
