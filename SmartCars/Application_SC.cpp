#include "Application_SC.h"

Application_SC::Application_SC(vector<Rue> rues, vector<Voiture*> voitures, QWidget* parent) : QMainWindow(parent)
{
    QWidget* widget = new QWidget();
    smart_cars = new SmartCars(rues, voitures);

    avancer = new QPushButton("Avancer");

    QHBoxLayout* layout = new QHBoxLayout();

    QScrollArea* scrollArea = new QScrollArea();
    int scrollbarWidth = 25; // +25px pour scrollbar
    int scrollMaxWidth = smart_cars->getHexMeshWidth() + scrollbarWidth;
    int scrollMaxHeight = smart_cars->getHexMeshHeight() + scrollbarWidth;

    scrollArea->setWidget(smart_cars);
    scrollArea->setMaximumSize(scrollMaxWidth, scrollMaxHeight);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    layout->addWidget(scrollArea);
    layout->addWidget(avancer);

    widget->setLayout(layout);
    setCentralWidget(widget);

    connect(avancer, &QPushButton::clicked, this, &Application_SC::handleAvancer);
}

void Application_SC::handleAvancer() {
    while (1) {
        for (Voiture* v : smart_cars->getVoitures()) v->avancer(200);
        smart_cars->repaint();
        Sleep(100);
    }
}