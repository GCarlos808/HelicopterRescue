#ifndef EDIFICIO_H
#define EDIFICIO_H

#include "Entidad.h"

class Edificio : public Entidad {
public:
    explicit Edificio(int nivel, QGraphicsItem *parent = nullptr);
    TipoEntidad tipo() const override;
};

#endif // EDIFICIO_H