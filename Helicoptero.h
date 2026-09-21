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

    bool estaAterrizado() const;
    qreal velocidadVerticalActual() const;

private:
    void resolverAterrizaje();

    qreal velocidadVertical;
    qreal gravedad;
    qreal anguloInclinacion;
    bool ascensoActivo;

    qreal velocidadHorizontal;
    bool izquierdaActiva;
    bool derechaActiva;

    int vida;
    qreal tiempoInvulnerable;
    bool aterrizado;
    static const int VIDA_MAXIMA = 3;
    static constexpr qreal SUELO_Y = 480.0;
    static constexpr qreal VELOCIDAD_ATERRIZAJE_SUAVE = 200.0;

signals:
    void vidaCambiada(int actual, int maximo);
    void destruido();
    void aterrizajeSuave();
    void aterrizajeViolento();
};

#endif // HELICOPTERO_H