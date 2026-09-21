#include "Soldado.h"

Soldado::Soldado(int nivel, QGraphicsItem *parent)
    : Entidad(parent)
    , tiempoDesdeUltimoDisparo(0.0)
    , intervaloDisparo(2.5) //dps ajustable en calibracion
{
    QString ruta = QString(":/assets/soldier%1_1.png").arg(nivel);
    setPixmap(QPixmap(ruta).scaled(45, 65, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Soldado::actualizar(qreal deltaTime) {
    setX(x() - velocidadDesplazamiento * deltaTime);
    tiempoDesdeUltimoDisparo += deltaTime;
}

TipoEntidad Soldado::tipo() const {
    return TipoEntidad::Enemigo;
}

bool Soldado::listoParaDisparar() const {
    return tiempoDesdeUltimoDisparo >= intervaloDisparo;
}

void Soldado::reiniciarCooldownDisparo() {
    tiempoDesdeUltimoDisparo = 0.0;
}
QPointF Soldado::origenDisparo() const {
    return QPointF(x(), y() + pixmap().height() / 2.0);
}