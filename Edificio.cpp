#include "Edificio.h"
#include <QRandomGenerator>

Edificio::Edificio(QGraphicsItem *parent) : Entidad(parent) {
    int variante = QRandomGenerator::global()->bounded(1, 5); // 1 a 4, random
    QString ruta = QString(":/assets/obstacle1_%1.png").arg(variante);
    setPixmap(QPixmap(ruta));
}