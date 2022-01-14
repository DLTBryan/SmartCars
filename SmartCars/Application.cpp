#include "Application.h"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qscrollarea.h>
#include <chrono>
#include <thread>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <QThread>

Application::Application(SmartCars* sc, QWidget* parent) : QMainWindow(parent) {

	QWidget* window = new QWidget();

	smart_cars = sc;
    setupHelper = new SetupSelectionHelper();
    setupHelper->fillComboBox(sc->getVoitures().size());
    setupHelper->modifyCurrentVitesseInInput(sc->getVoitures());

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
    layout->addWidget(scrollArea);

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
    vbox->addWidget(vitesseSimulationBox);
    vbox->addWidget(setupHelper->box());
    commandPrompt->setLayout(vbox);

    layout->addWidget(commandPrompt);

    window->setLayout(layout);
    setCentralWidget(window);

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
}

void Application::handleAvancer() {
    *active = !*active;
    if (*active) avancer->setText("Stopper la simulation");
    else avancer->setText("Lancer la simulation");
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
    int nvSpeedValue = setupHelper->getValueOfSpeedInput();
    //cout << nvSpeedValue << "<= NEW SPEED" << endl;
}

void Application::handleSelectCar() {
    setupHelper->modifyCurrentVitesseInInput(smart_cars->getVoitures());
    smart_cars->repaint();
}