#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include "SmartCars.h"
#include "SimulationThread.h"
#include "SetupSelectionHelper.h"
#include <QLabel>
#include <QtWidgets/QApplication>
#include <QCheckBox>


// Classe permettant de creer la fenêtre Qt par defaut
class Application : public QMainWindow
{
	Q_OBJECT

public:
	Application(SmartCars* sc, QWidget* parent = Q_NULLPTR);

private:
	// Bouton pour lancer la simulation
	QPushButton* lancerSimulation;
	// Bouton pour generer une nouvelle simulation
	QPushButton* generateNewSimulation;
	// Champ d'entree du nombre de voitures pour la generation
	QLineEdit* inputSizeGeneration;
	// Curseur de multiplicateur de vitesse
	QSlider* speedSlider;
	// Label d'affichage de la vitesse
	QLabel* vitesseLabel;
	// Objet de simulation de l'application
	SmartCars* smart_cars;
	// Contenu de la partie "Selection" de l'application
	SetupSelectionHelper* setupHelper;
	// Box "Selection" de l'application
	QGroupBox* commandPrompt;
	
	// Afficher ou masquer la range
	bool* showRange = new bool(true);

	// Simulation en marche ou non
	bool* active = new bool(false);

	// Est-ce que le string est un nombre
	bool isNumber(const string& str);

private slots:
	// Lancer la simulation
	void handleAvancer();
	// Lancer la generation
	void handleGenerateCars();
	// Gerer le changement de vitesse de la simulation
	void handleChangeSpeedSimulation();
	// Gerer la selection d'une voiture
	void handleSelectCar();
	// Gerer le changement de vitesse d'une voiture
	void handleChangeSpeed();
	// Fonction pour appeler la methode paint de smart_cars
	void repaintSmartCars();
	// Gerer l'affichage ou le masquage de la range
	void handleShowRange();
};

