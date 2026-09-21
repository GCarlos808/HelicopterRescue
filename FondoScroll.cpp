#include "FondoScroll.h"
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QtGlobal>

FondoScroll::FondoScroll(QGraphicsScene *escenaJuego, qreal ancho, qreal alto, int nivelInicial)
    : escena(escenaJuego)
    , zonaHangar(nullptr)
    , textoHangar(nullptr)
    , anchoEscena(ancho)
    , altoEscena(alto)
    , anchoSegmento(ancho)
    , desplazamientoCielo(0.0)
    , desplazamientoTerreno(0.0)
    , desplazamientoMaximo(0.0)
    , velocidadCielo(40.0)
    , velocidadTerreno(200.0)
    , activo(false)
    , nivelActual(nivelInicial > 0 ? nivelInicial : 1)
{
    for (int i = 0; i < NUM_SEGMENTOS; ++i) {
        cielos[i] = nullptr;
        terrenos[i] = nullptr;
    }

    desplazamientoMaximo = (NUM_SEGMENTOS - 1) * anchoSegmento;
    cargarSegmentos();
    aplicarDesplazamientos();
}

FondoScroll::~FondoScroll() {
}

QPixmap FondoScroll::cargarPixmap(const QString &ruta, int ancho, int alto) const {
    QPixmap pix(ruta);
    if (pix.isNull()) {
        pix = QPixmap(ancho, alto);
        pix.fill(Qt::darkGray);
    }
    return pix.scaled(ancho, alto, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void FondoScroll::cargarSegmentos() {
    const int altoTerreno = int(altoEscena * 0.3);
    const qreal yTerreno = altoEscena - altoTerreno;
    const qreal margenHangar = anchoSegmento * 0.25;
    const qreal anchoZona = anchoSegmento * 0.50;
    const qreal altoZona = 18.0;

    for (int i = 0; i < NUM_SEGMENTOS; ++i) {
        const int numero = i + 1;
        QPixmap cieloPix = cargarPixmap(
            QString(":/assets/sky%1_%2.png").arg(nivelActual).arg(numero),
            int(anchoSegmento), int(altoEscena));
        QPixmap terrenoPix = cargarPixmap(
            QString(":/assets/landscape%1_%2.png").arg(nivelActual).arg(numero),
            int(anchoSegmento), altoTerreno);

        cielos[i] = escena->addPixmap(cieloPix);
        cielos[i]->setZValue(-2);

        terrenos[i] = escena->addPixmap(terrenoPix);
        terrenos[i]->setZValue(-1);
        terrenos[i]->setY(yTerreno);
    }

    zonaHangar = escena->addRect(
        0, 0, anchoZona, altoZona,
        QPen(QColor(80, 220, 120), 2),
        QBrush(QColor(60, 180, 100, 90)));
    zonaHangar->setZValue(0);
    zonaHangar->setY(altoEscena - altoTerreno - altoZona - 4.0);
    zonaHangar->setData(0, margenHangar);

    textoHangar = escena->addText(
        QStringLiteral("HANGAR - ATERRIZA"),
        QFont(QStringLiteral("Segoe UI"), 12, QFont::Bold));
    textoHangar->setDefaultTextColor(QColor(210, 255, 220));
    textoHangar->setZValue(1);
    textoHangar->setY(zonaHangar->y() - 24.0);
}

void FondoScroll::aplicarDesplazamientos() {
    for (int i = 0; i < NUM_SEGMENTOS; ++i) {
        const qreal baseX = i * anchoSegmento;
        if (cielos[i]) {
            cielos[i]->setX(baseX - desplazamientoCielo);
        }
        if (terrenos[i]) {
            terrenos[i]->setX(baseX - desplazamientoTerreno);
        }
    }

    if (zonaHangar) {
        const qreal margenHangar = zonaHangar->data(0).toReal();
        const qreal xHangar = (NUM_SEGMENTOS - 1) * anchoSegmento
                              - desplazamientoTerreno + margenHangar;
        zonaHangar->setX(xHangar);
        if (textoHangar) {
            textoHangar->setX(xHangar);
        }
    }
}

void FondoScroll::setScrollActivo(bool valor) {
    activo = valor;
}

bool FondoScroll::scrollActivo() const {
    return activo;
}

bool FondoScroll::finDeRecorrido() const {
    return desplazamientoTerreno >= desplazamientoMaximo - 0.5;
}

bool FondoScroll::enZonaHangar() const {
    return desplazamientoTerreno >= desplazamientoMaximo - anchoSegmento * 0.55;
}

int FondoScroll::segmentoActual() const {
    if (anchoSegmento <= 0.0) {
        return 1;
    }
    const int indice = int(desplazamientoTerreno / anchoSegmento);
    if (indice < 0) {
        return 1;
    }
    if (indice >= NUM_SEGMENTOS - 1 && finDeRecorrido()) {
        return NUM_SEGMENTOS;
    }
    if (indice >= NUM_SEGMENTOS) {
        return NUM_SEGMENTOS;
    }
    return indice + 1;
}

qreal FondoScroll::progreso() const {
    if (desplazamientoMaximo <= 0.0) {
        return 0.0;
    }
    return qBound(0.0, desplazamientoTerreno / desplazamientoMaximo, 1.0);
}

void FondoScroll::actualizar(qreal deltaTime) {
    if (!activo || finDeRecorrido()) {
        if (finDeRecorrido()) {
            desplazamientoTerreno = desplazamientoMaximo;
            const qreal maxCielo = desplazamientoMaximo * (velocidadCielo / velocidadTerreno);
            if (desplazamientoCielo > maxCielo) {
                desplazamientoCielo = maxCielo;
            }
            aplicarDesplazamientos();
        }
        return;
    }

    desplazamientoTerreno += velocidadTerreno * deltaTime;
    desplazamientoCielo += velocidadCielo * deltaTime;

    if (desplazamientoTerreno > desplazamientoMaximo) {
        desplazamientoTerreno = desplazamientoMaximo;
    }

    const qreal maxCielo = desplazamientoMaximo * (velocidadCielo / velocidadTerreno);
    if (desplazamientoCielo > maxCielo) {
        desplazamientoCielo = maxCielo;
    }

    aplicarDesplazamientos();
}

void FondoScroll::recargarTexturas() {
    const int altoTerreno = int(altoEscena * 0.3);
    for (int i = 0; i < NUM_SEGMENTOS; ++i) {
        const int numero = i + 1;
        QPixmap cieloPix = cargarPixmap(
            QString(":/assets/sky%1_%2.png").arg(nivelActual).arg(numero),
            int(anchoSegmento), int(altoEscena));
        QPixmap terrenoPix = cargarPixmap(
            QString(":/assets/landscape%1_%2.png").arg(nivelActual).arg(numero),
            int(anchoSegmento), altoTerreno);
        if (cielos[i]) {
            cielos[i]->setPixmap(cieloPix);
        }
        if (terrenos[i]) {
            terrenos[i]->setPixmap(terrenoPix);
        }
    }
}

void FondoScroll::establecerNivel(int nivel) {
    nivelActual = nivel > 0 ? nivel : 1;
    recargarTexturas();
    desplazamientoCielo = 0.0;
    desplazamientoTerreno = 0.0;
    activo = false;
    aplicarDesplazamientos();
}

int FondoScroll::nivel() const {
    return nivelActual;
}