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
<<<<<<< HEAD
class QSoundEffect;
class ScoreManager;
class ProgressManager;
=======
class QShowEvent;
class QEvent;
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc

class NucleoFisico : public QMainWindow {
    Q_OBJECT

public:
    NucleoFisico(const QString& nombrePiloto,
                 int nivel,
                 ScoreManager* gestorPuntajes,
                 ProgressManager* progreso,
                 QWidget *parent = nullptr);
    ~NucleoFisico() override;

<<<<<<< HEAD
signals:
    void solicitarReiniciar(int nivel);
    void solicitarNivel(int nivel);
    void solicitarNiveles();
    void solicitarMenu();
=======
    int puntajeActual() const;
    int civilesRescatados() const;
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc

protected:
    bool event(QEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
<<<<<<< HEAD
    void configurarNivel(int nivel);
    void configurarFaseHistoria(int fase);
    void revisarObjetivosHistoria();
    void actualizarHud();
    void registrarPuntaje();
    void alPerder();
    void alCompletarNivel();
    void mostrarPantallaResultado(int tipoCodigo); // 0 derrota, 1 siguiente, 2 victoria
=======
    bool esTeclaDeJuego(int codigoTecla) const;
    void intentarDisparar();
    void actualizarHudRescate();
    void actualizarHudMision();
    void registrarRescate();
    void iniciarMision();
    void completarNivel();
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc

    GestorEntidades *gestorEntidades;
    FondoScroll *fondo;
    QGraphicsScene *escena;
    QGraphicsView *vista;
    Helicoptero *helicoptero;
    InputManager *inputManager;
    MotorJuego *motorJuego;
    QGraphicsRectItem *fondoBarraVida;
    QGraphicsRectItem *barraVida;
<<<<<<< HEAD
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
=======
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
>>>>>>> 09785195295aaf0a38e5503e88c25b8dd4d5e2bc
};

#endif // NUCLEOFISICO_H
