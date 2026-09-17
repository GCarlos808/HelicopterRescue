#include "GestorEntidades.h"
#include "Entidad.h"
#include "Edificio.h"
#include "Civil.h"
#include "Drone.h"
#include <QRandomGenerator>

GestorEntidades::GestorEntidades(QGraphicsScene *escenaJuego, Helicoptero *helicopteroJugador, qreal ancho, qreal alto)
    : entidades(nullptr), cantidad(0), capacidad(4)
    , escena(escenaJuego), helicoptero(helicopteroJugador)
    , anchoEscena(ancho), altoEscena(alto)
    , tiempoDesdeUltimoSpawn(0.0), intervaloSpawn(2.0)
{
    entidades = new Entidad*[capacidad];
}

GestorEntidades::~GestorEntidades() {
    for (int i = 0; i < cantidad; ++i) {
        delete entidades[i];
    }
    delete[] entidades;
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
        nuevoArreglo[i] = entidades[i];
    }
    delete[] entidades;
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

void GestorEntidades::generarEdificio() {
    Edificio *nuevo = new Edificio();
    qreal alturaAleatoria = 100 + QRandomGenerator::global()->bounded(150);
    nuevo->setPixmap(nuevo->pixmap().scaled(80, alturaAleatoria, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    nuevo->setPos(anchoEscena, altoEscena - alturaAleatoria);
    escena->addItem(nuevo);
    agregar(nuevo);
}

void GestorEntidades::generarCivil() {
    Civil *nuevo = new Civil();
    nuevo->setPos(anchoEscena, altoEscena - nuevo->pixmap().height());
    escena->addItem(nuevo);
    agregar(nuevo);
}

void GestorEntidades::generarDrone() {
    Drone *nuevo = new Drone(helicoptero);
    qreal alturaAleatoria = QRandomGenerator::global()->bounded(int(altoEscena) - 100);
    nuevo->setPos(anchoEscena, alturaAleatoria);
    escena->addItem(nuevo);
    agregar(nuevo);
}

void GestorEntidades::intentarGenerar(qreal deltaTime) {
    tiempoDesdeUltimoSpawn += deltaTime;
    if (tiempoDesdeUltimoSpawn < intervaloSpawn) return;
    tiempoDesdeUltimoSpawn = 0.0;

    int eleccion = QRandomGenerator::global()->bounded(3); // 0, 1 o 2
    switch (eleccion) {
    case 0: generarEdificio(); break;
    case 1: generarCivil(); break;
    case 2: generarDrone(); break;
    }
}

Entidad *GestorEntidades::colisionCon(QGraphicsItem *objetivo) const {
    for (int i = 0; i < cantidad; ++i) {
        if (objetivo->collidesWithItem(entidades[i])) {
            return entidades[i];
        }
    }
    return nullptr;
}

void GestorEntidades::rescatar(Entidad *civil) {
    for (int i = 0; i < cantidad; ++i) {
        if (entidades[i] == civil) {
            escena->removeItem(entidades[i]);
            delete entidades[i];
            for (int j = i; j < cantidad - 1; ++j) {
                entidades[j] = entidades[j + 1];
            }
            cantidad--;
            return;
        }
    }
}