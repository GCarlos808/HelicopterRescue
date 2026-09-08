#include "GestorEntidades.h"
#include "Entidad.h"
#include "Edificio.h"
#include <QRandomGenerator>

GestorEntidades::GestorEntidades(QGraphicsScene *escenaJuego, qreal ancho, qreal alto) : entidades(nullptr), cantidad(0), capacidad(4), escena(escenaJuego)
                                                            , anchoEscena(ancho), altoEscena(alto), tiempoDesdeUltimoSpawn(0.0), intervaloSpawn(2.0){
    entidades = new Entidad*[capacidad];
}

GestorEntidades::~GestorEntidades() {
    for (int i = 0; i < cantidad; ++i) {
        delete entidades[i]; // libera cada objeto individual
    }
    delete[] entidades; // libera el arreglo
}

void GestorEntidades::agregar(Entidad *nueva) {
    if (cantidad >= capacidad) {
        redimensionar();
    }
    entidades[cantidad] = nueva;
    cantidad++;
}

void GestorEntidades::redimensionar() {
    int nuevaCapacidad = capacidad * 2;
    Entidad **nuevoArreglo = new Entidad*[nuevaCapacidad];

    for (int i = 0; i < cantidad; ++i) {
        nuevoArreglo[i] = entidades[i]; // se copian los punteros, no los objetos
    }

    delete[] entidades; // solo libera el arreglo viejo
    entidades = nuevoArreglo;
    capacidad = nuevaCapacidad;
}

void GestorEntidades::liberarSalientes() {
    int escritura = 0;
    for (int lectura = 0; lectura < cantidad; ++lectura) {
        if (entidades[lectura]->haSalidoDePantalla(0.0)) {
            escena->removeItem(entidades[lectura]);
            delete entidades[lectura];
        } else {
            entidades[escritura] = entidades[lectura];
            escritura++;
        }
    }
    cantidad = escritura;
}

void GestorEntidades::actualizar(qreal deltaTime) {
    for (int i = 0; i < cantidad; ++i) {
        entidades[i]->actualizar(deltaTime);
    }
    liberarSalientes();
}

void GestorEntidades::intentarGenerar(qreal deltaTime) {
    tiempoDesdeUltimoSpawn += deltaTime;
    if (tiempoDesdeUltimoSpawn < intervaloSpawn) return;
    tiempoDesdeUltimoSpawn = 0.0;

    Edificio *nuevoEdificio = new Edificio();

    qreal alturaAleatoria = 100 + QRandomGenerator::global()->bounded(150); // 100 a 250 pixeles
    nuevoEdificio->setPixmap(
        nuevoEdificio->pixmap().scaled(80, alturaAleatoria, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
        );
    nuevoEdificio->setPos(anchoEscena, altoEscena - alturaAleatoria);

    escena->addItem(nuevoEdificio);
    agregar(nuevoEdificio);
}

bool GestorEntidades::hayColisionCon(QGraphicsItem *objetivo) const {
    for (int i = 0; i < cantidad; ++i) {
        if (objetivo->collidesWithItem(entidades[i])) {
            return true;
        }
    }
    return false;
}