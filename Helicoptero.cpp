#include "Helicoptero.h"
#include <algorithm>

Helicoptero::Helicoptero(QGraphicsItem *parent)
    : QObject(nullptr)
    , QGraphicsPixmapItem(parent)
    , velocidadVertical(0.0)
    , gravedad(500.0)
    , anguloInclinacion(0.0)
    , ascensoActivo(false)
    , velocidadHorizontal(250.0)
    , izquierdaActiva(false)
    , derechaActiva(false)
    , vida(VIDA_MAXIMA)
    , tiempoInvulnerable(0.0)
    , aterrizado(true)
{
    QPixmap sprite(":/assets/chopper1_1.png");
    setPixmap(sprite.scaled(110, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setTransformOriginPoint(boundingRect().center());
    // arranca en el suelo
    setPos(150, SUELO_Y);
}

Helicoptero::~Helicoptero() {}

void Helicoptero::setAscenso(bool activo) { ascensoActivo = activo; }
void Helicoptero::setMoverIzquierda(bool activo) { izquierdaActiva = activo; }
void Helicoptero::setMoverDerecha(bool activo) { derechaActiva = activo; }

void Helicoptero::actualizarFisica(qreal deltaTime) {
    if (tiempoInvulnerable > 0.0) {
        tiempoInvulnerable -= deltaTime;
        if (tiempoInvulnerable < 0.0) {
            tiempoInvulnerable = 0.0;
        }
    }

    // fisica vertical
    const qreal empuje = 900.0;
    qreal aceleracionNeta = gravedad;
    if (ascensoActivo) aceleracionNeta -= empuje;

    velocidadVertical += aceleracionNeta * deltaTime;
    setPos(x(), y() + velocidadVertical * deltaTime);

    if (y() < 0) { setY(0); velocidadVertical = 0; }
    resolverAterrizaje();

    // mov horizontal
    qreal desplazamientoX = 0.0;
    if (derechaActiva) desplazamientoX += velocidadHorizontal;
    if (izquierdaActiva) desplazamientoX -= velocidadHorizontal;
    setX(x() + desplazamientoX * deltaTime);

    const qreal anchoSprite = pixmap().width();
    if (x() < 0.0) {
        setX(0.0);
    } else if (x() > 900.0 - anchoSprite) {
        setX(900.0 - anchoSprite);
    }

    const qreal anguloMaximo = 15.0;
    const qreal velocidadRotacion = 90.0;

    qreal anguloObjetivo = 0.0;
    if (derechaActiva) anguloObjetivo = anguloMaximo;
    else if (izquierdaActiva) anguloObjetivo = -anguloMaximo;

    if (anguloInclinacion < anguloObjetivo) {
        anguloInclinacion = std::min(anguloInclinacion + velocidadRotacion * deltaTime, anguloObjetivo);
    } else if (anguloInclinacion > anguloObjetivo) {
        anguloInclinacion = std::max(anguloInclinacion - velocidadRotacion * deltaTime, anguloObjetivo);
    }

    setRotation(anguloInclinacion);
}

void Helicoptero::resolverAterrizaje() {
    if (y() < SUELO_Y) {
        aterrizado = false;
        return;
    }

    const bool acabaDeTocarSuelo = !aterrizado;
    const qreal impactoVertical = velocidadVertical;
    setY(SUELO_Y);

    if (velocidadVertical > 0.0) {
        velocidadVertical = 0.0;
    }
    aterrizado = true;

    if (!acabaDeTocarSuelo) {
        return;
    }

    if (impactoVertical > VELOCIDAD_ATERRIZAJE_SUAVE) {
        emit aterrizajeViolento();
        vida = 0;
        emit vidaCambiada(vida, VIDA_MAXIMA);
        emit destruido();
    } else {
        emit aterrizajeSuave();
    }
}

void Helicoptero::recibirDano(int cantidad) {
    if (cantidad <= 0 || vida <= 0 || tiempoInvulnerable > 0.0) {
        return;
    }

    vida -= cantidad;
    if (vida < 0) vida = 0;

    tiempoInvulnerable = 1.0; // 1segundo

    emit vidaCambiada(vida, VIDA_MAXIMA);

    if (vida == 0) {
        emit destruido();
    }
}

int Helicoptero::vidaActual() const { return vida; }

int Helicoptero::vidaMaxima() const { return VIDA_MAXIMA; }

bool Helicoptero::estaAterrizado() const { return aterrizado; }

qreal Helicoptero::velocidadVerticalActual() const { return velocidadVertical; }