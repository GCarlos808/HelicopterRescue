#ifndef NUCLEOFISICO_H
#define NUCLEOFISICO_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Helicoptero.h"
#include "Inputs.h"
#include "MotorJuego.h"
#include "GestorEntidades.h"
#include "FondoScroll.h"
#include <QGraphicsRectItem>

class QCloseEvent;
class QKeyEvent;

class NucleoFisico : public QMainWindow {
    Q_OBJECT

public:
    explicit NucleoFisico(QWidget *parent = nullptr);
    ~NucleoFisico() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    GestorEntidades *gestorEntidades;
    FondoScroll *fondo;
    QGraphicsScene *escena;
    QGraphicsView *vista;
    Helicoptero *helicoptero;
    InputManager *inputManager;
    MotorJuego *motorJuego;
    QGraphicsRectItem *fondoBarraVida;
    QGraphicsRectItem *barraVida;
};

#endif // NUCLEOFISICO_H