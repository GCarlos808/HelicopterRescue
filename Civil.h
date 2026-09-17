#ifndef CIVIL_H
#define CIVIL_H

#include "Entidad.h"

class Civil : public Entidad {
public:
    explicit Civil(QGraphicsItem *parent = nullptr);
    TipoEntidad tipo() const override;
};

#endif // CIVIL_H