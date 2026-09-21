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
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class QCloseEvent;
class QKeyEvent;
class QSoundEffect;
class ScoreManager;
class ProgressManager;

class NucleoFisico : public QMainWindow {
    Q_OBJECT

public:
    NucleoFisico(const QString& nombrePiloto,
                 int nivel,
                 ScoreManager* gestorPuntajes,
                 ProgressManager* progreso,
                 QWidget *parent = nullptr);
    ~NucleoFisico() override;

signals:
    void solicitarReiniciar(int nivel);
    void solicitarNivel(int nivel);
    void solicitarNiveles();
    void solicitarMenu();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void configurarNivel(int nivel);
    void configurarFaseHistoria(int fase);
    void revisarObjetivosHistoria();
    void actualizarHud();
    void registrarPuntaje();
    void alPerder();
    void alCompletarNivel();
    void mostrarPantallaResultado(int tipoCodigo); // 0 derrota, 1 siguiente, 2 victoria

    GestorEntidades *gestorEntidades;
    FondoScroll *fondo;
    QGraphicsScene *escena;
    QGraphicsView *vista;
    Helicoptero *helicoptero;
    InputManager *inputManager;
    MotorJuego *motorJuego;
    QGraphicsRectItem *fondoBarraVida;
    QGraphicsRectItem *barraVida;
    QGraphicsTextItem *textoHud;
    QGraphicsTextItem *textoHistoria;
    QSoundEffect *sonidoChoqueEdificio;

    ScoreManager *gestorPuntajes_;
    ProgressManager *progreso_;
    QString nombrePiloto_;
    int nivelActual_;
    int metaRescates_;
    int metaDrones_;
    int rescatados_;
    int dronesDestruidos_;
    int rescatadosFase_;
    int dronesFase_;
    int faseHistoria_;
    int puntaje_;
    qreal tiempoPartida_;
    bool partidaTerminada_;
    bool modoHistoria_;
    QString objetivoActual_;
};

#endif // NUCLEOFISICO_H
