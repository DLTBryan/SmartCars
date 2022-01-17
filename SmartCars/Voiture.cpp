#include "voiture.h"

Voiture::Voiture(string nom, int vitesse, Noeud* depart)
{
	this->nom = nom;
	this->vitesse = vitesse;
	this->noeudDepart = depart;
	std::vector<Noeud*> noeudsvoisinsvoiturables = depart->noeudsVoisinsVoiturables();
	int rd = random(0, noeudsvoisinsvoiturables.size() - 1);
	this->noeudArrivee = noeudsvoisinsvoiturables.at(rd);
	this->coordonnees = depart->coordonnees();
}

// Avancement de la voiture sur sa route actuelle en fonction du multiplicateur en recursif
void Voiture::avancer(int multiplicateur, float distanceRestance)
{
	Point depart = this->coordonnees;

	float distance = getDistance(depart, this->noeudArrivee->coordonnees()) * 1000;

	float distanceAparcourir = distanceRestance;

	// Si c'est la première boucle recursive
	if (distanceAparcourir == 0) {
		distanceAparcourir = this->vitesse * multiplicateur;
	}

	float pourcentageParcouru = distanceAparcourir / distance;

	// Si on va plus loin que le segment actuel entre le depart et l'arrivee,
	// on change l'arrivee en depart et on prend aleatoirement un autre noeud roulable
	if (pourcentageParcouru >= 1) {
		Noeud* tmp = noeudArrivee;

		this->noeudArrivee = this->noeudDepart;

		//cout << "voisins : " << tmp->voisins().size() << endl;

		vector<Noeud*> noeudsVoisinsVoiturables = tmp->noeudsVoisinsVoiturables();

		// Si il y a plus de deux noeuds voisins on en prend un aleatoire different du courant
		if (noeudsVoisinsVoiturables.size() > 2) {
			while (this->noeudArrivee == this->noeudDepart || this->noeudArrivee == tmp) {
				int rd = random(0, noeudsVoisinsVoiturables.size() - 1);
				this->noeudArrivee = noeudsVoisinsVoiturables.at(rd);
			}
		}
		// Si il y en a deux (ligne droite) on prend l'autre
		else if (noeudsVoisinsVoiturables.size() == 2) {
			if (noeudsVoisinsVoiturables.at(0) == this->noeudDepart) {
				this->noeudArrivee = noeudsVoisinsVoiturables.at(1);
			}
			else {
				this->noeudArrivee = noeudsVoisinsVoiturables.at(0);
			}
		}
		// Sinon demi tour
		else {
			this->noeudArrivee = this->noeudDepart;
		}

		this->noeudDepart = tmp;

		this->coordonnees = this->noeudDepart->coordonnees();

		//cout << "new temp coords : " << this->coordonnees.x() << ", " << this->coordonnees.y() << endl;

		if (pourcentageParcouru > 1) this->avancer(multiplicateur, distanceAparcourir - distance);
	}
	// Si pourcentageParcouru < 1 alors on place la voiture sur le segment courant a sa nouvelle position
	else {
		this->coordonnees = getCoordsFromPercentage(depart, this->noeudArrivee, pourcentageParcouru);
		//cout << "new coords : " << this->coordonnees.x() << ", " << this->coordonnees.y() << endl;
	}
}

// Affichage console des infos de la voiture
void Voiture::affichage() {
	//cout << "--- Affichage ---" << this->nom << endl;
	//cout << "Nom : " << this->nom << endl;
	cout << "Coords : " << this->coordonnees.x() << ", " << this->coordonnees.y() << endl;
	//cout << "Vitesse : " << this->vitesse << endl;
	cout << "Depart : " << this->noeudDepart->coordonnees().x() << ", " << this->noeudDepart->coordonnees().y() << endl;
	cout << "Arrivee : " << this->noeudArrivee->coordonnees().x() << ", " << this->noeudArrivee->coordonnees().y() << endl;
}

// Recuperation de la distance euclidienne entre deux points
float Voiture::getDistance(Point a, Point b)
{
	float x1 = a.x();
	float y1 = a.y();

	float x2 = b.x();
	float y2 = b.y();

	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

// Recuperer les coordonnees entre un point de depart et d'arrivee en fonction d'un pourcentage
Point Voiture::getCoordsFromPercentage(Point depart, Noeud* arrivee, float pourcentage) {
	float lat1 = depart.x();
	float long1 = depart.y();
	float lat2 = arrivee->coordonnees().x();
	float long2 = arrivee->coordonnees().y();

	return Point(lat1 + (lat2 - lat1) * pourcentage, long1 + (long2 - long1) * pourcentage);
}

// Generation d'un nombre aleatoire [min, max]
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