#ifndef INPUTS_H
#define INPUTS_H

#include <QObject>
#include <QMap>

class InputManager : public QObject {
    Q_OBJECT

public:
    enum class Accion {
        Ascender,
        Izquierda,
        Derecha,
        Disparar
    };

    explicit InputManager(QObject *parent = nullptr);

    void procesarPresion(int codigoTecla);
    void procesarLiberacion(int codigoTecla);

    bool accionActiva(Accion accion) const;

signals:
    void accionCambiada(Accion accion, bool activa);

private:
    QMap<int, Accion> mapeoTeclas;
    QMap<Accion, bool> estadoAcciones;
};

#endif // INPUTS_H