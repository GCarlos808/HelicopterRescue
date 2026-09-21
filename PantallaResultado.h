#ifndef PANTALLARESULTADO_H
#define PANTALLARESULTADO_H

#include <QDialog>
#include <QString>

class QLabel;
class QPushButton;

// Pantallas de fin de partida: derrota, siguiente nivel y victoria final.
class PantallaResultado : public QDialog {
    Q_OBJECT

public:
    enum class Tipo {
        Derrota,
        SiguienteNivel,
        VictoriaFinal
    };

    PantallaResultado(Tipo tipo,
                      const QString& nombrePiloto,
                      int nivel,
                      int puntaje,
                      int rescatados,
                      double tiempoSegundos,
                      QWidget* parent = nullptr);

signals:
    void reiniciarSolicitado();
    void continuarSolicitado();
    void nivelesSolicitados();
    void menuSolicitado();

private:
    void crearInterfaz(Tipo tipo,
                       const QString& nombrePiloto,
                       int nivel,
                       int puntaje,
                       int rescatados,
                       double tiempoSegundos);
    QPushButton* crearBoton(const QString& texto, const QString& objectName = QString());
};

#endif // PANTALLARESULTADO_H
