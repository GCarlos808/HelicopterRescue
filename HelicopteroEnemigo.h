#ifndef HELICOPTEROENEMIGO_H
#define HELICOPTEROENEMIGO_H

#include "Entidad.h"
#include "Disparable.h"

class Helicoptero;

class HelicopteroEnemigo : public Entidad, public Disparable {
public:
    explicit HelicopteroEnemigo(Helicoptero *objetivo, QGraphicsItem *parent = nullptr);

    void actualizar(qreal deltaTime) override;
    TipoEntidad tipo() const override;

    bool listoParaDisparar() const override;
    void reiniciarCooldownDisparo() override;
    QPointF origenDisparo() const override;

private:
    Helicoptero *objetivo;
    qreal velocidadSeguimiento;
    qreal tiempoDesdeUltimoDisparo;
    qreal intervaloDisparo;
};

#endif // HELICOPTEROENEMIGO_H