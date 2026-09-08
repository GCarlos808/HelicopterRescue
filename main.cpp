#include "NucleoFisico.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    NucleoFisico ventana;
    ventana.setFixedSize(900, 580);
    ventana.show();

    return QApplication::exec();
}