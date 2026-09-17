// Drone.h
#ifndef DRONE_H
#define DRONE_H

#include "Entidad.h"

class Helicoptero;

class Drone : public Entidad {
public:
    explicit Drone(Helicoptero *objetivo, QGraphicsItem *parent = nullptr);
    void actualizar(qreal deltaTime) override;
    TipoEntidad tipo() const override;

private:
    Helicoptero *objetivo;
    qreal velocidadSeguimiento;
};

#endif // DRONE_H