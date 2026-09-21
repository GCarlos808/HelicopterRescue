#ifndef FONDOSCROLL_H
#define FONDOSCROLL_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class FondoScroll {
public:
    explicit FondoScroll(QGraphicsScene *escenaJuego, qreal anchoEscena, qreal altoEscena, int nivelInicial = 1);
    void establecerNivel(int nivel);
    int nivel() const;

    static const int NUM_SEGMENTOS = 11;

    FondoScroll(QGraphicsScene *escenaJuego, qreal anchoEscena, qreal altoEscena);
    ~FondoScroll();

    void actualizar(qreal deltaTime);
    void setScrollActivo(bool activo);
    bool scrollActivo() const;
    bool enZonaHangar() const;
    bool finDeRecorrido() const;
    int segmentoActual() const; // 1..11
    qreal progreso() const; // segun terreno

private:
    void recargarTexturas();
    void cargarSegmentos();
    void aplicarDesplazamientos();
    QPixmap cargarPixmap(const QString &ruta, int ancho, int alto) const;

    QGraphicsScene *escena;
    QGraphicsPixmapItem *cielos[NUM_SEGMENTOS];
    QGraphicsPixmapItem *terrenos[NUM_SEGMENTOS];
    QGraphicsRectItem *zonaHangar;
    QGraphicsTextItem *textoHangar;

    qreal anchoEscena;
    qreal altoEscena;
    qreal anchoSegmento;
    qreal desplazamientoCielo;
    qreal desplazamientoTerreno;
    qreal desplazamientoMaximo;
    qreal velocidadCielo;
    qreal velocidadTerreno;
    bool activo;

    int nivelActual;
};

#endif // FONDOSCROLL_H