#include "MenuPrincipal.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("HelicopterRescue");
    QApplication::setApplicationDisplayName("Helicopter Rescue");

    MenuPrincipal menu;
    menu.show();

    return QApplication::exec();
}
