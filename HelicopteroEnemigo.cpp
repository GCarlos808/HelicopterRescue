#include "HelicopteroEnemigo.h"
#include "Helicoptero.h"

HelicopteroEnemigo::HelicopteroEnemigo(Helicoptero *objetivoHelicoptero, QGraphicsItem *parent)
    : Entidad(parent)
    , objetivo(objetivoHelicoptero)
    , velocidadSeguimiento(110.0)
    , tiempoDesdeUltimoDisparo(0.0)
    , intervaloDisparo(2.0)
{
    QPixmap sprite(":/assets/chopper_enemy1.png");
    if (sprite.isNull()) {
        sprite = QPixmap(90, 60);
        sprite.fill(Qt::darkRed);
    }
    setPixmap(sprite.scaled(90, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void HelicopteroEnemigo::actualizar(qreal deltaTime) {
    setX(x() - velocidadDesplazamiento * deltaTime);

    if (objetivo) {
        qreal diferenciaY = objetivo->y() - y();
        if (diferenciaY > 1.0) setY(y() + velocidadSeguimiento * deltaTime);
        else if (diferenciaY < -1.0) setY(y() - velocidadSeguimiento * deltaTime);
    }

    tiempoDesdeUltimoDisparo += deltaTime;
}

TipoEntidad HelicopteroEnemigo::tipo() const { return TipoEntidad::Enemigo; }
bool HelicopteroEnemigo::listoParaDisparar() const { return tiempoDesdeUltimoDisparo >= intervaloDisparo; }
void HelicopteroEnemigo::reiniciarCooldownDisparo() { tiempoDesdeUltimoDisparo = 0.0; }
QPointF HelicopteroEnemigo::origenDisparo() const { return QPointF(x(), y() + pixmap().height() / 2.0); }