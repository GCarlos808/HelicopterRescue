#include "Entidad.h"

Entidad::Entidad(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) , velocidadDesplazamiento(200.0){

}

Entidad::~Entidad() {}

void Entidad::actualizar(qreal deltaTime) {
    setPos(x() - velocidadDesplazamiento * deltaTime, y());
}

bool Entidad::haSalidoDePantalla(qreal limiteIzquierdo) const {
    return (x() + pixmap().width()) < limiteIzquierdo;
}