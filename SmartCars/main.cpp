#include "SmartCars.h"
#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget* window = new QWidget;

    SmartCars* sc = new SmartCars();
    QPushButton* button2 = new QPushButton("Two");

    QHBoxLayout* layout = new QHBoxLayout(window);
    layout->addWidget(sc);
    layout->addWidget(button2);

    window->show();
    return a.exec();
}
