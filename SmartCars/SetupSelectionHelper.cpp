#include "SetupSelectionHelper.h"


SetupSelectionHelper::SetupSelectionHelper() {
	d_box = new QGroupBox();
	d_speedBox = new QGroupBox();

	d_btn = new QPushButton("Modifier");
	d_box->setTitle(QString(d_title.c_str()));

	d_layoutSpeedBox = new QHBoxLayout();
	d_speedLabel = new QLabel(QString(d_labelSpeedText.c_str()));
	d_speedInput = new QLineEdit();
	d_btnSelect = new QPushButton("Choisir voiture");

	d_layoutSpeedBox->addWidget(d_speedLabel);
	d_layoutSpeedBox->addWidget(d_speedInput);
	d_layoutSpeedBox->addWidget(d_btnSelect);

	d_layoutSpeedBox->addStretch();
	d_speedBox->setLayout(d_layoutSpeedBox);

	//make meta comboBox

	d_comboBoxListCars = new QComboBox();

	d_layoutBox = new QVBoxLayout();

	d_layoutBox->addWidget(d_comboBoxListCars);
	d_layoutBox->addWidget(d_speedBox);
	d_layoutBox->addWidget(d_btn);
	d_layoutBox->addStretch(1);
	d_box->setLayout(d_layoutBox);

}


QGroupBox* SetupSelectionHelper::box() const {
	return d_box;
}

void SetupSelectionHelper::modifyCurrentVitesseInInput(vector<Voiture*> voitures) {
	indexComboBox(d_comboBoxListCars->currentIndex());
	d_speedInput->setText(QString(to_string(voitures.at(d_indexSelectedCar)->getVitesse()).c_str()));
}

void SetupSelectionHelper::fillComboBox(int nbCars) {
	d_comboBoxListCars->clear();

	for (int i = 0; i < nbCars; i++) {
		d_comboBoxListCars->addItem(QString(("Voiture Number " + to_string(i)).c_str()));
	}
}