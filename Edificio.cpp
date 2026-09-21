#include "Edificio.h"
#include <QRandomGenerator>

Edificio::Edificio(int nivel, QGraphicsItem *parent) : Entidad(parent) {
    int variante = QRandomGenerator::global()->bounded(1, 5);
    QString ruta = QString(":/assets/obstacle%1_%2.png").arg(nivel).arg(variante);
    setPixmap(QPixmap(ruta));
}

TipoEntidad Edificio::tipo() const {
    return TipoEntidad::Obstaculo;
}