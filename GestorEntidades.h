#ifndef GESTORENTIDADES_H
#define GESTORENTIDADES_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPointF>

class Entidad;
class Helicoptero;

enum class ModoGeneracion {
    Mixto,
    Evacuacion,
    CombateAereo,
    Extraccion
};

class GestorEntidades {
public:
    explicit GestorEntidades(QGraphicsScene *escenaJuego, Helicoptero *helicopteroJugador, qreal anchoEscena, qreal altoEscena);
    ~GestorEntidades();

    void actualizar(qreal deltaTime);
    void intentarGenerar(qreal deltaTime);
    void setGeneracionActiva(bool activa);
    bool generacionActiva() const;
    Entidad *colisionPeligro(QGraphicsItem *objetivo) const;
    void rescatar(Entidad *civil);
    Entidad *civilCercano(QGraphicsItem *objetivo, qreal radio) const;

    void eliminarEntidad(Entidad *entidad);
    void dispararMisil(QPointF origen);
    bool resolverImpactosMisiles(); // true si destruyo un drone
    void setIntervaloSpawn(qreal segundos);
    void setModoGeneracion(ModoGeneracion modo);
    void setEdificiosAltos(bool activos);

    void establecerNivel(int nivel);

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
    bool generarActivo;

    void generarSoldado();
    void procesarDisparosEnemigos();

    int nivelActual = 1;
    void generarHelicopteroEnemigo();
};

#endif // GESTORENTIDADES_H
