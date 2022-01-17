#include "SetupSelectionHelper.h"


SetupSelectionHelper::SetupSelectionHelper() {
	// Creation de la box parente
	d_box = new QGroupBox();
	d_box->setTitle(QString(u8"Selection"));

	QGroupBox* optionsBox = new QGroupBox();
	optionsBox->setTitle(QString("Options"));

	// Partie de modification des attributs de la voiture
	QVBoxLayout* optionsLayout = new QVBoxLayout();
	QHBoxLayout* speedLayout = new QHBoxLayout();
	
	d_speedInput = new QLineEdit();
	d_speedInput->setAlignment(Qt::AlignRight);
	
	speedLayout->addWidget(new QLabel(QString("Vitesse :")));
	speedLayout->addStretch();
	speedLayout->addWidget(d_speedInput);
	speedLayout->addWidget(new QLabel(QString("km/h")));

	// Bouton de mise a jour de la selection
	d_btn = new QPushButton(QString(u8"Mettre a jour"));
	optionsLayout->addLayout(speedLayout);
	// Ajouter options a modifier ici
	optionsLayout->addWidget(d_btn);
	optionsBox->setLayout(optionsLayout);
	
	// Partie de selection de la voiture a afficher
	QHBoxLayout* selectionLayout = new QHBoxLayout();

	d_comboBoxListCars = new QComboBox();
	d_comboBoxListCars->setFixedWidth(150);

	d_btnSelect = new QPushButton(u8"Selectionner");

	selectionLayout->addWidget(d_comboBoxListCars);
	selectionLayout->addWidget(d_btnSelect);

	// Partie de l'affichage des voisins de la voiture selectionnee
	QGroupBox* voisinsBox = new QGroupBox();
	voisinsBox->setTitle(QString(u8"Voitures a portee"));

	QVBoxLayout* voisinsLayout = new QVBoxLayout();
	QVBoxLayout* voisinsListLayout = new QVBoxLayout();
	d_voisinsList = new QListWidget();

	voisinsListLayout->addWidget(d_voisinsList);

	voisinsLayout->addLayout(voisinsListLayout);

	voisinsBox->setLayout(voisinsLayout);

	QVBoxLayout* layoutBox = new QVBoxLayout();

	layoutBox->addLayout(selectionLayout);
	layoutBox->addWidget(optionsBox);
	layoutBox->addWidget(voisinsBox);

	d_box->setLayout(layoutBox);
}

// Retourne la box parente
QGroupBox* SetupSelectionHelper::box() const {
	return d_box;
}

// Modifie la vitesse de la voiture selectionnee
void SetupSelectionHelper::modifyCurrentVitesseInInput(vector<Voiture*> voitures) {
	indexComboBox(d_comboBoxListCars->currentIndex());
	for (Voiture* v : voitures) v->setSelected(v == voitures.at(d_indexSelectedCar));
	d_speedInput->setText(QString(to_string(voitures.at(d_indexSelectedCar)->getVitesse()).c_str()));
}

// Met a jour la combobox avec les voitures
void SetupSelectionHelper::fillComboBox(vector<Voiture*> voitures) {
	d_comboBoxListCars->clear();

	for (Voiture* v : voitures) {
		d_comboBoxListCars->addItem(QString((v->getNom().c_str())));
	}
}

// Met a jour les voisins avec la liste des voitures
void SetupSelectionHelper::fillVoisinsLayout(vector<Voiture*> voitures) {
	d_voisinsList->clear();

	for (Voiture* v : voitures) {
		new QListWidgetItem(QString((v->getNom().c_str())), d_voisinsList);

	}
}