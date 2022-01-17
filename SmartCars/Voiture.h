#ifndef VOITURE_H
#define VOITURE_H

#include <string>
#include <iostream>

#include "Point.h"
#include "Noeud.h"
#include "Rue.h"

using namespace std;

// Classe representant une voiture
class Voiture {
public:
	Voiture(std::string nom, int vitesse, Noeud* depart);

	// Permet de mettre a jour les coordonnees
	void avancer(int multiplicateur, float distanceRestante = 0.0);

	// Affichage console pour debug
	void affichage();

	// Recuperer le noeud de depart
	Noeud* getDepart() {
		return noeudDepart;
	}

	// Recuperer le noeud d'arrivee
	Noeud* getArrivee() {
		return noeudArrivee;
	}

	// Recuperer les coordonnees
	Point getCoordonnees() {
		return coordonnees;
	}

	// Recuperer la vitesse
	int getVitesse() {
		return vitesse;
	}

	// Voiture selectionnee ?
	bool getSelected() {
		return selected;
	}

	// Definir la selection
	void setSelected(bool value) {
		selected = value;
	}

	// Definir la vitesse
	void setVitesse(int value) {
		vitesse = value;
	}

	// Definir si elle est voisine
	void setVoisin(bool value) {
		voisin = value;
	}

	// Est voisine avec la voiture actuelle ?
	bool getVoisin() { return voisin; }

	// Recuperer le nom
	string getNom() { return nom; }

private:
	// Nom
	string nom;
	// Vitesse
	int vitesse;
	// Coordonnees
	Point coordonnees;

	// Est selectionnee
	bool selected;
	// Est voisine de la voiture courante
	bool voisin = false;

	// Noeud de depart
	Noeud* noeudDepart;
	// Noeud d'arrivee
	Noeud* noeudArrivee;

	// Recuperer la distance entre deux coordonnees
	float getDistance(Point a, Point b);
	// Recuperer les coordonnees entre un point de depart et d'arrivee en fonction d'un pourcentage
	Point getCoordsFromPercentage(Point depart, Noeud* arrivee, float pourcentage);
	// Generation aleatoire [a,b]
	int random(int a, int b);
};

#endif
