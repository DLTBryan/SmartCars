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

	avancer = new QPushButton("Lancer la simulation");

    QScrollArea* scrollArea = new QScrollArea();
    int scrollbarWidth = 25; // +25px pour scrollbar
    int scrollMaxWidth = smart_cars->getHexMeshWidth() + scrollbarWidth;
    int scrollMaxHeight = smart_cars->getHexMeshHeight() + scrollbarWidth;
    scrollArea->setWidget(smart_cars);
    scrollArea->setMaximumSize(scrollMaxWidth, scrollMaxHeight);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    
    QHBoxLayout* layout = new QHBoxLayout(window);
    layout->addWidget(scrollArea,10);

    QGroupBox* vitesseSimulationBox = new QGroupBox(tr("Changement de vitesse de simulation"));

    plus  = new QPushButton("+");
    moins = new QPushButton("-");

    QHBoxLayout* hbox = new QHBoxLayout;
    QVBoxLayout* vbox = new QVBoxLayout();

    QString vitesseString = QString("Vitesse actuelle : %1").arg(smart_cars->getVitesse());

    vitesseLabel = new QLabel(vitesseString, this);
    vbox->addWidget(vitesseLabel);

    hbox->addWidget(moins);
    hbox->addWidget(plus);

    vbox->addLayout(hbox);

    vitesseSimulationBox->setLayout(vbox);

    QGroupBox* commandPrompt = new QGroupBox(tr("Commandes de la simulation"));
    vbox = new QVBoxLayout();
    vbox->addWidget(avancer);
    
    // Partie génération
    QGroupBox* boxGeneration = new QGroupBox();
    boxGeneration->setTitle(QString("Generation des voitures"));
    nbrevoituresagenerer = new QLineEdit;
    nbrevoituresagenerer->setPlaceholderText("Number of cars to generate...");
    nbrevoituresagenerer->setFocus();
    buttoninitialisation = new QPushButton("Generate");
    connect(buttoninitialisation, &QPushButton::released, this, &Application::handleGenerateCars);
    QVBoxLayout* layoutinitialisation = new QVBoxLayout;
    layoutinitialisation->addWidget(nbrevoituresagenerer);
    layoutinitialisation->addWidget(buttoninitialisation);
    layoutinitialisation->addStretch(1);
    boxGeneration->setLayout(layoutinitialisation);
    vbox->addWidget(boxGeneration);

    vbox->addWidget(vitesseSimulationBox);
    vbox->addWidget(setupHelper->box());
    commandPrompt->setLayout(vbox);

    layout->addWidget(commandPrompt,1);

    window->setLayout(layout);
    setCentralWidget(window);

    window->setMinimumSize(1150, 700);

    connect(avancer, &QPushButton::clicked, this, &Application::handleAvancer);
    connect(plus, &QPushButton::clicked, this, &Application::handleSpeedSimulation);
    connect(moins, &QPushButton::clicked, this, &Application::handleSlowSimulation);
    connect(setupHelper->buttonSelectCar(), &QPushButton::clicked, this, &Application::handleSelectCar);
    connect(setupHelper->buttonModification(), &QPushButton::clicked, this, &Application::handleChangeSpeed);

    SimulationThread* workerThread = new SimulationThread();

    workerThread->smart_cars = smart_cars;
    workerThread->active = active;

    connect(workerThread, &SimulationThread::needRepaint, this, &Application::repaintSmartCars);
    connect(workerThread, &SimulationThread::finished, workerThread, &QObject::deleteLater);

    workerThread->start();
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
    if (*active) avancer->setText("Stopper la simulation");
    else avancer->setText("Lancer la simulation");
}

void Application::handleGenerateCars() {
    QString NBREVOITURES = nbrevoituresagenerer->text();
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
    smart_cars->repaint();
}

void Application::handleSpeedSimulation() {
    int vitesse = smart_cars->getVitesse() + 100;
    if (vitesse >= 600) {
        vitesse = 600;
    }
    plus->setEnabled(vitesse < 600);
    moins->setEnabled(vitesse > 50);
    QString vitesseString = QString("Vitesse actuelle : %1").arg(vitesse);
    vitesseLabel->setText(vitesseString);
    smart_cars->setVitesse(vitesse);
}

void Application::handleSlowSimulation() {
    int vitesse = smart_cars->getVitesse() - 100;
    if (vitesse <= 50) {
        vitesse = 50;
    }
    plus->setEnabled(vitesse < 600);
    moins->setEnabled(vitesse > 50);
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
