#include "Drone.h"
#include "Helicoptero.h"

Drone::Drone(Helicoptero *objetivoHelicoptero, QGraphicsItem *parent)
    : Entidad(parent)
    , objetivo(objetivoHelicoptero)
    , velocidadSeguimiento(80.0)
{
    setPixmap(QPixmap(":/assets/drone.png").scaled(90, 55, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Drone::actualizar(qreal deltaTime) {

    setX(x() - velocidadDesplazamiento * deltaTime);

    if (objetivo) {
        qreal diferenciaY = objetivo->y() - y();
        if (diferenciaY > 1.0) {
            setY(y() + velocidadSeguimiento * deltaTime);
        } else if (diferenciaY < -1.0) {
            setY(y() - velocidadSeguimiento * deltaTime);
        }
    }
}

TipoEntidad Drone::tipo() const {
    return TipoEntidad::Enemigo;
}