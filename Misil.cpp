#include "Misil.h"

Misil::Misil(qreal limiteDerechoEscena, QGraphicsItem *parent)
    : Entidad(parent)
    , velocidadMisil(600.0)
    , limiteDerecho(limiteDerechoEscena)
{
    setPixmap(QPixmap(":/assets/missile.png").scaled(30, 10, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Misil::actualizar(qreal deltaTime) {
    setX(x() + velocidadMisil * deltaTime);
}

bool Misil::haSalidoDePantalla(qreal limiteIzquierdo) const {
    Q_UNUSED(limiteIzquierdo);
    return x() > limiteDerecho;
}

TipoEntidad Misil::tipo() const {
    return TipoEntidad::Misil;
}