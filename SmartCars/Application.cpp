#include "Application.h"
#include <qlayout.h>
#include <Windows.h>

Application::Application(SmartCars* sc, QWidget* parent) : QMainWindow(parent)
{

    QWidget* widget = new QWidget;
    smart_cars = sc;
    avancer = new QPushButton("Avancer");

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(sc);
    layout->addWidget(avancer);

    widget->setLayout(layout);
    setCentralWidget(widget);

    connect(avancer, &QPushButton::clicked, this, &Application::handleAvancer);
}

void Application::handleAvancer() {
    while (1) {
        for (Voiture* v : smart_cars->getVoitures()) v->avancer(200);
        smart_cars->repaint();
        Sleep(100);
    }
}