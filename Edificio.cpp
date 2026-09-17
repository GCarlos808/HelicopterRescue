#include "Edificio.h"
#include <QRandomGenerator>

Edificio::Edificio(QGraphicsItem *parent) : Entidad(parent) {
    int variante = QRandomGenerator::global()->bounded(1, 5);
    QString ruta = QString(":/assets/obstacle1_%1.png").arg(variante);
    setPixmap(QPixmap(ruta));
}

TipoEntidad Edificio::tipo() const {
    return TipoEntidad::Obstaculo;
}