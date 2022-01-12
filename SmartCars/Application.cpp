#include "Application.h"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qscrollarea.h>
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
    layout->addWidget(avancer);

    window->setLayout(layout);
    setCentralWidget(window);

	connect(avancer, &QPushButton::clicked, this, &Application::handleAvancer);
	return;
}

void Application::handleAvancer() {
    while (1) {
        for (Voiture* v : smart_cars->getVoitures()) v->avancer(200);
        smart_cars->repaint();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return;
}