#ifndef SOLDADO_H
#define SOLDADO_H
#include "Entidad.h"

class Soldado : public Entidad {
public:
    explicit Soldado(int nivel, QGraphicsItem *parent = nullptr);
    void actualizar(qreal deltaTime) override;
    TipoEntidad tipo() const override;

    bool listoParaDisparar() const;
    void reiniciarCooldownDisparo();

private:
    qreal tiempoDesdeUltimoDisparo;
    qreal intervaloDisparo;
};
#endif // SOLDADO_H