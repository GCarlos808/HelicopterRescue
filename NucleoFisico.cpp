#include "NucleoFisico.h"
#include "Entidad.h"
#include <QCloseEvent>
#include <QKeyEvent>

NucleoFisico::NucleoFisico(QWidget *parent) : QMainWindow(parent) {
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 900, 580);

    helicoptero = new Helicoptero();
    escena->addItem(helicoptero);

    fondo = new FondoScroll(escena, 900, 580);

    gestorEntidades = new GestorEntidades(escena, helicoptero, 900, 580);

    connect(inputManager, &InputManager::accionCambiada, this, [this](InputManager::Accion accion, bool activa) {
        if (accion == InputManager::Accion::Ascender) {
            helicoptero->setAscenso(activa);
        } else if (accion == InputManager::Accion::Izquierda) {
            helicoptero->setMoverIzquierda(activa);
        } else if (accion == InputManager::Accion::Derecha) {
            helicoptero->setMoverDerecha(activa);
        }
    });

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
        }
    });

    connect(motorJuego, &MotorJuego::tickFisica, helicoptero, &Helicoptero::actualizarFisica);

    connect(motorJuego, &MotorJuego::tickFisica, this, [this](qreal deltaTime) {
        fondo->actualizar(deltaTime);
        gestorEntidades->actualizar(deltaTime);
        gestorEntidades->intentarGenerar(deltaTime);

        Entidad *colisionado = gestorEntidades->colisionCon(helicoptero);
        if (colisionado) {
            if (colisionado->tipo() == TipoEntidad::Civil) {
                gestorEntidades->rescatar(colisionado);
                // incrementar rescatados
            } else {
                // restar salud
            }
        }
    });

    motorJuego->iniciar();
}

NucleoFisico::~NucleoFisico() = default;

void NucleoFisico::keyPressEvent(QKeyEvent *event) {
    inputManager->procesarPresion(event->key());
}

void NucleoFisico::keyReleaseEvent(QKeyEvent *event) {
    inputManager->procesarLiberacion(event->key());
}

void NucleoFisico::closeEvent(QCloseEvent *event) {
    if (motorJuego) {
        motorJuego->detener();
    }
    QMainWindow::closeEvent(event);
}