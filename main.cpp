#include "menuwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Creating a QApplication object
    QApplication a(argc, argv);

    // Creating an instance of the MenuWindow class
    MenuWindow w;

    // Displaying the MenuWindow
    w.show();

    // Executing the application event loop
    return a.exec();
}
