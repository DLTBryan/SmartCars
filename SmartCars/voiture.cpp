#include "voiture.h"

Voiture::Voiture(string nom, int vitesse, Noeud* depart)
{
	this->nom = nom;
	this->vitesse = vitesse;
	this->noeudDepart = depart;
	int rd = random(0, depart->voisins().size() - 1);
	this->noeudArrivee = depart->voisins().at(rd);
	this->coordonnees = depart->coordonnees();
}

void Voiture::avancer(int multiplicateur, float distanceRestance)
{
	Point depart = this->coordonnees;

	float distance = getDistance(depart, this->noeudArrivee->coordonnees()) * 1000;

	float distanceAparcourir = distanceRestance;

	if (distanceAparcourir == 0) {
		distanceAparcourir = this->vitesse * multiplicateur;
	}

	float pourcentageParcouru = distanceAparcourir / distance;

	//cout << "distanceAparcourir : " << distanceAparcourir;

	//cout << ", distance : " << distance;

	//cout << ", % : " << pourcentageParcouru << endl;

	if (pourcentageParcouru >= 1) {
		Noeud* tmp = noeudArrivee;

		this->noeudArrivee = this->noeudDepart;

		//cout << "voisins : " << tmp->voisins().size() << endl;

		if (tmp->voisins().size() != 1) {
			while (this->noeudArrivee == this->noeudDepart || this->noeudArrivee == tmp) {
				int rd = random(0, tmp->voisins().size() - 1);
				this->noeudArrivee = tmp->voisins().at(rd);
			}
		}
		else {
			this->noeudArrivee = this->noeudDepart;
		}


		this->noeudDepart = tmp;

		this->coordonnees = this->noeudDepart->coordonnees();

		//cout << "new temp coords : " << this->coordonnees.x() << ", " << this->coordonnees.y() << endl;

		if(pourcentageParcouru > 1) this->avancer(multiplicateur, distanceAparcourir - distance);
	}
	else {
		this->coordonnees = getCoordsFromPercentage(depart, this->noeudArrivee, pourcentageParcouru);
		//cout << "new coords : " << this->coordonnees.x() << ", " << this->coordonnees.y() << endl;
	}

	//Reflechir au ratio vitesse distance
}

void Voiture::affichage() {
	//cout << "--- Affichage ---" << this->nom << endl;
	//cout << "Nom : " << this->nom << endl;
	cout << "Coords : " << this->coordonnees.x() << ", " << this->coordonnees.y() << endl;
	//cout << "Vitesse : " << this->vitesse << endl;
	cout << "Depart : " << this->noeudDepart->coordonnees().x() << ", " << this->noeudDepart->coordonnees().y() << endl;
	cout << "Arrivee : " << this->noeudArrivee->coordonnees().x() << ", " << this->noeudArrivee->coordonnees().y() << endl;
}

float Voiture::getDistance(Point a, Point b)
{
	float x1 = a.x();
	float y1 = a.y();

	float x2 = b.x();
	float y2 = b.y();

	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

Point Voiture::getCoordsFromPercentage(Point depart, Noeud* arrivee, float pourcentage) {
	float lat1 = depart.x();
	float long1 = depart.y();
	float lat2 = arrivee->coordonnees().x();
	float long2 = arrivee->coordonnees().y();

	return Point(lat1 + (lat2 - lat1) * pourcentage, long1 + (long2 - long1) * pourcentage);
}

int Voiture::random(int min, int max)
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}