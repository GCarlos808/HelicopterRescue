#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QGraphicsPixmapItem>

enum class TipoEntidad {
    Obstaculo,
    Civil,
    Enemigo
};

class Entidad : public QGraphicsPixmapItem {
public:
    explicit Entidad(QGraphicsItem *parent = nullptr);
    virtual ~Entidad();

    virtual void actualizar(qreal deltaTime);
    virtual bool haSalidoDePantalla(qreal limiteIzquierdo) const;
    virtual TipoEntidad tipo() const = 0; // obliga a cada subclase a identificarse

protected:
    qreal velocidadDesplazamiento;
};

#endif // ENTIDAD_H