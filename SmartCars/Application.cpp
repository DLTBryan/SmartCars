#include "Application.h"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qscrollarea.h>
#include <chrono>
#include <thread>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <QThread>
#include <iostream>


Application::Application(SmartCars* sc, QWidget* parent) : QMainWindow(parent) {

	QWidget* window = new QWidget();

	smart_cars = sc;
    setupHelper = new SetupSelectionHelper();

    // Cr�ation de la zone d'affichage de la carte
    QScrollArea* mapArea = new QScrollArea();
    int scrollbarWidth = 25; // +25px pour scrollbar
    int scrollMaxWidth = smart_cars->getHexMeshWidth() + scrollbarWidth;
    int scrollMaxHeight = smart_cars->getHexMeshHeight() + scrollbarWidth;
    mapArea->setWidget(smart_cars);
    mapArea->setMaximumSize(scrollMaxWidth, scrollMaxHeight);
    mapArea->setWidgetResizable(true);
    mapArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mapArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // Cr�ation de la partie gestion de vitesse de simulation
    QGroupBox* vitesseSimulationBox = new QGroupBox(tr("Changement de vitesse de simulation"));

    decrementSpeed = new QPushButton("-");
    incrementSpeed = new QPushButton("+");

    QHBoxLayout* vitesseSimulationHLayout = new QHBoxLayout();
    QVBoxLayout* vitesseSimulationVLayout = new QVBoxLayout();

    QString vitesseString = QString("Vitesse actuelle : %1").arg(smart_cars->getVitesse());

    QVBoxLayout* vitesseBox = new QVBoxLayout();

    vitesseSimulationHLayout->addWidget(decrementSpeed);
    vitesseSimulationHLayout->addWidget(incrementSpeed);

    vitesseLabel = new QLabel(vitesseString, this);

    vitesseSimulationVLayout->addWidget(vitesseLabel);
    vitesseSimulationVLayout->addLayout(vitesseSimulationHLayout);

    vitesseBox->addLayout(vitesseSimulationVLayout);

    // Cr�ation de la partie panneau de contr�le
    commandPrompt = new QGroupBox(tr(u8"Panneau de contr�le"));
    QVBoxLayout* commandPromptLayout = new QVBoxLayout();

    // Ajout du bouton de lancement de simulation
    lancerSimulation = new QPushButton("Lancer la simulation");
    lancerSimulation->setStyleSheet("font: bold;");
    commandPromptLayout->addWidget(lancerSimulation);
    
    // Partie g�n�ration de solution
    QGroupBox* generationBox = new QGroupBox();
    generationBox->setTitle(QString(u8"G�n�rateur de solution"));

    inputSizeGeneration = new QLineEdit();
    inputSizeGeneration->setPlaceholderText(u8"Nombre de voitures � g�n�rer...");
    inputSizeGeneration->setFocus();

    generateNewSimulation = new QPushButton(u8"G�n�rer");

    QVBoxLayout* generationLayout = new QVBoxLayout;

    generationLayout->addWidget(inputSizeGeneration);
    generationLayout->addWidget(generateNewSimulation);

    generationBox->setLayout(generationLayout);

    commandPromptLayout->addLayout(vitesseBox);

    // Partie S�lection et modification
    commandPromptLayout->addWidget(setupHelper->box());

    commandPrompt->setLayout(commandPromptLayout);

    commandPrompt->setDisabled(true);

    QVBoxLayout* commandesLayout = new QVBoxLayout();
    commandesLayout->addWidget(generationBox);
    commandesLayout->addWidget(commandPrompt);

    // Cr�ation du layout g�n�ral
    QHBoxLayout* globalLayout = new QHBoxLayout(window);
    globalLayout->addWidget(mapArea, 10);
    globalLayout->addLayout(commandesLayout);

    window->setLayout(globalLayout);
    setCentralWidget(window);

    //window->setMinimumSize(1150, 700);
    resize(1200, 700);
    setMaximumSize(1200, 700);

    // Liaison de l'ensemble des inputs
    connect(lancerSimulation, &QPushButton::clicked, this, &Application::handleAvancer);
    connect(incrementSpeed, &QPushButton::clicked, this, &Application::handleSpeedSimulation);
    connect(decrementSpeed, &QPushButton::clicked, this, &Application::handleSlowSimulation);
    connect(setupHelper->buttonSelectCar(), &QPushButton::clicked, this, &Application::handleSelectCar);
    connect(setupHelper->buttonModification(), &QPushButton::clicked, this, &Application::handleChangeSpeed);
    connect(generateNewSimulation, &QPushButton::released, this, &Application::handleGenerateCars);

    // Cr�ation du thread de simulation
    SimulationThread* simulationThread = new SimulationThread();

    simulationThread->smart_cars = smart_cars;
    simulationThread->active = active;

    connect(simulationThread, &SimulationThread::needRepaint, this, &Application::repaintSmartCars);
    connect(simulationThread, &SimulationThread::finished, simulationThread, &QObject::deleteLater);

    simulationThread->start();
}

