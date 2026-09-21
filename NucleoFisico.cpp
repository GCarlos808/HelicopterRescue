#include "NucleoFisico.h"
#include "Entidad.h"
#include <QCloseEvent>
#include <QKeyEvent>
#include <QShowEvent>
#include <QFont>

    NucleoFisico::NucleoFisico(QWidget *parent)
    : QMainWindow(parent)
    , puntaje(0)
    , rescatados(0)
    , cooldownDisparo(0.0)
    , misionIniciada(false)
    , nivelCompletado(false)
    , nivelActual(1) {

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 900, 580);

    helicoptero = new Helicoptero();
    escena->addItem(helicoptero);

    fondoBarraVida = escena->addRect(20, 20, 200, 20, QPen(Qt::black), QBrush(Qt::darkGray));
    barraVida = escena->addRect(20, 20, 200, 20, QPen(Qt::NoPen), QBrush(Qt::green));
    fondoBarraVida->setZValue(10);
    barraVida->setZValue(11);

    QFont fuenteHud("Segoe UI", 14, QFont::Bold);
    textoPuntaje = escena->addText("Puntos: 0", fuenteHud);
    textoPuntaje->setDefaultTextColor(Qt::white);
    textoPuntaje->setPos(20, 48);
    textoPuntaje->setZValue(12);

    textoEstado = escena->addText("Despega para iniciar la mision", fuenteHud);
    textoEstado->setDefaultTextColor(QColor(255, 210, 90));
    textoEstado->setPos(20, 126);
    textoEstado->setZValue(12);

    textoRescatados = escena->addText("Rescatados: 0", fuenteHud);
    textoRescatados->setDefaultTextColor(QColor(120, 220, 140));
    textoRescatados->setPos(20, 74);
    textoRescatados->setZValue(12);

    connect(helicoptero, &Helicoptero::vidaCambiada, this, [this](int actual, int maximo) {
        qreal proporcion = qreal(actual) / maximo;
        barraVida->setRect(20, 20, 200 * proporcion, 20);
    });

    connect(helicoptero, &Helicoptero::destruido, this, [this]() {
        if (motorJuego) {
            motorJuego->detener();
        }
    });

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
            if (activa) {
                iniciarMision();
            }
        } else if (accion == InputManager::Accion::Izquierda) {
            helicoptero->setMoverIzquierda(activa);
        } else if (accion == InputManager::Accion::Derecha) {
            helicoptero->setMoverDerecha(activa);
        } else if (accion == InputManager::Accion::Disparar && activa) {
            intentarDisparar();
        }
    });

    connect(motorJuego, &MotorJuego::tickFisica, helicoptero, &Helicoptero::actualizarFisica);

    connect(motorJuego, &MotorJuego::tickFisica, this, [this](qreal deltaTime) {
        if (cooldownDisparo > 0.0) {
            cooldownDisparo -= deltaTime;
            if (cooldownDisparo < 0.0) {
                cooldownDisparo = 0.0;
            }
        }

        fondo->actualizar(deltaTime);
        gestorEntidades->actualizar(deltaTime);
        gestorEntidades->intentarGenerar(deltaTime);
        gestorEntidades->resolverImpactosMisiles();

        //solo se puede rescatar tras un aterrizaje suave
        if (helicoptero->estaAterrizado() && helicoptero->vidaActual() > 0) {
            Entidad *civilCerca = gestorEntidades->civilCercano(helicoptero, 70.0);
            if (civilCerca) {
                gestorEntidades->rescatar(civilCerca);
                registrarRescate();
            }
        }

        Entidad *peligro = gestorEntidades->colisionPeligro(helicoptero);
        if (peligro) {
            helicoptero->recibirDano(1);
            if (peligro->tipo() == TipoEntidad::Enemigo) {
                gestorEntidades->eliminarEntidad(peligro);
            }
        }
        if (fondo->finDeRecorrido() && !nivelCompletado) {
            completarNivel();
        }

        actualizarHudMision();
    });

    motorJuego->iniciar();
}

