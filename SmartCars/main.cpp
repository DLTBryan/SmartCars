#include "SmartCars.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SmartCars w;
    w.show();
    return a.exec();
}
