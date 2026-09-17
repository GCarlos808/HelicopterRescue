#include "NucleoFisico.h"
#include "Entidad.h"
#include <QCloseEvent>
#include <QKeyEvent>

NucleoFisico::NucleoFisico(QWidget *parent) : QMainWindow(parent) {
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 900, 580);

    helicoptero = new Helicoptero();
    escena->addItem(helicoptero);

    fondoBarraVida = escena->addRect(20, 20, 200, 20, QPen(Qt::black), QBrush(Qt::darkGray));
    barraVida = escena->addRect(20, 20, 200, 20, QPen(Qt::NoPen), QBrush(Qt::green));
    fondoBarraVida->setZValue(10);
    barraVida->setZValue(11);

    connect(helicoptero, &Helicoptero::vidaCambiada, this, [this](int actual, int maximo) {
        qreal proporcion = qreal(actual) / maximo;
        barraVida->setRect(20, 20, 200 * proporcion, 20);
    });

    connect(helicoptero, &Helicoptero::destruido, this, [this]() {
        if (motorJuego) {
            motorJuego->detener();
        }
    });

    fondo = new FondoScroll(escena, 900, 580);
    gestorEntidades = new GestorEntidades(escena, helicoptero, 900, 580);
    inputManager = new InputManager(this);

    vista = new QGraphicsView(escena, this);
    vista->setFixedSize(900, 580);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(vista);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    motorJuego = new MotorJuego(this);

    connect(inputManager, &InputManager::accionCambiada, this, [this](InputManager::Accion accion, bool activa) {
        if (accion == InputManager::Accion::Ascender) {
            helicoptero->setAscenso(activa);
        } else if (accion == InputManager::Accion::Izquierda) {
            helicoptero->setMoverIzquierda(activa);
        } else if (accion == InputManager::Accion::Derecha) {
            helicoptero->setMoverDerecha(activa);
        } else if (accion == InputManager::Accion::Disparar && activa) {
            QPointF origenDisparo = helicoptero->pos()
                + QPointF(helicoptero->pixmap().width(), helicoptero->pixmap().height() / 2.0);
            gestorEntidades->dispararMisil(origenDisparo);
        }
    });

    connect(motorJuego, &MotorJuego::tickFisica, helicoptero, &Helicoptero::actualizarFisica);

    connect(motorJuego, &MotorJuego::tickFisica, this, [this](qreal deltaTime) {
        fondo->actualizar(deltaTime);
        gestorEntidades->actualizar(deltaTime);
        gestorEntidades->intentarGenerar(deltaTime);
        gestorEntidades->resolverImpactosMisiles();

        Entidad *civilCerca = gestorEntidades->civilCercano(helicoptero, 40.0);
        if (civilCerca) {
            gestorEntidades->rescatar(civilCerca);
        }

        Entidad *peligro = gestorEntidades->colisionPeligro(helicoptero);
        if (peligro) {
            helicoptero->recibirDano(1);
            if (peligro->tipo() == TipoEntidad::Enemigo) {
                gestorEntidades->eliminarEntidad(peligro);
            }
        }
    });

    motorJuego->iniciar();
}

NucleoFisico::~NucleoFisico() {
    if (motorJuego) {
        motorJuego->detener();
    }
    // Liberar entidades antes de que la escena destruya los QGraphicsItem.
    delete gestorEntidades;
    gestorEntidades = nullptr;
    delete fondo;
    fondo = nullptr;
}

void NucleoFisico::keyPressEvent(QKeyEvent *event) {
    if (inputManager) {
        inputManager->procesarPresion(event->key());
    }
}

void NucleoFisico::keyReleaseEvent(QKeyEvent *event) {
    if (inputManager) {
        inputManager->procesarLiberacion(event->key());
    }
}

void NucleoFisico::closeEvent(QCloseEvent *event) {
    if (motorJuego) {
        motorJuego->detener();
    }
    QMainWindow::closeEvent(event);
}
