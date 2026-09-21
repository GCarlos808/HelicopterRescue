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
#include <QGraphicsTextItem>

class QCloseEvent;
class QKeyEvent;
class QShowEvent;
class QEvent;

class NucleoFisico : public QMainWindow {
    Q_OBJECT

public:
    explicit NucleoFisico(QWidget *parent = nullptr);
    ~NucleoFisico() override;

    int puntajeActual() const;
    int civilesRescatados() const;

protected:
    bool event(QEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    bool esTeclaDeJuego(int codigoTecla) const;
    void intentarDisparar();
    void actualizarHudRescate();
    void actualizarHudMision();
    void registrarRescate();
    void iniciarMision();
    void completarNivel();

    GestorEntidades *gestorEntidades;
    FondoScroll *fondo;
    QGraphicsScene *escena;
    QGraphicsView *vista;
    Helicoptero *helicoptero;
    InputManager *inputManager;
    MotorJuego *motorJuego;
    QGraphicsRectItem *fondoBarraVida;
    QGraphicsRectItem *barraVida;
    QGraphicsTextItem *textoPuntaje;
    QGraphicsTextItem *textoRescatados;
    QGraphicsTextItem *textoSector;
    QGraphicsTextItem *textoEstado;

    int puntaje;
    int rescatados;
    qreal cooldownDisparo;
    bool misionIniciada;
    bool nivelCompletado;
    int nivelActual;
    static const int PUNTOS_POR_RESCATE = 100;
    static const int BONUS_NIVEL = 500;
    static constexpr qreal COOLDOWN_DISPARO_SEGUNDOS = 3.0;
};

#endif // NUCLEOFISICO_H