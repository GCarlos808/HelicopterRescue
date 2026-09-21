#ifndef PANTALLANIVELES_H
#define PANTALLANIVELES_H

#include <QDialog>

class ProgressManager;
class QPushButton;

// Pantalla para elegir entre niveles desbloqueados.
class PantallaNiveles : public QDialog {
    Q_OBJECT

public:
    explicit PantallaNiveles(ProgressManager* progreso, QWidget* parent = nullptr);

signals:
    void nivelElegido(int nivel);
    void volverMenu();

private:
    static const int CANTIDAD_NIVELES = 4;

    ProgressManager* progreso_;
    QPushButton* botonesNivel_[CANTIDAD_NIVELES];

    void crearInterfaz();
    void actualizarEstadoBotones();
};

#endif // PANTALLANIVELES_H
