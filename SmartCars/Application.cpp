#include "Application.h"
#include <QtWidgets/qboxlayout.h>

Application::Application(SmartCars* sc, QWidget* parent) : QMainWindow(parent) {

	QWidget* window = new QWidget();
	smart_cars = sc;
	avancer = new QPushButton("Test");

	QHBoxLayout* layout = new QHBoxLayout(window);

	return;
}

void Application::handleAvancer() {
	return;
}