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

using namespace std;

class SetupSelectionHelper
{
public:
	SetupSelectionHelper();
	QGroupBox* box() const;
	QPushButton* buttonSelectCar() const { return d_btnSelect; };
	QPushButton* buttonModification() const { return d_btn; };
	void fillComboBox(int nbCars);
	void indexComboBox(int nvIndex) {
		d_indexSelectedCar = nvIndex;
	}
	int indexComboBox() const{
		return d_indexSelectedCar;
	}
	
	int getValueOfSpeedInput() {
		return d_speedInput->text().toInt();
	}

	void modifyCurrentVitesseInInput(vector<Voiture*> voitures);

private:
	QGroupBox* d_box;
	QGroupBox* d_speedBox;
	QVBoxLayout* d_layoutBox;
	QHBoxLayout* d_layoutSpeedBox;
	QLineEdit* d_speedInput;
	QLabel* d_speedLabel;
	QComboBox* d_comboBoxListCars;
	QPushButton* d_btn;
	QPushButton* d_btnSelect;
	string d_title = "Selection";
	string d_labelSpeedText = "Vitesse";

	int d_indexSelectedCar = 0;
};

