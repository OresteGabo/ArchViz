#include <QApplication>
#include <QMainWindow>
#include "DrawingArea.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("ArchiViz - Clean OpenGL Example");
    mainWindow.resize(1200, 800);

    DrawingArea *drawingArea = new DrawingArea(&mainWindow);
    mainWindow.setCentralWidget(drawingArea);

    mainWindow.show();
    return a.exec();
}
