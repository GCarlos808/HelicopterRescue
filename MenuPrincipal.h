#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include "ScoreManager.h"
#include <QMainWindow>
#include <QPointer>

class NucleoFisico;
class VentanaPuntajes;
class QLineEdit;
class QPushButton;

class MenuPrincipal : public QMainWindow {
    Q_OBJECT

public:
    explicit MenuPrincipal(QWidget* parent = nullptr);
    ~MenuPrincipal() override;

private slots:
    void alJugar();
    void alVerPuntajes();
    void alVerInstrucciones();
    void alSalir();
    void alCerrarJuego();

private:
    ScoreManager gestorPuntajes_;
    QLineEdit* entradaNombre_;
    QPointer<NucleoFisico> ventanaJuego_;
    QPointer<VentanaPuntajes> ventanaPuntajes_;

    void crearInterfaz();
    QPushButton* crearBotonMenu(const QString& texto);
};

#endif // MENUPRINCIPAL_H
