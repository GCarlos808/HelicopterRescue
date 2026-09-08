#ifndef GESTORENTIDADES_H
#define GESTORENTIDADES_H

#include <QGraphicsScene>

class Entidad;

class GestorEntidades {
public:
    explicit GestorEntidades(QGraphicsScene *escenaJuego, qreal anchoEscena, qreal altoEscena);
    ~GestorEntidades();

    void actualizar(qreal deltaTime);
    void intentarGenerar(qreal deltaTime);
    bool hayColisionCon(QGraphicsItem *objetivo) const;

private:
    void agregar(Entidad *nueva);
    void redimensionar();
    void liberarSalientes();

    Entidad **entidades;
    int cantidad;
    int capacidad;

    QGraphicsScene *escena;
    qreal anchoEscena;
    qreal altoEscena;
    qreal tiempoDesdeUltimoSpawn;
    qreal intervaloSpawn;
};

#endif // GESTORENTIDADES_H