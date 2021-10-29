#include "hexagonal_mesh.h"

#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget* window = new QWidget;

    Hexagonal_mesh* hexMesh = new Hexagonal_mesh();
    QPushButton* button2 = new QPushButton("Two");

    QHBoxLayout* layout = new QHBoxLayout(window);
    layout->addWidget(hexMesh);
    layout->addWidget(button2);

    window->show();
    return a.exec();
}
