#ifndef VENTANAPUNTAJES_H
#define VENTANAPUNTAJES_H

#include "ScoreManager.h"
#include <QMainWindow>

class QTabWidget;
class QTableWidget;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QLabel;

class VentanaPuntajes : public QMainWindow {
    Q_OBJECT

public:
    explicit VentanaPuntajes(ScoreManager* gestor, QWidget* parent = nullptr);
    void recargar();

private slots:
    void alAgregarPuntaje();
    void alGuardarPartida();
    void alCargarPartida();
    void alEliminarPartida();
    void alSeleccionarPartida(int fila, int columna);
    void alRecargarDatos();
    void alVolver();

private:
    ScoreManager* gestor_;

    QTabWidget* pestanas_;
    QTableWidget* tablaPuntajes_;
    QLineEdit* entradaNombrePuntaje_;
    QSpinBox* entradaPuntaje_;
    QDoubleSpinBox* entradaTiempo_;
    QSpinBox* entradaNivelPuntaje_;

    QTableWidget* tablaPartidas_;
    QLineEdit* entradaNombrePartida_;
    QSpinBox* entradaNivelPartida_;
    QSpinBox* entradaPuntajePartida_;
    QSpinBox* entradaSalud_;
    QSpinBox* entradaCombustible_;
    QSpinBox* entradaRescatados_;
    QDoubleSpinBox* entradaTiempoPartida_;
    QDoubleSpinBox* entradaPosX_;
    QDoubleSpinBox* entradaPosY_;
    QLabel* etiquetaDetallePartida_;

    void crearInterfaz();
    QWidget* crearPestanaPuntajes();
    QWidget* crearPestanaPartidas();
    void actualizarTablaPuntajes();
    void actualizarTablaPartidas();
    void limpiarFormularioPuntaje();
    void llenarFormularioPartida(const SavedGame& partida);
    void mostrarMensaje(const QString& titulo, const QString& mensaje, bool esError = false);
};

#endif // VENTANAPUNTAJES_H
