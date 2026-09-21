#include "NucleoFisico.h"
#include "Entidad.h"
#include <QCloseEvent>
#include <QKeyEvent>
#include <QShowEvent>
#include <QFont>
#include <QMessageBox>

NucleoFisico::NucleoFisico(QWidget *parent)
    : QMainWindow(parent)
    , gestorEntidades(nullptr)
    , fondo(nullptr)
    , escena(nullptr)
    , vista(nullptr)
    , helicoptero(nullptr)
    , inputManager(nullptr)
    , motorJuego(nullptr)
    , fondoBarraVida(nullptr)
    , barraVida(nullptr)
    , textoPuntaje(nullptr)
    , textoRescatados(nullptr)
    , textoSector(nullptr)
    , textoEstado(nullptr)
    , puntaje(0)
    , rescatados(0)
    , cooldownDisparo(0.0)
    , misionIniciada(false)
    , nivelCompletado(false)
    , nivelActual(1)
{
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 900, 580);

    // Crear el fondo antes del motor
    fondo = new FondoScroll(escena, 900, 580, nivelActual);

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

    textoRescatados = escena->addText("Rescatados: 0", fuenteHud);
    textoRescatados->setDefaultTextColor(QColor(120, 220, 140));
    textoRescatados->setPos(20, 74);
    textoRescatados->setZValue(12);

    textoSector = escena->addText("Sector: 1/11", fuenteHud);
    textoSector->setDefaultTextColor(QColor(180, 210, 255));
    textoSector->setPos(20, 100);
    textoSector->setZValue(12);

    textoEstado = escena->addText("Despega para iniciar la mision", fuenteHud);
    textoEstado->setDefaultTextColor(QColor(255, 210, 90));
    textoEstado->setPos(230, 20);
    textoEstado->setZValue(12);

    connect(helicoptero, &Helicoptero::vidaCambiada, this, [this](int actual, int maximo) {
        qreal proporcion = qreal(actual) / maximo;
        barraVida->setRect(20, 20, 200 * proporcion, 20);
    });

    connect(helicoptero, &Helicoptero::destruido, this, [this]() {
        if (motorJuego) {
            motorJuego->detener();
        }
        if (fondo) {
            fondo->setScrollActivo(false);
        }
        if (gestorEntidades) {
            gestorEntidades->setGeneracionActiva(false);
        }
        if (textoEstado) {
            textoEstado->setPlainText("Mision fallida");
            textoEstado->setDefaultTextColor(QColor(255, 120, 120));
        }
    });

    connect(helicoptero, &Helicoptero::aterrizajeSuave, this, [this]() {
        if (nivelCompletado || !misionIniciada || !fondo) {
            return;
        }
        if (fondo->enZonaHangar()) {
            completarNivel();
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
        if (nivelCompletado && nivelActual > 2) {
            return;
        }
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
        if (!fondo || !gestorEntidades || !helicoptero) {
            return;
        }

        if (cooldownDisparo > 0.0) {
            cooldownDisparo -= deltaTime;
            if (cooldownDisparo < 0.0) {
                cooldownDisparo = 0.0;
            }
        }

        // Scroll solo en vuelo
        if (misionIniciada && !nivelCompletado) {
            const bool debeScrollear = !helicoptero->estaAterrizado()
            && !fondo->finDeRecorrido()
                && helicoptero->vidaActual() > 0;
            fondo->setScrollActivo(debeScrollear);
        }

        fondo->actualizar(deltaTime);

        if (misionIniciada && fondo->enZonaHangar()) {
            gestorEntidades->setGeneracionActiva(false);
        }

        gestorEntidades->actualizar(deltaTime);
        gestorEntidades->intentarGenerar(deltaTime);
        gestorEntidades->resolverImpactosMisiles();

        if (helicoptero->estaAterrizado()
            && helicoptero->vidaActual() > 0
            && !fondo->enZonaHangar()) {
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

        actualizarHudMision();
    });

    motorJuego->iniciar();
}

NucleoFisico::~NucleoFisico() {
    releaseKeyboard();
    if (motorJuego) {
        motorJuego->detener();
    }
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
    if (!helicoptero || helicoptero->vidaActual() <= 0 || !gestorEntidades) {
        return;
    }
    if (!misionIniciada || cooldownDisparo > 0.0) {
        return;
    }

    QPointF origenDisparo = helicoptero->pos()
                            + QPointF(helicoptero->pixmap().width(), helicoptero->pixmap().height() / 2.0);
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
    if (misionIniciada || !fondo || !gestorEntidades) {
        return;
    }
    misionIniciada = true;
    nivelCompletado = false;
    fondo->setScrollActivo(true);
    gestorEntidades->setGeneracionActiva(true);
    actualizarHudMision();
}

void NucleoFisico::actualizarHudMision() {
    if (!fondo) {
        return;
    }

    if (textoSector) {
        textoSector->setPlainText(
            QString("Nivel %1 | Sector: %2/%3")
                .arg(nivelActual)
                .arg(fondo->segmentoActual())
                .arg(FondoScroll::NUM_SEGMENTOS));
    }

    if (!textoEstado) {
        return;
    }

    if (nivelCompletado && nivelActual > 2) {
        textoEstado->setPlainText("Mision completada");
        textoEstado->setDefaultTextColor(QColor(120, 255, 160));
    } else if (!misionIniciada) {
        textoEstado->setPlainText("Despega para iniciar la mision");
        textoEstado->setDefaultTextColor(QColor(255, 210, 90));
    } else if (fondo->enZonaHangar()) {
        textoEstado->setPlainText("Hangar a la vista - Aterriza con suavidad");
        textoEstado->setDefaultTextColor(QColor(120, 255, 160));
    } else {
        textoEstado->setPlainText("Mision en curso - Rescata civiles");
        textoEstado->setDefaultTextColor(QColor(180, 220, 255));
    }
}

void NucleoFisico::completarNivel() {
    if (nivelCompletado || !fondo || !gestorEntidades) {
        return;
    }

    puntaje += BONUS_NIVEL;
    actualizarHudRescate();
    fondo->setScrollActivo(false);
    gestorEntidades->setGeneracionActiva(false);

    if (nivelActual == 1) {
        nivelActual = 2;
        nivelCompletado = false;
        misionIniciada = false;
        gestorEntidades->establecerNivel(2);
        fondo->establecerNivel(2);
        if (textoEstado) {
            textoEstado->setPlainText("Nivel 1 completado - Despega para el nivel 2");
            textoEstado->setDefaultTextColor(QColor(120, 255, 160));
        }
        QMessageBox::information(
            this,
            "Nivel 1 completado",
            QString("Entrega en hangar exitosa.\n\n"
                    "Civiles rescatados: %1\n"
                    "Puntos: %2\n\n"
                    "Despega para comenzar el nivel 2.")
                .arg(rescatados)
                .arg(puntaje));
        actualizarHudMision();
        return;
    }

    nivelCompletado = true;
    if (motorJuego) {
        motorJuego->detener();
    }
    if (textoEstado) {
        textoEstado->setPlainText("Mision completada");
        textoEstado->setDefaultTextColor(QColor(120, 255, 160));
    }
    QMessageBox::information(
        this,
        "Mision completada",
        QString("Has terminado la mision.\n\n"
                "Civiles rescatados: %1\n"
                "Puntos finales: %2")
            .arg(rescatados)
            .arg(puntaje));
    actualizarHudMision();
}