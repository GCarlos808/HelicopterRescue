#include "MotorJuego.h"

MotorJuego::MotorJuego(QObject *parent) : QObject(parent) {
    timer.setTimerType(Qt::PreciseTimer);
    connect(&timer, &QTimer::timeout, this, &MotorJuego::tick);
}

void MotorJuego::iniciar() {
    cronometro.start();
    timer.start(16); //60 fps
}

void MotorJuego::detener() {
    timer.stop();
}

void MotorJuego::tick() {
    qreal deltaSegundos = cronometro.restart() / 1000.0;
    emit tickFisica(deltaSegundos);
}