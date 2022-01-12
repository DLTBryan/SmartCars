#include "Application.h"

Application::Application(SmartCars* sc, QWidget* parent) : QMainWindow(parent)
{
    QWidget* widget = new QWidget;
    smart_cars = sc;

    avancer = new QPushButton("Avancer");

    QHBoxLayout* layout = new QHBoxLayout();

    QScrollArea* scrollArea = new QScrollArea();
    int scrollbarWidth = 25; // +25px pour scrollbar
    int scrollMaxWidth = sc->getHexMeshWidth() + scrollbarWidth;
    int scrollMaxHeight = sc->getHexMeshHeight() + scrollbarWidth;

    scrollArea->setWidget(sc);
    scrollArea->setMaximumSize(scrollMaxWidth, scrollMaxHeight);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    layout->addWidget(scrollArea);
    layout->addWidget(avancer);

    widget->setLayout(layout);

    connect(avancer, &QPushButton::clicked, this, &Application::handleAvancer);
}

void Application::handleAvancer() {
    while (1) {
        for (Voiture* v : smart_cars->getVoitures()) v->avancer(200);
        smart_cars->repaint();
        Sleep(100);
    }
}