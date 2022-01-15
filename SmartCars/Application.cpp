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
    sc->showRange = showRange;
    setupHelper = new SetupSelectionHelper();

    // Création de la zone d'affichage de la carte
    QScrollArea* mapArea = new QScrollArea();
    int scrollbarWidth = 25; // +25px pour scrollbar
    int scrollMaxWidth = smart_cars->getHexMeshWidth() + scrollbarWidth;
    int scrollMaxHeight = smart_cars->getHexMeshHeight() + scrollbarWidth;
    mapArea->setWidget(smart_cars);
    mapArea->setMaximumSize(scrollMaxWidth, scrollMaxHeight);
    mapArea->setWidgetResizable(true);
    mapArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mapArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // Création de la partie gestion de vitesse de simulation
    QGroupBox* vitesseSimulationBox = new QGroupBox(tr("Changement de vitesse de simulation"));

    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setTickInterval(2);
    speedSlider->setMinimum(1);
    speedSlider->setMaximum(31);
    speedSlider->setValue(10);

    QVBoxLayout* vitesseSimulationLayout = new QVBoxLayout();

    QString vitesseString = QString("Multiplicateur de vitesse : x%1").arg((double) smart_cars->getVitesse());

    QVBoxLayout* vitesseBox = new QVBoxLayout();

    vitesseLabel = new QLabel(vitesseString, this);

    vitesseSimulationLayout->addWidget(vitesseLabel);
    vitesseSimulationLayout->addWidget(speedSlider);

    vitesseBox->addLayout(vitesseSimulationLayout);

    QCheckBox* showRangeCheckBox = new QCheckBox(u8"Afficher la portée de la fréquence : ", this);
    showRangeCheckBox->setLayoutDirection(Qt::RightToLeft);
    showRangeCheckBox->setCheckState(Qt::Checked);

    // Création de la partie panneau de contrôle
    commandPrompt = new QGroupBox(tr(u8"Panneau de contrôle"));
    QVBoxLayout* commandPromptLayout = new QVBoxLayout();

    // Ajout du bouton de lancement de simulation
    lancerSimulation = new QPushButton("Lancer la simulation");
    lancerSimulation->setStyleSheet("font: bold;");
    commandPromptLayout->addWidget(lancerSimulation);
    
    // Partie génération de solution
    QGroupBox* generationBox = new QGroupBox();
    generationBox->setTitle(QString(u8"Générateur de solution"));

    inputSizeGeneration = new QLineEdit();
    inputSizeGeneration->setPlaceholderText(u8"Nombre de voitures à générer...");
    inputSizeGeneration->setFocus();
    inputSizeGeneration->setAlignment(Qt::AlignRight);

    generateNewSimulation = new QPushButton(u8"Générer");

    QVBoxLayout* generationLayout = new QVBoxLayout;

    generationLayout->addWidget(inputSizeGeneration);
    generationLayout->addWidget(generateNewSimulation);

    generationBox->setLayout(generationLayout);

    commandPromptLayout->addLayout(vitesseBox);
    commandPromptLayout->addWidget(showRangeCheckBox);

    // Partie Sélection et modification
    commandPromptLayout->addWidget(setupHelper->box());

    commandPrompt->setLayout(commandPromptLayout);

    commandPrompt->setDisabled(true);

    QVBoxLayout* commandesLayout = new QVBoxLayout();
    commandesLayout->addWidget(generationBox);
    commandesLayout->addWidget(commandPrompt);

    // Création du layout général
    QHBoxLayout* globalLayout = new QHBoxLayout(window);
    globalLayout->addWidget(mapArea, 10);
    globalLayout->addLayout(commandesLayout);

    window->setLayout(globalLayout);
    setCentralWidget(window);

    resize(1200, 700);
    setMaximumSize(1200, 700);

    // Liaison de l'ensemble des inputs
    connect(lancerSimulation, &QPushButton::clicked, this, &Application::handleAvancer);
    connect(speedSlider, &QSlider::sliderMoved, this, &Application::handleChangeSpeedSimulation);
    connect(setupHelper->buttonSelectCar(), &QPushButton::clicked, this, &Application::handleSelectCar);
    connect(setupHelper->buttonModification(), &QPushButton::clicked, this, &Application::handleChangeSpeed);
    connect(generateNewSimulation, &QPushButton::released, this, &Application::handleGenerateCars);
    connect(showRangeCheckBox, &QCheckBox::stateChanged, this, &Application::handleShowRange);

    // Création du thread de simulation
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

void Application::handleShowRange() {
    *showRange = !*showRange;
}

void Application::handleGenerateCars() {
    QString NBREVOITURES = inputSizeGeneration->text();
    int nbrevoitures = NBREVOITURES.toInt();
    vector<Voiture*> voitures;
    for (int i = 0; i < nbrevoitures; i++) {
        vector<Noeud*> noeuds = smart_cars->getNoeuds();
        int noeud = 0 + rand() % (noeuds.size() - 0);
        while (!noeuds.at(noeud)->estVoiturable())
            noeud = 0 + rand() % (noeuds.size() - 0);
        int vitesse = 10 + rand() % (30 - 10) + 1;
        string nom = "Voiture " + to_string(i);
        voitures.push_back(new Voiture(nom, vitesse, noeuds.at(noeud)));
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

void Application::handleChangeSpeedSimulation() {
    double vitesse = (double) speedSlider->value() / 10;
    smart_cars->setVitesse(vitesse);
    QString vitesseString = QString("Multiplicateur de vitesse : x%1").arg(smart_cars->getVitesse());
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
