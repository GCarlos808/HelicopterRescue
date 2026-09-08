#include "NucleoFisico.h"
#include <QKeyEvent>

NucleoFisico::NucleoFisico(QWidget *parent) : QMainWindow(parent) {
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 900, 580);

    helicoptero = new Helicoptero();
    escena->addItem(helicoptero);

    fondo = new FondoScroll(escena, 900, 580);
    gestorEntidades = new GestorEntidades(escena, 900, 580);

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

        if (gestorEntidades->hayColisionCon(helicoptero)) {
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