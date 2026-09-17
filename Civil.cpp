#include "Civil.h"
#include <QRandomGenerator>

Civil::Civil(QGraphicsItem *parent) : Entidad(parent) {
    int variante = QRandomGenerator::global()->bounded(1, 4);
    QString ruta = QString(":/assets/rescue_%1.png").arg(variante);
    setPixmap(QPixmap(ruta).scaled(40, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

TipoEntidad Civil::tipo() const {
    return TipoEntidad::Civil;
}