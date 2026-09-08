#ifndef FONDOSCROLL_H
#define FONDOSCROLL_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class FondoScroll {
public:
    FondoScroll(QGraphicsScene *escenaJuego, qreal anchoEscena, qreal altoEscena);
    void actualizar(qreal deltaTime);

private:
    void desplazarPar(QGraphicsPixmapItem *tileA, QGraphicsPixmapItem *tileB, qreal velocidad, qreal deltaTime);

    QGraphicsPixmapItem *cieloA, *cieloB;
    QGraphicsPixmapItem *terrenoA, *terrenoB;

    qreal velocidadCielo;
    qreal velocidadTerreno;
};

#endif // FONDOSCROLL_H