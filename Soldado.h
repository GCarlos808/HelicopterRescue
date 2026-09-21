#ifndef SOLDADO_H
#define SOLDADO_H
#include "Entidad.h"
#include "Disparable.h"

class Soldado : public Entidad, public Disparable {
public:
    explicit Soldado(int nivel, QGraphicsItem *parent = nullptr);
    void actualizar(qreal deltaTime) override;
    TipoEntidad tipo() const override;

    bool listoParaDisparar() const override;
    void reiniciarCooldownDisparo() override;
    QPointF origenDisparo() const override;

private:
    qreal tiempoDesdeUltimoDisparo;
    qreal intervaloDisparo;
};
#endif // SOLDADO_H