NucleoFisico::~NucleoFisico() {
    releaseKeyboard();
    if (motorJuego) {
        motorJuego->detener();
    }
    //liberar entidades antes de que la escena destruya los QGraphicsItem.
    delete gestorEntidades;
    gestorEntidades = nullptr;
    delete fondo;
    fondo = nullptr;
}

bool NucleoFisico::esTeclaDeJuego(int codigoTecla) const {
    return codigoTecla == Qt::Key_Up
           || codigoTecla == Qt::Key_Left
           || codigoTecla == Qt::Key_Right
           || codigoTecla == Qt::Key_Space;
}

bool NucleoFisico::event(QEvent *event) {
    //evita que Qt consuma flechas/espacio
    if (event->type() == QEvent::ShortcutOverride) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        if (esTeclaDeJuego(keyEvent->key())) {
            event->accept();
            return true;
        }
    }
    return QMainWindow::event(event);
}

void NucleoFisico::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    activateWindow();
    setFocus(Qt::OtherFocusReason);
    grabKeyboard();
}

void NucleoFisico::intentarDisparar() {
    if (!helicoptero || helicoptero->vidaActual() <= 0) {
        return;
    }
    if (cooldownDisparo > 0.0) {
        return;
    }

    QPointF origenDisparo = helicoptero->pos() + QPointF(helicoptero->pixmap().width(), helicoptero->pixmap().height() / 2.0);
    gestorEntidades->dispararMisil(origenDisparo);
    cooldownDisparo = COOLDOWN_DISPARO_SEGUNDOS;
}

void NucleoFisico::actualizarHudRescate() {
    if (textoPuntaje) {
        textoPuntaje->setPlainText(QString("Puntos: %1").arg(puntaje));
    }
    if (textoRescatados) {
        textoRescatados->setPlainText(QString("Rescatados: %1").arg(rescatados));
    }
}

void NucleoFisico::registrarRescate() {
    ++rescatados;
    puntaje += PUNTOS_POR_RESCATE;
    actualizarHudRescate();
}

int NucleoFisico::puntajeActual() const {
    return puntaje;
}

int NucleoFisico::civilesRescatados() const {
    return rescatados;
}

void NucleoFisico::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        event->accept();
        return;
    }
    if (inputManager) {
        inputManager->procesarPresion(event->key());
    }
    event->accept();
}

void NucleoFisico::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        event->accept();
        return;
    }
    if (inputManager) {
        inputManager->procesarLiberacion(event->key());
    }
    event->accept();
}

void NucleoFisico::closeEvent(QCloseEvent *event) {
    releaseKeyboard();
    if (motorJuego) {
        motorJuego->detener();
    }
    QMainWindow::closeEvent(event);
}
void NucleoFisico::iniciarMision() {
    if (misionIniciada) return;
    misionIniciada = true;
    fondo->setScrollActivo(true);
    gestorEntidades->setGeneracionActiva(true);
    actualizarHudMision();
}
void NucleoFisico::actualizarHudMision() {
    if (textoSector) {
        textoSector->setPlainText(QString("Sector: %1/%2").arg(fondo->segmentoActual()).arg(FondoScroll::NUM_SEGMENTOS));
    }
    if (textoEstado) {
        if (nivelCompletado) {
            textoEstado->setPlainText("Nivel completado");
        } else if (!misionIniciada) {
            textoEstado->setPlainText("Despega para iniciar la mision");
        } else {
            textoEstado->setPlainText("Mision en curso");
        }
    }
}
void NucleoFisico::completarNivel() {
    if (nivelCompletado) return;
    nivelCompletado = true;
    puntaje += BONUS_NIVEL;

    if (nivelActual == 1) {
        nivelActual = 2;
        nivelCompletado = false;
        misionIniciada = false;
        gestorEntidades->setGeneracionActiva(false);
        gestorEntidades->establecerNivel(2);
        fondo->establecerNivel(2);
    } else {
        gestorEntidades->setGeneracionActiva(false);
        motorJuego->detener(); //nivel 3
    }

    actualizarHudRescate();
    actualizarHudMision();
}