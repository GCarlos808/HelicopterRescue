#include "NucleoFisico.h"
#include "Entidad.h"
#include "PantallaResultado.h"
#include "ProgressManager.h"
#include "ScoreManager.h"

#include <QCloseEvent>
#include <QKeyEvent>
#include <QSoundEffect>
#include <QUrl>
#include <algorithm>

namespace {
constexpr int puntosPorRescate = 100;
constexpr int puntosPorDrone = 150;
constexpr int bonusNivelCompleto = 250;
constexpr int bonusFaseHistoria = 200;
constexpr int vidaHistoria = 8;
constexpr int nivelMaximoJuego = 4;

int metaParaNivel(int nivel)
{
    switch (nivel) {
    case 2: return 5;
    case 3: return 7;
    case 4: return 0; // historia usa fases
    default: return 3;
    }
}

qreal intervaloParaNivel(int nivel)
{
    switch (nivel) {
    case 2: return 1.4;
    case 3: return 1.0;
    case 4: return 1.6;
    default: return 2.0;
    }
}
}

NucleoFisico::NucleoFisico(const QString& nombrePiloto,
                           int nivel,
                           ScoreManager* gestorPuntajes,
                           ProgressManager* progreso,
                           QWidget *parent)
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
    , textoHud(nullptr)
    , textoHistoria(nullptr)
    , sonidoChoqueEdificio(nullptr)
    , gestorPuntajes_(gestorPuntajes)
    , progreso_(progreso)
    , nombrePiloto_(nombrePiloto)
    , nivelActual_(std::clamp(nivel, 1, nivelMaximoJuego))
    , metaRescates_(metaParaNivel(nivelActual_))
    , metaDrones_(0)
    , rescatados_(0)
    , dronesDestruidos_(0)
    , rescatadosFase_(0)
    , dronesFase_(0)
    , faseHistoria_(0)
    , puntaje_(0)
    , tiempoPartida_(0.0)
    , partidaTerminada_(false)
    , modoHistoria_(nivelActual_ == 4)
{
    const QString tituloNivel = modoHistoria_
        ? QString("Historia")
        : QString("Nivel %1").arg(nivelActual_);
    setFixedSize(900, 580);
    setWindowTitle(QString("Helicopter Rescue - %1 | %2")
                       .arg(nombrePiloto_)
                       .arg(tituloNivel));

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 900, 580);

    helicoptero = new Helicoptero();
    escena->addItem(helicoptero);

    fondoBarraVida = escena->addRect(20, 20, 200, 20, QPen(Qt::black), QBrush(Qt::darkGray));
    barraVida = escena->addRect(20, 20, 200, 20, QPen(Qt::NoPen), QBrush(Qt::green));
    fondoBarraVida->setZValue(10);
    barraVida->setZValue(11);

    textoHud = escena->addText("");
    textoHud->setDefaultTextColor(Qt::white);
    textoHud->setPos(20, 48);
    textoHud->setZValue(12);
    QFont fuenteHud = textoHud->font();
    fuenteHud.setPointSize(11);
    fuenteHud.setBold(true);
    textoHud->setFont(fuenteHud);

    textoHistoria = escena->addText("");
    textoHistoria->setDefaultTextColor(QColor("#ffd166"));
    textoHistoria->setPos(20, 72);
    textoHistoria->setZValue(12);
    QFont fuenteHistoria = textoHistoria->font();
    fuenteHistoria.setPointSize(10);
    fuenteHistoria.setBold(true);
    textoHistoria->setFont(fuenteHistoria);
    textoHistoria->setTextWidth(860);

    connect(helicoptero, &Helicoptero::vidaCambiada, this, [this](int actual, int maximo) {
        qreal proporcion = qreal(actual) / maximo;
        barraVida->setRect(20, 20, 200 * proporcion, 20);
    });

    connect(helicoptero, &Helicoptero::destruido, this, [this]() {
        alPerder();
    });

    fondo = new FondoScroll(escena, 900, 580);
    gestorEntidades = new GestorEntidades(escena, helicoptero, 900, 580);
    configurarNivel(nivelActual_);
    actualizarHud();
    inputManager = new InputManager(this);

    sonidoChoqueEdificio = new QSoundEffect(this);
    sonidoChoqueEdificio->setSource(QUrl("qrc:/assets/hit_building.wav"));
    sonidoChoqueEdificio->setVolume(0.85f);

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
        if (partidaTerminada_) {
            return;
        }
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
        if (partidaTerminada_) {
            return;
        }

        tiempoPartida_ += deltaTime;
        fondo->actualizar(deltaTime);
        gestorEntidades->actualizar(deltaTime);
        gestorEntidades->intentarGenerar(deltaTime);

        if (gestorEntidades->resolverImpactosMisiles()) {
            dronesDestruidos_++;
            dronesFase_++;
            puntaje_ += puntosPorDrone;
            if (modoHistoria_) {
                revisarObjetivosHistoria();
            }
        }

        Entidad *civilCerca = gestorEntidades->civilCercano(helicoptero, 40.0);
        if (civilCerca) {
            gestorEntidades->rescatar(civilCerca);
            rescatados_++;
            rescatadosFase_++;
            puntaje_ += puntosPorRescate;
            if (modoHistoria_) {
                revisarObjetivosHistoria();
            } else if (rescatados_ >= metaRescates_) {
                alCompletarNivel();
                return;
            }
        }

        Entidad *peligro = gestorEntidades->colisionPeligro(helicoptero);
        if (peligro) {
            const bool aplicoDano = helicoptero->recibirDano(1);
            if (aplicoDano && peligro->tipo() == TipoEntidad::Obstaculo && sonidoChoqueEdificio) {
                sonidoChoqueEdificio->play();
            }
            if (peligro->tipo() == TipoEntidad::Enemigo) {
                gestorEntidades->eliminarEntidad(peligro);
                dronesDestruidos_++;
                dronesFase_++;
                puntaje_ += puntosPorDrone / 2;
                if (modoHistoria_) {
                    revisarObjetivosHistoria();
                }
            }
        }

        actualizarHud();
    });

    motorJuego->iniciar();
}

