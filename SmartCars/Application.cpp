#include "Application.h"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qgroupbox.h>
#include <chrono>
#include <thread>

Application::Application(SmartCars* sc, QWidget* parent) : QMainWindow(parent) {

	QWidget* window = new QWidget();

	smart_cars = sc;
	avancer = new QPushButton("Avancer");

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

    QGroupBox* metaBox = new QGroupBox();
    metaBox->setTitle(QString("Parameters"));

    QGroupBox* boxGeneration = new QGroupBox();
    boxGeneration->setTitle(QString("Generation"));

    // Partie génération
    nbrevoituresagenerer = new QLineEdit;
    nbrevoituresagenerer->setPlaceholderText("Number of cars to generate...");
    nbrevoituresagenerer->setFocus();
    QPushButton* buttoninitialisation = new QPushButton("Generate");
    // Connect button signal to appropriate slot
    connect(buttoninitialisation, &QPushButton::released, this, &Application::handleGenerateCars);
    QVBoxLayout* layoutinitialisation = new QVBoxLayout;
    layoutinitialisation->addWidget(nbrevoituresagenerer);
    layoutinitialisation->addWidget(buttoninitialisation);
    layoutinitialisation->addStretch(1);
    boxGeneration->setLayout(layoutinitialisation);


    QGroupBox* boxSelection = new QGroupBox();
    boxSelection->setTitle(QString("Simulation"));
    QVBoxLayout* vboxSelection = new QVBoxLayout;
    vboxSelection->addWidget(avancer);
    vboxSelection->addStretch(1);
    boxSelection->setLayout(vboxSelection);

    QVBoxLayout* vboxMeta = new QVBoxLayout;
    vboxMeta->addWidget(boxGeneration);
    vboxMeta->addWidget(boxSelection);
    vboxMeta->addStretch(1);
    metaBox->setLayout(vboxMeta);

    layout->addWidget(metaBox);

    window->setLayout(layout);
    window->setMinimumSize(800, 500);
    setCentralWidget(window);

	//connect(avancer, &QPushButton::clicked, this, &Application::handleAvancer);

    connect(avancer, &QPushButton::clicked, this, &Application::handleAvancer);

	return;
}

void Application::handleAvancer() {
    //connect(&workerThread, &QThread::finished, workerSimulation, &QObject::deleteLater);
    //connect(this, &operate, workerSimulation, &WorkerSimulation::doWork);
    //workerThread.start();
    WorkerController* workerController = new WorkerController;
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
        voitures.push_back(new Voiture("Voiture " + i, vitesse, rues.at(rue).noeuds().at(noeud)));
    }
    smart_cars->setVoitures(voitures);
    smart_cars->repaint();
}
