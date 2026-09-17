#ifndef GESTORENTIDADES_H
#define GESTORENTIDADES_H

#include <QGraphicsScene>

class Entidad;
class Helicoptero;

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
    void resolverImpactosMisiles();

private:
    void agregar(Entidad *nueva);
    void redimensionar();
    void liberarSalientes();
    void generarEdificio();
    void generarCivil();
    void generarDrone();

    Entidad **entidades;
    int cantidad;
    int capacidad;

    QGraphicsScene *escena;
    Helicoptero *helicoptero;
    qreal anchoEscena;
    qreal altoEscena;
    qreal tiempoDesdeUltimoSpawn;
    qreal intervaloSpawn;
};

#endif // GESTORENTIDADES_H