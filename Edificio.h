#ifndef EDIFICIO_H
#define EDIFICIO_H

#include "Entidad.h"

class Edificio : public Entidad {
public:
    explicit Edificio(QGraphicsItem *parent = nullptr);
};

#endif // EDIFICIO_H