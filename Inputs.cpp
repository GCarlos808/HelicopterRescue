#include "Inputs.h"
#include <Qt>

InputManager::InputManager(QObject *parent) : QObject(parent) {
    // Configuracion de controles
    mapeoTeclas.insert(Qt::Key_Up, Accion::Ascender);
    mapeoTeclas.insert(Qt::Key_Left, Accion::Izquierda);
    mapeoTeclas.insert(Qt::Key_Right, Accion::Derecha);

    // Todas las acciones inician inactivas.
    estadoAcciones.insert(Accion::Ascender, false);
    estadoAcciones.insert(Accion::Izquierda, false);
    estadoAcciones.insert(Accion::Derecha, false);
}

void InputManager::procesarPresion(int codigoTecla) {
    if (!mapeoTeclas.contains(codigoTecla)) return; // tecla sin asignar

    Accion accion = mapeoTeclas.value(codigoTecla);

    if (estadoAcciones.value(accion) == true) return;

    estadoAcciones[accion] = true;
    emit accionCambiada(accion, true);
}

void InputManager::procesarLiberacion(int codigoTecla) {
    if (!mapeoTeclas.contains(codigoTecla)) return;

    Accion accion = mapeoTeclas.value(codigoTecla);
    estadoAcciones[accion] = false;
    emit accionCambiada(accion, false);
}

bool InputManager::accionActiva(Accion accion) const {
    return estadoAcciones.value(accion, false);
}