NucleoFisico::~NucleoFisico() {
    if (motorJuego) {
        motorJuego->detener();
    }
    delete gestorEntidades;
    gestorEntidades = nullptr;
    delete fondo;
    fondo = nullptr;
}

void NucleoFisico::configurarNivel(int nivel)
{
    modoHistoria_ = (nivel == 4);
    metaRescates_ = metaParaNivel(nivel);
    metaDrones_ = 0;

    if (gestorEntidades) {
        gestorEntidades->setIntervaloSpawn(intervaloParaNivel(nivel));
        gestorEntidades->setEdificiosAltos(false);
        gestorEntidades->setModoGeneracion(ModoGeneracion::Mixto);
    }

    if (modoHistoria_) {
        if (helicoptero) {
            helicoptero->configurarVidaMaxima(vidaHistoria);
        }
        configurarFaseHistoria(1);
    } else {
        if (helicoptero) {
            helicoptero->configurarVidaMaxima(3);
        }
        objetivoActual_.clear();
        if (textoHistoria) {
            textoHistoria->setPlainText("");
        }
    }
}

void NucleoFisico::configurarFaseHistoria(int fase)
{
    faseHistoria_ = fase;
    rescatadosFase_ = 0;
    dronesFase_ = 0;

    switch (fase) {
    case 1:
        metaRescates_ = 4;
        metaDrones_ = 0;
        objetivoActual_ = "Fase 1 — Evacuacion urbana: rescata 4 civiles. Cuidado con los edificios.";
        if (gestorEntidades) {
            gestorEntidades->setModoGeneracion(ModoGeneracion::Evacuacion);
            gestorEntidades->setIntervaloSpawn(1.7);
            gestorEntidades->setEdificiosAltos(false);
        }
        break;
    case 2:
        metaRescates_ = 0;
        metaDrones_ = 3;
        objetivoActual_ = "Fase 2 — Amenaza aerea: destruye 3 drones enemigos (Espacio = misil).";
        if (gestorEntidades) {
            gestorEntidades->setModoGeneracion(ModoGeneracion::CombateAereo);
            gestorEntidades->setIntervaloSpawn(1.3);
            gestorEntidades->setEdificiosAltos(true);
        }
        break;
    case 3:
    default:
        metaRescates_ = 3;
        metaDrones_ = 2;
        objetivoActual_ = "Fase 3 — Extraccion final: rescata 3 civiles y derriba 2 drones.";
        if (gestorEntidades) {
            gestorEntidades->setModoGeneracion(ModoGeneracion::Extraccion);
            gestorEntidades->setIntervaloSpawn(1.1);
            gestorEntidades->setEdificiosAltos(true);
        }
        break;
    }

    if (textoHistoria) {
        textoHistoria->setPlainText(objetivoActual_);
    }
    actualizarHud();
}

void NucleoFisico::revisarObjetivosHistoria()
{
    if (!modoHistoria_ || partidaTerminada_) {
        return;
    }

    const bool rescatesListos = (metaRescates_ <= 0) || (rescatadosFase_ >= metaRescates_);
    const bool dronesListos = (metaDrones_ <= 0) || (dronesFase_ >= metaDrones_);

    if (!(rescatesListos && dronesListos)) {
        return;
    }

    puntaje_ += bonusFaseHistoria;

    if (faseHistoria_ < 3) {
        configurarFaseHistoria(faseHistoria_ + 1);
    } else {
        alCompletarNivel();
    }
}

