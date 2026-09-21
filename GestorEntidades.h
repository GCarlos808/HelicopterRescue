#ifndef GESTORENTIDADES_H
#define GESTORENTIDADES_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPointF>

class Entidad;
class Helicoptero;

enum class ModoGeneracion {
    Mixto,
    Evacuacion,    // edificios + civiles (historia fase 1)
    CombateAereo,  // drones + edificios (historia fase 2)
    Extraccion     // mezcla agresiva (historia fase 3)
};

class GestorEntidades {
public:
    explicit GestorEntidades(QGraphicsScene *escenaJuego, Helicoptero *helicopteroJugador, qreal anchoEscena, qreal altoEscena);
    ~GestorEntidades();

    void actualizar(qreal deltaTime);
    void intentarGenerar(qreal deltaTime);
    Entidad *colisionPeligro(QGraphicsItem *objetivo) const;
    void rescatar(Entidad *civil);
    Entidad *civilCercano(QGraphicsItem *objetivo, qreal radio) const;

    void eliminarEntidad(Entidad *entidad);
    void dispararMisil(QPointF origen);
    bool resolverImpactosMisiles(); // true si destruyo un drone
    void setIntervaloSpawn(qreal segundos);
    void setModoGeneracion(ModoGeneracion modo);
    void setEdificiosAltos(bool activos);

private:
    void agregar(Entidad *nueva);
    void redimensionar();
    void liberarSalientes();
    void generarEdificio();
    void generarCivil();
    void generarDrone();
    void quitarDeArreglo(int indice);

    Entidad **entidades;
    int cantidad;
    int capacidad;

    QGraphicsScene *escena;
    Helicoptero *helicoptero;
    qreal anchoEscena;
    qreal altoEscena;
    qreal tiempoDesdeUltimoSpawn;
    qreal intervaloSpawn;
    ModoGeneracion modoGeneracion;
    bool edificiosAltos;
};

#endif // GESTORENTIDADES_H
