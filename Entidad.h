#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QGraphicsPixmapItem>

class Entidad : public QGraphicsPixmapItem {
public:
    explicit Entidad(QGraphicsItem *parent = nullptr);
    virtual ~Entidad();

    virtual void actualizar(qreal deltaTime);
    virtual bool haSalidoDePantalla(qreal limiteIzquierdo) const;

protected:
    qreal velocidadDesplazamiento;
};

#endif // ENTIDAD_H