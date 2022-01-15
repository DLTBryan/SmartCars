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

class SetupSelectionHelper
{
public:
	SetupSelectionHelper();
	QGroupBox* box() const;
	QPushButton* buttonSelectCar() const { return d_btnSelect; };
	QPushButton* buttonModification() const { return d_btn; };
	void fillComboBox(vector<Voiture*> voitures);
	void indexComboBox(int nvIndex) {
		d_indexSelectedCar = nvIndex;
	}
	int indexComboBox() const{
		return d_indexSelectedCar;
	}
	
	string getValueOfSpeedInput() {
		return d_speedInput->text().toStdString();
	}

	int getCurrentIndexCombo() {
		return d_comboBoxListCars->currentIndex();
	}
	
	void setValueOfInput(int value) {
		d_speedInput->setText(QString(to_string(value).c_str()));
	}

	void modifyCurrentVitesseInInput(vector<Voiture*> voitures);

	void fillVoisinsLayout(vector<Voiture*> voitures);

private:
	QGroupBox* d_box;
	QLineEdit* d_speedInput;
	QComboBox* d_comboBoxListCars;
	QPushButton* d_btn;
	QPushButton* d_btnSelect;

	QListWidget* d_voisinsList;

	int d_indexSelectedCar = 0;
};