void NucleoFisico::actualizarHud()
{
    if (!textoHud) {
        return;
    }

    if (modoHistoria_) {
        QString progresoFase;
        if (metaRescates_ > 0) {
            progresoFase += QString("Rescates %1/%2  ").arg(rescatadosFase_).arg(metaRescates_);
        }
        if (metaDrones_ > 0) {
            progresoFase += QString("Drones %1/%2  ").arg(dronesFase_).arg(metaDrones_);
        }

        textoHud->setPlainText(
            QString("HISTORIA  Fase %1/3   %2  Vida %3/%4   Puntaje %5   Tiempo %6s")
                .arg(faseHistoria_)
                .arg(progresoFase.trimmed())
                .arg(helicoptero ? helicoptero->vidaActual() : 0)
                .arg(helicoptero ? helicoptero->vidaMaxima() : 0)
                .arg(puntaje_)
                .arg(tiempoPartida_, 0, 'f', 1));
    } else {
        textoHud->setPlainText(
            QString("Nivel %1   Rescates %2/%3   Puntaje %4   Tiempo %5s")
                .arg(nivelActual_)
                .arg(rescatados_)
                .arg(metaRescates_)
                .arg(puntaje_)
                .arg(tiempoPartida_, 0, 'f', 1));
    }
}

void NucleoFisico::registrarPuntaje()
{
    if (!gestorPuntajes_) {
        return;
    }
    ScoreEntry entrada;
    entrada.playerName = nombrePiloto_.toStdString();
    entrada.score = puntaje_;
    entrada.timeSeconds = tiempoPartida_;
    entrada.level = nivelActual_;
    gestorPuntajes_->agregarPuntaje(entrada);
    gestorPuntajes_->guardarPuntajes();
}

void NucleoFisico::alPerder()
{
    if (partidaTerminada_) {
        return;
    }
    partidaTerminada_ = true;
    if (motorJuego) {
        motorJuego->detener();
    }
    registrarPuntaje();
    mostrarPantallaResultado(0);
}

void NucleoFisico::alCompletarNivel()
{
    if (partidaTerminada_) {
        return;
    }
    partidaTerminada_ = true;
    if (motorJuego) {
        motorJuego->detener();
    }

    puntaje_ += bonusNivelCompleto;
    actualizarHud();
    registrarPuntaje();

    if (progreso_) {
        progreso_->seleccionarPiloto(nombrePiloto_.toStdString());
        progreso_->desbloquearNivel(std::min(nivelMaximoJuego, nivelActual_ + 1));
    }

    if (nivelActual_ >= nivelMaximoJuego) {
        mostrarPantallaResultado(2);
    } else {
        mostrarPantallaResultado(1);
    }
}

void NucleoFisico::mostrarPantallaResultado(int tipoCodigo)
{
    PantallaResultado::Tipo tipo = PantallaResultado::Tipo::Derrota;
    if (tipoCodigo == 1) {
        tipo = PantallaResultado::Tipo::SiguienteNivel;
    } else if (tipoCodigo == 2) {
        tipo = PantallaResultado::Tipo::VictoriaFinal;
    }

    auto* dialogo = new PantallaResultado(
        tipo, nombrePiloto_, nivelActual_, puntaje_, rescatados_, tiempoPartida_, this);
    dialogo->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialogo, &PantallaResultado::reiniciarSolicitado, this, [this, dialogo]() {
        emit solicitarReiniciar(nivelActual_);
        dialogo->accept();
        close();
    });
    connect(dialogo, &PantallaResultado::continuarSolicitado, this, [this, dialogo]() {
        emit solicitarNivel(nivelActual_ + 1);
        dialogo->accept();
        close();
    });
    connect(dialogo, &PantallaResultado::nivelesSolicitados, this, [this, dialogo]() {
        emit solicitarNiveles();
        dialogo->accept();
        close();
    });
    connect(dialogo, &PantallaResultado::menuSolicitado, this, [this, dialogo]() {
        emit solicitarMenu();
        dialogo->accept();
        close();
    });

    dialogo->move(geometry().center() - dialogo->rect().center());
    dialogo->open();
}

void NucleoFisico::keyPressEvent(QKeyEvent *event) {
    if (inputManager && !partidaTerminada_) {
        inputManager->procesarPresion(event->key());
    }
}

void NucleoFisico::keyReleaseEvent(QKeyEvent *event) {
    if (inputManager && !partidaTerminada_) {
        inputManager->procesarLiberacion(event->key());
    }
}

void NucleoFisico::closeEvent(QCloseEvent *event) {
    if (motorJuego) {
        motorJuego->detener();
    }
    QMainWindow::closeEvent(event);
}
