#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include "ProgressManager.h"
#include "ScoreManager.h"
#include <QMainWindow>
#include <QPointer>
#include <QString>

class NucleoFisico;
class VentanaPuntajes;
class PantallaNiveles;
class QLineEdit;
class QPushButton;

class MenuPrincipal : public QMainWindow {
    Q_OBJECT

public:
    explicit MenuPrincipal(QWidget* parent = nullptr);
    ~MenuPrincipal() override;

private slots:
    void alJugar();
    void alVerNiveles();
    void alVerPuntajes();
    void alVerInstrucciones();
    void alSalir();
    void alCerrarJuego();
    void alIniciarNivel(int nivel);
    void alProgramarReinicio(int nivel);
    void alProgramarNivel(int nivel);
    void alProgramarNiveles();
    void alProgramarMenu();

private:
    enum class AccionPostCierre {
        MostrarMenu,
        AbrirNiveles,
        AbrirNivel
    };

    ScoreManager gestorPuntajes_;
    ProgressManager progreso_;
    QLineEdit* entradaNombre_;
    QPointer<NucleoFisico> ventanaJuego_;
    QPointer<VentanaPuntajes> ventanaPuntajes_;
    QPointer<PantallaNiveles> ventanaNiveles_;
    AccionPostCierre accionPostCierre_;
    int nivelPendiente_;

    void crearInterfaz();
    QPushButton* crearBotonMenu(const QString& texto);
    QString nombrePilotoActual() const;
    void conectarSenalesJuego(NucleoFisico* juego);
};

#endif // MENUPRINCIPAL_H
