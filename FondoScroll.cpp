#include "FondoScroll.h"

FondoScroll::FondoScroll(QGraphicsScene *escenaJuego, qreal anchoEscena, qreal altoEscena): velocidadCielo(40.0), velocidadTerreno(200.0){
    QPixmap cieloPix(":/assets/sky1_1.png");
    cieloPix = cieloPix.scaled(anchoEscena, altoEscena, Qt::IgnoreAspectRatio);

    cieloA = escenaJuego->addPixmap(cieloPix);
    cieloB = escenaJuego->addPixmap(cieloPix);
    cieloA->setZValue(-2);
    cieloB->setZValue(-2);
    cieloA->setPos(0, 0);
    cieloB->setPos(anchoEscena, 0);

    QPixmap terrenoPix(":/assets/landscape1_1.png");
    terrenoPix = terrenoPix.scaled(anchoEscena, altoEscena * 0.3, Qt::IgnoreAspectRatio);

    terrenoA = escenaJuego->addPixmap(terrenoPix);
    terrenoB = escenaJuego->addPixmap(terrenoPix);
    terrenoA->setZValue(-1);
    terrenoB->setZValue(-1);
    terrenoA->setPos(0, altoEscena - terrenoPix.height());
    terrenoB->setPos(anchoEscena, altoEscena - terrenoPix.height());
}

void FondoScroll::desplazarPar(QGraphicsPixmapItem *tileA, QGraphicsPixmapItem *tileB, qreal velocidad, qreal deltaTime) {
    qreal ancho = tileA->pixmap().width();

    tileA->setX(tileA->x() - velocidad * deltaTime);
    tileB->setX(tileB->x() - velocidad * deltaTime);

    if (tileA->x() <= -ancho) tileA->setX(tileB->x() + ancho);
    if (tileB->x() <= -ancho) tileB->setX(tileA->x() + ancho);
}

void FondoScroll::actualizar(qreal deltaTime) {
    desplazarPar(cieloA, cieloB, velocidadCielo, deltaTime);
    desplazarPar(terrenoA, terrenoB, velocidadTerreno, deltaTime);
}