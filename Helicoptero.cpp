// Helicoptero.cpp
#include "Helicoptero.h"
#include <algorithm> // std::clamp

Helicoptero::Helicoptero(QGraphicsItem *parent)
    : QObject(nullptr)
    , QGraphicsPixmapItem(parent)
    , velocidadVertical(0.0)
    , gravedad(500.0)
    , anguloInclinacion(0.0)
    , ascensoActivo(false)
    , velocidadHorizontal(250.0)
    , izquierdaActiva(false)
    , derechaActiva(false)
{
    QPixmap sprite(":/assets/chopper1_1.png");
    setPixmap(sprite.scaled(110, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setTransformOriginPoint(boundingRect().center());
    setPos(150, 250);
}

Helicoptero::~Helicoptero() {}

void Helicoptero::setAscenso(bool activo) { ascensoActivo = activo; }
void Helicoptero::setMoverIzquierda(bool activo) { izquierdaActiva = activo; }
void Helicoptero::setMoverDerecha(bool activo) { derechaActiva = activo; }

void Helicoptero::actualizarFisica(qreal deltaTime) {
    // fisica vertical (gravedad)
    const qreal empuje = 900.0;
    qreal aceleracionNeta = gravedad;
    if (ascensoActivo) aceleracionNeta -= empuje;

    velocidadVertical += aceleracionNeta * deltaTime;
    setPos(x(), y() + velocidadVertical * deltaTime);

    if (y() < 0) { setY(0); velocidadVertical = 0; }
    if (y() > 480) { setY(480); velocidadVertical = 0; }

    // movimiento horizontal
    qreal desplazamientoX = 0.0;
    if (derechaActiva) desplazamientoX += velocidadHorizontal;
    if (izquierdaActiva) desplazamientoX -= velocidadHorizontal;
    setX(x() + desplazamientoX * deltaTime);

    // inclinacion
    const qreal anguloMaximo = 15.0;
    const qreal velocidadRotacion = 90.0;

    qreal anguloObjetivo = 0.0;
    if (derechaActiva) anguloObjetivo = anguloMaximo;
    else if (izquierdaActiva) anguloObjetivo = -anguloMaximo;

    if (anguloInclinacion < anguloObjetivo) {
        anguloInclinacion = std::min(anguloInclinacion + velocidadRotacion * deltaTime, anguloObjetivo);
    } else if (anguloInclinacion > anguloObjetivo) {
        anguloInclinacion = std::max(anguloInclinacion - velocidadRotacion * deltaTime, anguloObjetivo);
    }

    setRotation(anguloInclinacion);
}