void Application::repaintSmartCars() {
    smart_cars->repaint();
    vector<Voiture*> voisins;
    for (Voiture* v : smart_cars->getVoitures()) {
        if (v->getVoisin()) {
            voisins.push_back(v);
        }
    }
    setupHelper->fillVoisinsLayout(voisins);
}

void Application::handleAvancer() {
    *active = !*active;
    if (*active) {
        lancerSimulation->setText("Stopper la simulation");
        lancerSimulation->setStyleSheet("background-color: darkred; color: white; font: bold;");
    }
    else {
        lancerSimulation->setText("Lancer la simulation");
        lancerSimulation->setStyleSheet("background-color: green; color: white; font: bold;");
    }
}

void Application::handleGenerateCars() {
    QString NBREVOITURES = inputSizeGeneration->text();
    int nbrevoitures = NBREVOITURES.toInt();
    vector<Voiture*> voitures;
    for (int i = 0; i < nbrevoitures; i++) {
        vector<Rue> rues = smart_cars->getRues();
        int rue = 0 + rand() % (rues.size() - 0);
        int noeud = 0 + rand() % (rues.at(rue).noeuds().size() - 0);
        int vitesse = 10 + rand() % (30 - 10) + 1;
        string nom = "Voiture " + to_string(i);
        voitures.push_back(new Voiture(nom, vitesse, rues.at(rue).noeuds().at(noeud)));
    }
    smart_cars->setVoitures(voitures);
    for (Cell* c : smart_cars->getAllCells()) {
        c->setSelected(0);
    }
    setupHelper->fillComboBox(voitures);
    setupHelper->modifyCurrentVitesseInInput(smart_cars->getVoitures());
    commandPrompt->setDisabled(false);
    *active = false;
    lancerSimulation->setText("Lancer la simulation");
    lancerSimulation->setStyleSheet("background-color: green; color: white; font: bold;");
    smart_cars->repaint();
}

void Application::handleSpeedSimulation() {
    int vitesse = smart_cars->getVitesse() + 100;
    if (vitesse >= 600) {
        vitesse = 600;
    }
    incrementSpeed->setEnabled(vitesse < 600);
    decrementSpeed->setEnabled(vitesse > 50);
    QString vitesseString = QString("Vitesse actuelle : %1").arg(vitesse);
    vitesseLabel->setText(vitesseString);
    smart_cars->setVitesse(vitesse);
}

void Application::handleSlowSimulation() {
    int vitesse = smart_cars->getVitesse() - 100;
    if (vitesse <= 50) {
        vitesse = 50;
    }
    incrementSpeed->setEnabled(vitesse < 600);
    decrementSpeed->setEnabled(vitesse > 50);
    QString vitesseString = QString("Vitesse actuelle : %1").arg(vitesse);
    vitesseLabel->setText(vitesseString);
    smart_cars->setVitesse(vitesse);
}

void Application::handleChangeSpeed() {
    string nvSpeedValue = setupHelper->getValueOfSpeedInput();
    if (isNumber(nvSpeedValue)) {
        int nvValue = stoi(nvSpeedValue);
        smart_cars->getVoitures().at(setupHelper->getCurrentIndexCombo())->setVitesse(nvValue);
    }
    else {
        setupHelper->setValueOfInput(
            smart_cars->getVoitures().at(setupHelper->getCurrentIndexCombo())->getVitesse()
        );
    }
        
}

void Application::handleSelectCar() {
    setupHelper->modifyCurrentVitesseInInput(smart_cars->getVoitures());
    smart_cars->repaint();
}

bool Application::isNumber(const string& str)
{
    for (char const& c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}
