#ifndef HELICOPTERO_H
#define HELICOPTERO_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Helicoptero : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    explicit Helicoptero(QGraphicsItem *parent = nullptr);
    ~Helicoptero() override;

    void actualizarFisica(qreal deltaTime);

    void setAscenso(bool activo);
    void setMoverIzquierda(bool activo);
    void setMoverDerecha(bool activo);

private:
    qreal velocidadVertical;
    qreal gravedad;
    qreal anguloInclinacion;
    bool ascensoActivo;

    qreal velocidadHorizontal;
    bool izquierdaActiva;
    bool derechaActiva;
};

#endif // HELICOPTERO_H