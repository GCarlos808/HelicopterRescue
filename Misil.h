#ifndef MISIL_H
#define MISIL_H

#include "Entidad.h"

class Misil : public Entidad {
public:
    explicit Misil(qreal limiteDerechoEscena, QGraphicsItem *parent = nullptr);

    void actualizar(qreal deltaTime) override;
    bool haSalidoDePantalla(qreal limiteIzquierdo) const override;
    TipoEntidad tipo() const override;

private:
    qreal velocidadMisil;
    qreal limiteDerecho;
};

#endif // MISIL_H