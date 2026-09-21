#include "MisilEnemigo.h"

MisilEnemigo::MisilEnemigo(QGraphicsItem *parent)
    : Entidad(parent)
    , velocidadMisil(350.0)
{
    setPixmap(QPixmap(":/assets/missile.png").scaled(24, 8, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MisilEnemigo::actualizar(qreal deltaTime) {
    setX(x() - velocidadMisil * deltaTime);
}

TipoEntidad MisilEnemigo::tipo() const {
    return TipoEntidad::Enemigo;
}