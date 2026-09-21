#ifndef NUCLEOFISICO_H
#define NUCLEOFISICO_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QString>
#include "Helicoptero.h"
#include "Inputs.h"
#include "MotorJuego.h"
#include "GestorEntidades.h"
#include "FondoScroll.h"
#include "GestorAudio.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class QCloseEvent;
class QKeyEvent;
class QShowEvent;
class QEvent;
class ScoreManager;
class ProgressManager;

class NucleoFisico : public QMainWindow {
    Q_OBJECT

public:
    NucleoFisico(const QString &nombrePiloto, int nivel, ScoreManager *gestorPuntajes, ProgressManager *progreso, QWidget *parent = nullptr);
    ~NucleoFisico() override;

    int puntajeActual() const;
    int civilesRescatados() const;

signals:
    void solicitarReiniciar(int nivel);
    void solicitarNivel(int nivel);
    void solicitarNiveles();
    void solicitarMenu();

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
    void terminarPartida(bool victoria);

    GestorEntidades *gestorEntidades;
    FondoScroll *fondo;
    QGraphicsScene *escena;
    QGraphicsView *vista;
    Helicoptero *helicoptero;
    InputManager *inputManager;
    MotorJuego *motorJuego;
    GestorAudio *gestorAudio;
    QGraphicsRectItem *fondoBarraVida;
    QGraphicsRectItem *barraVida;
    QGraphicsTextItem *textoPuntaje;
    QGraphicsTextItem *textoRescatados;
    QGraphicsTextItem *textoSector;
    QGraphicsTextItem *textoEstado;

    ScoreManager *gestorPuntajes_;
    ProgressManager *progreso_;
    QString nombrePiloto_;

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