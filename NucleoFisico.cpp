#include "NucleoFisico.h"
#include "Entidad.h"
#include "ScoreManager.h"
#include "ProgressManager.h"
#include "ScoreEntry.h"
#include <QCloseEvent>
#include <QKeyEvent>
#include <QShowEvent>
#include <QFont>

NucleoFisico::NucleoFisico(const QString &nombrePiloto,int nivel, ScoreManager *gestorPuntajes, ProgressManager *progreso, QWidget *parent)
    : QMainWindow(parent)
    , gestorPuntajes_(gestorPuntajes)
    , progreso_(progreso)
    , nombrePiloto_(nombrePiloto)
    , puntaje(0)
    , rescatados(0)
    , cooldownDisparo(0.0)
    , misionIniciada(false)
    , nivelCompletado(false)
    , nivelActual(nivel)
{
    setWindowTitle(QString("Helicopter Rescue — %1 (Nivel %2)").arg(nombrePiloto_).arg(nivelActual));

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 900, 580);

    fondo = new FondoScroll(escena, 900, 580, nivelActual);

    helicoptero = new Helicoptero();
    escena->addItem(helicoptero);

    gestorAudio = new GestorAudio(this);
    gestorAudio->iniciarMusicaFondo();
    gestorAudio->iniciarRotorJugador();

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

    connect(helicoptero, &Helicoptero::danioRecibido, this, [this]() {
        gestorAudio->reproducirDano();
    });

    connect(helicoptero, &Helicoptero::destruido, this, [this]() {
        gestorAudio->detenerRotorJugador();
        gestorAudio->detenerMusicaFondo();
        if (motorJuego) motorJuego->detener();
        if (fondo) fondo->setScrollActivo(false);
        if (gestorEntidades) gestorEntidades->setGeneracionActiva(false);
        if (textoEstado) {
            textoEstado->setPlainText("Mision fallida");
            textoEstado->setDefaultTextColor(QColor(255, 120, 120));
        }
        terminarPartida(false); // NUEVO
    });

    connect(helicoptero, &Helicoptero::aterrizajeSuave, this, [this]() {
        if (nivelCompletado || !misionIniciada || !fondo) return;
        if (fondo->enZonaHangar()) completarNivel();
    });

    gestorEntidades = new GestorEntidades(escena, helicoptero, 900, 580);
    gestorEntidades->establecerNivel(nivelActual);
    inputManager = new InputManager(this);

    vista = new QGraphicsView(escena, this);
    vista->setFixedSize(900, 580);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(vista);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    if (nivelActual >= 3) {
        helicoptero->setTurbulenciaActiva(true);
    }

    motorJuego = new MotorJuego(this);

    connect(inputManager, &InputManager::accionCambiada, this, [this](InputManager::Accion accion, bool activa) {
        if (nivelCompletado && nivelActual > 2) return;
        if (accion == InputManager::Accion::Ascender) {
            helicoptero->setAscenso(activa);
            if (activa) iniciarMision();
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
        if (!fondo || !gestorEntidades || !helicoptero) return;

        if (cooldownDisparo > 0.0) {
            cooldownDisparo -= deltaTime;
            if (cooldownDisparo < 0.0) cooldownDisparo = 0.0;
        }

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

        gestorAudio->setRotorEnemigoActivo(gestorEntidades->hayEnemigosActivos());

        if (gestorEntidades->resolverImpactosMisiles()) {
            gestorAudio->reproducirExplosion();
        }

        if (helicoptero->estaAterrizado() && helicoptero->vidaActual() > 0 && !fondo->enZonaHangar()) {
            Entidad *civilCerca = gestorEntidades->civilCercano(helicoptero, 70.0);
            if (civilCerca) {
                gestorEntidades->rescatar(civilCerca);
                registrarRescate();
                gestorAudio->reproducirRescate();
            }
        }

        Entidad *peligro = gestorEntidades->colisionPeligro(helicoptero);
        if (peligro) {
            helicoptero->recibirDano(1);
            gestorAudio->reproducirImpacto();
            if (peligro->tipo() == TipoEntidad::Enemigo) {
                gestorEntidades->eliminarEntidad(peligro);
            }
        }

        actualizarHudMision();
    });

    motorJuego->iniciar();
}
void NucleoFisico::terminarPartida(bool victoria) {
    if (gestorPuntajes_) {
        ScoreEntry entrada; // AJUSTAR nombres de campo según tu ScoreEntry.h real
        entrada.nombre = nombrePiloto_.toStdString();
        entrada.puntaje = puntaje;
        entrada.nivel = nivelActual;
        gestorPuntajes_->agregarPuntaje(entrada);
        gestorPuntajes_->guardarPuntajes();
    }

    if (victoria && progreso_) {
        progreso_->desbloquearNivel(nivelActual + 1);
        progreso_->guardar();
    }

    emit solicitarMenu();
}