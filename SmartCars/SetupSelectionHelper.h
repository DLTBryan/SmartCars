#pragma once
#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qgroupbox.h>
#include <QTranslator>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qcombobox.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <QtWidgets/qlineedit.h>
#include <qobject.h>
#include "Voiture.h"
#include <QtWidgets/qlistwidget.h>

using namespace std;

// Volet de selection d'une voiture dans le panneau d'administration
class SetupSelectionHelper
{
public:
	SetupSelectionHelper();
	// Recuperation de la box parente
	QGroupBox* box() const;
	// Recuperation du bouton de selection
	QPushButton* buttonSelectCar() const { return d_btnSelect; };
	// Recuperation du bouton de mise a jour de la voiture
	QPushButton* buttonModification() const { return d_btn; };
	// Remplir la comboBox avec toutes les voitures
	void fillComboBox(vector<Voiture*> voitures);
	// Definir le nouvel index
	void indexComboBox(int nvIndex) {
		d_indexSelectedCar = nvIndex;
	}
	// Recuperer l'index
	int indexComboBox() const{
		return d_indexSelectedCar;
	}
	// Recuperer la valeur de l'input de vitesse
	string getValueOfSpeedInput() {
		return d_speedInput->text().toStdString();
	}
	// Recuperer l'index courant
	int getCurrentIndexCombo() {
		return d_comboBoxListCars->currentIndex();
	}
	// Defini la valeur du champ de vitesse de la voiture
	void setValueOfInput(int value) {
		d_speedInput->setText(QString(to_string(value).c_str()));
	}
	// Modifier la vitesse de la voiture
	void modifyCurrentVitesseInInput(vector<Voiture*> voitures);
	// Remplir la liste des voisins
	void fillVoisinsLayout(vector<Voiture*> voitures);

private:
	// Box globale
	QGroupBox* d_box;
	// Input de vitesse de voiture
	QLineEdit* d_speedInput;
	// ComboBox des voitures a selectionner
	QComboBox* d_comboBoxListCars;
	// Bouton de mise a jour de la voiture
	QPushButton* d_btn;
	// Bouton de selection d'une voiture
	QPushButton* d_btnSelect;

	// Liste des voisins
	QListWidget* d_voisinsList;

	// Index courant de la voiture
	int d_indexSelectedCar = 0;
};

