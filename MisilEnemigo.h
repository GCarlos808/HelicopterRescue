#ifndef MISILENEMIGO_H
#define MISILENEMIGO_H
#include "Entidad.h"

class MisilEnemigo : public Entidad {
public:
    explicit MisilEnemigo(QGraphicsItem *parent = nullptr);
    void actualizar(qreal deltaTime) override;
    TipoEntidad tipo() const override;

private:
    qreal velocidadMisil;
};
#endif // MISILENEMIGO_H