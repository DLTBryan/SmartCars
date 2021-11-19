#include "voiture.h"

Voiture::Voiture(string nom, int vitesse, Noeud* depart)
{
	this->nom = nom;
	this->vitesse = vitesse;
	this->depart = depart;
	this->arrivee = depart->voisins().at(0);
	this->coordonnees = depart->coordonnees();
}

void Voiture::avancer(int multiplicateur, float distanceRestance)
{
	Point depart = this->coordonnees;
	Point arrivee = this->arrivee->coordonnees();

	float distance = getDistance(depart, this->arrivee->coordonnees()) * 1000;

	float distanceAparcourir = distanceRestance;

	if (distanceAparcourir == 0) {
		distanceAparcourir = this->vitesse * multiplicateur;
	}

	float pourcentageParcouru = distanceAparcourir / distance;

	cout << "distanceAparcourir : " << distanceAparcourir << endl;

	cout << "Distance : " << distance << endl;

	cout << "% : " << pourcentageParcouru << endl;

	if (pourcentageParcouru >= 1) {
		cout << "Avancer car % : " << pourcentageParcouru << endl;
		this->depart = this->arrivee;

		int i = 0;
		while (this->arrivee == this->depart) {
			this->arrivee = this->arrivee->voisins().at(i);
			i++;
		}

		this->coordonnees = this->depart->coordonnees();

		if(pourcentageParcouru > 1) this->avancer(multiplicateur, distanceAparcourir - distance);
	}
	else {
		//Faire new coordonnees
		cout << "Fini" << endl;
	}

	//Reflechir au ratio vitesse distance
}

void Voiture::affichage() {
	cout << "--- Affichage ---" << this->nom << endl;
	cout << "Nom : " << this->nom << endl;
	cout << "Coordonnées : " << this->coordonnees.x() << ", " << this->coordonnees.y() << endl;
	cout << "Vitesse : " << this->vitesse << endl;
	cout << "Depart : " << this->depart->coordonnees().x() << ", " << this->depart->coordonnees().y() << endl;
	cout << "Arrivee : " << this->arrivee->coordonnees().x() << ", " << this->arrivee->coordonnees().y() << endl;
}

float Voiture::getDistance(Point a, Point b)
{
	float x1 = a.x();
	float y1 = a.y();

	float x2 = b.x();
	float y2 = b.y();

	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}


