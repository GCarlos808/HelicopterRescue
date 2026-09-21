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

    bool recibirDano(int cantidad);
    void configurarVidaMaxima(int maxima);
    int vidaActual() const;
    int vidaMaxima() const;

    void setTurbulenciaActiva(bool activa);

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
    int vidaMaxima_;
    qreal tiempoInvulnerable;
<<<<<<< HEAD
    static const int VIDA_MAXIMA_DEFAULT = 3;
=======
    bool aterrizado;
    static const int VIDA_MAXIMA = 3;
    static constexpr qreal SUELO_Y = 480.0;
    static constexpr qreal VELOCIDAD_ATERRIZAJE_SUAVE = 200.0;

    void actualizarViento(qreal deltaTime);

    bool turbulenciaActiva;
    qreal vientoHorizontal;
    qreal vientoVertical;
    qreal tiempoParaCambioViento;
    static constexpr qreal INTERVALO_CAMBIO_VIENTO = 1.5;
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc

signals:
    void vidaCambiada(int actual, int maximo);
    void destruido();
    void aterrizajeSuave();
    void aterrizajeViolento();
};

#endif // HELICOPTERO_H
