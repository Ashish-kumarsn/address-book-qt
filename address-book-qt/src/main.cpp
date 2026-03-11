#include <QApplication>
#include <QStyleFactory>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    
    app.setApplicationName("AddressBook");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("AddressBookApp");

    
    app.setStyle(QStyleFactory::create("Fusion"));

    
    MainWindow window;
    window.show();

    return app.exec();
}
