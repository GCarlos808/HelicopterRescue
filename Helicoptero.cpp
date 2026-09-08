#include "Helicoptero.h"

Helicoptero::Helicoptero(QGraphicsItem *parent)
    : QObject(nullptr)
    , QGraphicsPixmapItem(parent)
    , velocidadVertical(0.0)
    , gravedad(500.0)
    , anguloInclinacion(0.0)
    , ascensoActivo(false)
{
    QPixmap sprite(":/assets/chopper1_1.png");
    setPixmap(sprite.scaled(110, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    setPos(150, 250);
}

Helicoptero::~Helicoptero() {}

void Helicoptero::setAscenso(bool activo) {
    ascensoActivo = activo;
}

void Helicoptero::actualizarFisica(qreal deltaTime) {
    const qreal empuje = 900.0;

    qreal aceleracionNeta = gravedad;
    if (ascensoActivo) {
        aceleracionNeta -= empuje;
    }

    velocidadVertical += aceleracionNeta * deltaTime;
    setPos(x(), y() + velocidadVertical * deltaTime);

    // limites de pantalla
    if (y() < 0) { setY(0); velocidadVertical = 0; }
    if (y() > 480) { setY(480); velocidadVertical = 0; }
}