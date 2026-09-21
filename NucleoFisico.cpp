#include "NucleoFisico.h"
#include "Entidad.h"
#include "PantallaResultado.h"
#include "ProgressManager.h"
#include "ScoreManager.h"

#include <QCloseEvent>
#include <QKeyEvent>
<<<<<<< HEAD
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

=======
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
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc
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

<<<<<<< HEAD
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
=======
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
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc

    connect(helicoptero, &Helicoptero::vidaCambiada, this, [this](int actual, int maximo) {
        qreal proporcion = qreal(actual) / maximo;
        barraVida->setRect(20, 20, 200 * proporcion, 20);
    });

    connect(helicoptero, &Helicoptero::destruido, this, [this]() {
<<<<<<< HEAD
        alPerder();
=======
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
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc
    });

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
<<<<<<< HEAD
        if (partidaTerminada_) {
=======
        if (nivelCompletado && nivelActual > 2) {
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc
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
<<<<<<< HEAD
        if (partidaTerminada_) {
            return;
        }

        tiempoPartida_ += deltaTime;
=======
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

>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc
        fondo->actualizar(deltaTime);

        if (misionIniciada && fondo->enZonaHangar()) {
            gestorEntidades->setGeneracionActiva(false);
        }

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

<<<<<<< HEAD
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
=======
        if (helicoptero->estaAterrizado()
            && helicoptero->vidaActual() > 0
            && !fondo->enZonaHangar()) {
            Entidad *civilCerca = gestorEntidades->civilCercano(helicoptero, 70.0);
            if (civilCerca) {
                gestorEntidades->rescatar(civilCerca);
                registrarRescate();
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc
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

<<<<<<< HEAD
        actualizarHud();
=======
        actualizarHudMision();
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc
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

<<<<<<< HEAD
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
=======
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
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc
        inputManager->procesarPresion(event->key());
    }
    event->accept();
}

void NucleoFisico::keyReleaseEvent(QKeyEvent *event) {
<<<<<<< HEAD
    if (inputManager && !partidaTerminada_) {
=======
    if (event->isAutoRepeat()) {
        event->accept();
        return;
    }
    if (inputManager) {
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc
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

        QMessageBox::information(
            this, "Nivel 1 completado",
            QString("Entrega en hangar exitosa.\n\n"
                    "Civiles rescatados: %1\n"
                    "Puntos: %2\n\n"
                    "Despega para comenzar el nivel 2.")
                .arg(rescatados)
                .arg(puntaje));

    } else if (nivelActual == 2) {
        nivelActual = 3;
        nivelCompletado = false;
        misionIniciada = false;
        gestorEntidades->establecerNivel(3);
        fondo->establecerNivel(3);
        helicoptero->setTurbulenciaActiva(true); //tormenta al entrar al nivel 3

        QMessageBox::information(
            this, "Nivel 2 completado",
            QString("Entrega en hangar exitosa.\n\n"
                    "Civiles rescatados: %1\n"
                    "Puntos: %2\n\n"
                    "Atención: Se reportan fuertes turbulencias. Despega para el nivel 3.")
                .arg(rescatados)
                .arg(puntaje));

    } else {
        //fin del juego
        nivelCompletado = true;
        gestorEntidades->setGeneracionActiva(false);
        helicoptero->setTurbulenciaActiva(false);

        if (motorJuego) {
            motorJuego->detener();
        }
        if (textoEstado) {
            textoEstado->setPlainText("Mision completada");
            textoEstado->setDefaultTextColor(QColor(120, 255, 160));
        }

        QMessageBox::information(
            this,
            "Misión Completada",
            QString("¡Has terminado toda la misión!\n\n"
                    "Civiles rescatados: %1\n"
                    "Puntos finales: %2")
                .arg(rescatados)
                .arg(puntaje));
    }

    actualizarHudMision();
}