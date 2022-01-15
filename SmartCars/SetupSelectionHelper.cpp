#include "SetupSelectionHelper.h"


SetupSelectionHelper::SetupSelectionHelper() {
	d_box = new QGroupBox();
	d_box->setTitle(QString(u8"Sélection"));

	QGroupBox* optionsBox = new QGroupBox();
	optionsBox->setTitle(QString("Options"));

	QVBoxLayout* optionsLayout = new QVBoxLayout();
	QHBoxLayout* speedLayout = new QHBoxLayout();
	
	d_speedInput = new QLineEdit();
	d_speedInput->setAlignment(Qt::AlignRight);
	
	speedLayout->addWidget(new QLabel(QString("Vitesse :")));
	speedLayout->addStretch();
	speedLayout->addWidget(d_speedInput);
	speedLayout->addWidget(new QLabel(QString("km/h")));

	d_btn = new QPushButton(QString(u8"Mettre à jour"));
	optionsLayout->addLayout(speedLayout);
	// Ajouter options à modifier ici
	optionsLayout->addWidget(d_btn);
	optionsBox->setLayout(optionsLayout);
	
	QHBoxLayout* selectionLayout = new QHBoxLayout();

	d_comboBoxListCars = new QComboBox();
	d_comboBoxListCars->setFixedWidth(150);

	d_btnSelect = new QPushButton(u8"Sélectionner");

	selectionLayout->addWidget(d_comboBoxListCars);
	selectionLayout->addWidget(d_btnSelect);

	QGroupBox* voisinsBox = new QGroupBox();
	voisinsBox->setTitle(QString(u8"Voitures à portée"));

	QVBoxLayout* voisinsLayout = new QVBoxLayout();
	d_voisinsLayout = new QVBoxLayout();

	QScrollArea* scrollArea = new QScrollArea();
	int scrollbarWidth = 25; // +25px pour scrollbar
	scrollArea->setMaximumHeight(500);
	scrollArea->setWidgetResizable(false);
	scrollArea->setLayout(d_voisinsLayout);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	voisinsLayout->addWidget(scrollArea);

	voisinsBox->setLayout(voisinsLayout);

	QVBoxLayout* layoutBox = new QVBoxLayout();

	layoutBox->addLayout(selectionLayout);
	layoutBox->addWidget(optionsBox);
	layoutBox->addWidget(voisinsBox);

	d_box->setLayout(layoutBox);
}


QGroupBox* SetupSelectionHelper::box() const {
	return d_box;
}

void SetupSelectionHelper::modifyCurrentVitesseInInput(vector<Voiture*> voitures) {
	indexComboBox(d_comboBoxListCars->currentIndex());
	for (Voiture* v : voitures) v->setSelected(v == voitures.at(d_indexSelectedCar));
	d_speedInput->setText(QString(to_string(voitures.at(d_indexSelectedCar)->getVitesse()).c_str()));
}

void SetupSelectionHelper::fillComboBox(vector<Voiture*> voitures) {
	d_comboBoxListCars->clear();

	for (Voiture* v : voitures) {
		d_comboBoxListCars->addItem(QString((v->getNom().c_str())));
	}
}

void SetupSelectionHelper::fillVoisinsLayout(vector<Voiture*> voitures) {
	clearLayout(d_voisinsLayout);

	for (Voiture* v : voitures) {
		d_voisinsLayout->addWidget(new QLabel(v->getNom().c_str()));
	}
}

void SetupSelectionHelper::clearLayout(QLayout* layout) {
	QLayoutItem* item;
	while ((item = layout->takeAt(0))) {
		if (item->layout()) {
			clearLayout(item->layout());
			delete item->layout();
		}
		if (item->widget()) {
			delete item->widget();
		}
		delete item;
	}
}