#include "voiture.h"

Voiture::Voiture(std::string nom, int vitesse, Noeud* depart)
{
	this->nom = nom;
	this->vitesse = vitesse;
	this->depart = depart;
	this->arrivee = depart->getVoisins().at(1);
	this->coordonnees = depart->getCoordonnees();
}

void Voiture::avancer(int multiplicateur)
{
	Point depart = this->coordonnees;
	Point arrivee = this->arrivee->getCoordonnees();


	float distance = getDistance(depart, this->arrivee->getCoordonnees());



}

void Voiture::affichage() {
	std::cout << "--- Affichage ---" << this->nom << std::endl;
	std::cout << "Nom : " << this->nom << std::endl;
	std::cout << "Coordonnées : " << this->coordonnees.getX() << ", " << this->coordonnees.getY() << std::endl;
	std::cout << "Vitesse : " << this->vitesse << std::endl;
	std::cout << "Depart : " << this->depart->getCoordonnees().getX() << ", " << this->depart->getCoordonnees().getY() << std::endl;
	std::cout << "Arrivee : " << this->arrivee->getCoordonnees().getX() << ", " << this->arrivee->getCoordonnees().getY() << std::endl;
}

float Voiture::getDistance(Point a, Point b)
{
	float x1 = a.getX();
	float y1 = a.getY();

	float x2 = b.getX();
	float y2 = b.getY();

	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}


