#ifndef VOITURE_H
#define VOITURE_H

#include <string>
#include <iostream>

#include "point.h"
#include "noeud.h"
#include "rue.h"

class Voiture {
public:
	Voiture(std::string nom, int vitesse, Noeud* depart);

	void avancer(int multiplicateur);

	void affichage();

	Noeud* getDepart() {
		return depart;
	}

	Noeud* getArrivee() {
		return arrivee;
	}

	Point getCoordonnees() {
		return coordonnees;
	}

private:
	std::string nom;
	int vitesse;
	Point coordonnees;

	Noeud* depart;
	Noeud* arrivee;

	float getDistance(Point a, Point b);
};

#endif
