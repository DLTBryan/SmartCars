#include "hexagonal_mesh.h"

#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qscrollarea.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QMainWindow mainWindow;

    QWidget* window = new QWidget;


    Hexagonal_mesh* hexMesh = new Hexagonal_mesh();
    hexMesh->setFixedSize(hexMesh->getHexMeshWidth(), hexMesh->getHexMeshHeight());
    QPushButton* button2 = new QPushButton("Two");

    QScrollArea* scrollArea = new QScrollArea();
    int scrollbarWidth = 25; // +25px pour scrollbar
    int scrollMaxWidth = hexMesh->getHexMeshWidth() + scrollbarWidth;
    int scrollMaxHeight = hexMesh->getHexMeshHeight() + scrollbarWidth;
    scrollArea->setWidget(hexMesh);
    scrollArea->setMaximumSize(scrollMaxWidth, scrollMaxHeight); 
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QHBoxLayout* layout = new QHBoxLayout(window);
    layout->addWidget(scrollArea);
    layout->addWidget(button2);

    window->setLayout(layout);

    //mainWindow.setCentralWidget(window);
    //window->resize(640, 480);
    //mainWindow.show();

    window->show();
    return a.exec();
}
