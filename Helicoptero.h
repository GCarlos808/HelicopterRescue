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

    void recibirDano(int cantidad);
    int vidaActual() const;
    int vidaMaxima() const;

private:
    qreal velocidadVertical;
    qreal gravedad;
    qreal anguloInclinacion;
    bool ascensoActivo;

    qreal velocidadHorizontal;
    bool izquierdaActiva;
    bool derechaActiva;

    int vida;
    qreal tiempoInvulnerable;
    static const int VIDA_MAXIMA = 3;

signals:
    void vidaCambiada(int actual, int maximo);
    void destruido();
};

#endif // HELICOPTERO_H