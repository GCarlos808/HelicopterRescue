#include "GestorEntidades.h"
#include "Entidad.h"
#include "Edificio.h"
#include "Civil.h"
#include "Drone.h"
#include "Misil.h"
#include <QLineF>
#include <QRandomGenerator>
#include <algorithm>

GestorEntidades::GestorEntidades(QGraphicsScene *escenaJuego, Helicoptero *helicopteroJugador, qreal ancho, qreal alto)
    : entidades(nullptr), cantidad(0), capacidad(4)
    , escena(escenaJuego), helicoptero(helicopteroJugador)
    , anchoEscena(ancho), altoEscena(alto)
    , tiempoDesdeUltimoSpawn(0.0), intervaloSpawn(2.0)
    , modoGeneracion(ModoGeneracion::Mixto)
    , edificiosAltos(false)
{
    entidades = new Entidad*[capacidad];
    for (int i = 0; i < capacidad; ++i) {
        entidades[i] = nullptr;
    }
}

GestorEntidades::~GestorEntidades() {
    for (int i = 0; i < cantidad; ++i) {
        if (!entidades[i]) {
            continue;
        }
        if (escena && entidades[i]->scene() == escena) {
            escena->removeItem(entidades[i]);
        }
        delete entidades[i];
        entidades[i] = nullptr;
    }
    delete[] entidades;
    entidades = nullptr;
    cantidad = 0;
}

void GestorEntidades::agregar(Entidad *nueva) {
    if (!nueva) {
        return;
    }
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
    for (int i = cantidad; i < nuevaCapacidad; ++i) {
        nuevoArreglo[i] = nullptr;
    }
    delete[] entidades;
    entidades = nuevoArreglo;
    capacidad = nuevaCapacidad;
}

void GestorEntidades::quitarDeArreglo(int indice) {
    if (indice < 0 || indice >= cantidad) {
        return;
    }
    for (int j = indice; j < cantidad - 1; ++j) {
        entidades[j] = entidades[j + 1];
    }
    entidades[cantidad - 1] = nullptr;
    cantidad--;
}

void GestorEntidades::liberarSalientes() {
    int escritura = 0;
    for (int lectura = 0; lectura < cantidad; ++lectura) {
        Entidad *entidad = entidades[lectura];
        if (!entidad) {
            continue;
        }
        if (entidad->haSalidoDePantalla(0.0)) {
            if (escena && entidad->scene() == escena) {
                escena->removeItem(entidad);
            }
            delete entidad;
        } else {
            entidades[escritura] = entidad;
            escritura++;
        }
    }
    for (int i = escritura; i < cantidad; ++i) {
        entidades[i] = nullptr;
    }
    cantidad = escritura;
}

void GestorEntidades::actualizar(qreal deltaTime) {
    for (int i = 0; i < cantidad; ++i) {
        if (entidades[i]) {
            entidades[i]->actualizar(deltaTime);
        }
    }
    liberarSalientes();
}

void GestorEntidades::generarEdificio() {
    if (!escena) {
        return;
    }

    Edificio *nuevo = new Edificio();
    const int alturaBase = edificiosAltos ? 160 : 100;
    const int rangoAltura = edificiosAltos ? 180 : 150;
    qreal alturaAleatoria = alturaBase + QRandomGenerator::global()->bounded(rangoAltura);
    const int anchoEdificio = edificiosAltos ? 95 : 80;
    nuevo->setPixmap(nuevo->pixmap().scaled(anchoEdificio, int(alturaAleatoria), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    nuevo->setPos(anchoEscena, altoEscena - alturaAleatoria);
    escena->addItem(nuevo);
    agregar(nuevo);
}

void GestorEntidades::generarCivil() {
    if (!escena) {
        return;
    }

    Civil *nuevo = new Civil();

    qreal margenSuperior = 100.0;
    qreal margenInferior = 20.0;
    qreal altoSprite = std::max<qreal>(1.0, nuevo->pixmap().height());
    qreal rangoDisponible = altoEscena - altoSprite - margenSuperior - margenInferior;

    qreal posY = margenSuperior;
    if (rangoDisponible > 1.0) {
        posY = margenSuperior + QRandomGenerator::global()->bounded(int(rangoDisponible));
    }

    nuevo->setPos(anchoEscena, posY);
    escena->addItem(nuevo);
    agregar(nuevo);
}

void GestorEntidades::generarDrone() {
    if (!escena || !helicoptero) {
        return;
    }

    Drone *nuevo = new Drone(helicoptero);
    qreal altoSprite = std::max<qreal>(1.0, nuevo->pixmap().height());
    int rangoY = int(altoEscena - altoSprite - 20.0);
    qreal posY = 20.0;
    if (rangoY > 1) {
        posY = QRandomGenerator::global()->bounded(rangoY);
    }

    nuevo->setPos(anchoEscena, posY);
    escena->addItem(nuevo);
    agregar(nuevo);
}

void GestorEntidades::setIntervaloSpawn(qreal segundos) {
    if (segundos < 0.4) {
        segundos = 0.4;
    }
    intervaloSpawn = segundos;
}

void GestorEntidades::setModoGeneracion(ModoGeneracion modo) {
    modoGeneracion = modo;
}

void GestorEntidades::setEdificiosAltos(bool activos) {
    edificiosAltos = activos;
}

void GestorEntidades::intentarGenerar(qreal deltaTime) {
    tiempoDesdeUltimoSpawn += deltaTime;
    if (tiempoDesdeUltimoSpawn < intervaloSpawn) {
        return;
    }
    tiempoDesdeUltimoSpawn = 0.0;

    switch (modoGeneracion) {
    case ModoGeneracion::Evacuacion: {
        // 60% edificios, 40% civiles — sin drones
        if (QRandomGenerator::global()->bounded(100) < 60) {
            generarEdificio();
        } else {
            generarCivil();
        }
        break;
    }
    case ModoGeneracion::CombateAereo: {
        // 70% drones, 30% edificios — sin civiles
        if (QRandomGenerator::global()->bounded(100) < 70) {
            generarDrone();
        } else {
            generarEdificio();
        }
        break;
    }
    case ModoGeneracion::Extraccion: {
        // mezcla agresiva: a veces genera dos amenazas
        const int roll = QRandomGenerator::global()->bounded(100);
        if (roll < 35) {
            generarEdificio();
        } else if (roll < 55) {
            generarCivil();
        } else {
            generarDrone();
        }
        if (QRandomGenerator::global()->bounded(100) < 35) {
            generarDrone();
        }
        break;
    }
    case ModoGeneracion::Mixto:
    default: {
        int eleccion = QRandomGenerator::global()->bounded(3);
        switch (eleccion) {
        case 0: generarEdificio(); break;
        case 1: generarCivil(); break;
        case 2: generarDrone(); break;
        }
        break;
    }
    }
}

Entidad *GestorEntidades::civilCercano(QGraphicsItem *objetivo, qreal radio) const {
    if (!objetivo) {
        return nullptr;
    }

    QPointF centroObjetivo = objetivo->sceneBoundingRect().center();

    for (int i = 0; i < cantidad; ++i) {
        if (!entidades[i] || entidades[i]->tipo() != TipoEntidad::Civil) {
            continue;
        }

        QPointF centroCivil = entidades[i]->sceneBoundingRect().center();
        qreal distancia = QLineF(centroObjetivo, centroCivil).length();
        if (distancia <= radio) {
            return entidades[i];
        }
    }
    return nullptr;
}

void GestorEntidades::rescatar(Entidad *civil) {
    if (!civil) {
        return;
    }
    eliminarEntidad(civil);
}

Entidad *GestorEntidades::colisionPeligro(QGraphicsItem *objetivo) const {
    if (!objetivo) {
        return nullptr;
    }

    for (int i = 0; i < cantidad; ++i) {
        if (!entidades[i]) {
            continue;
        }
        TipoEntidad t = entidades[i]->tipo();
        if (t != TipoEntidad::Obstaculo && t != TipoEntidad::Enemigo) {
            continue;
        }
        if (objetivo->collidesWithItem(entidades[i])) {
            return entidades[i];
        }
    }
    return nullptr;
}

void GestorEntidades::eliminarEntidad(Entidad *entidad) {
    if (!entidad) {
        return;
    }

    for (int i = 0; i < cantidad; ++i) {
        if (entidades[i] != entidad) {
            continue;
        }

        if (escena && entidades[i]->scene() == escena) {
            escena->removeItem(entidades[i]);
        }
        delete entidades[i];
        quitarDeArreglo(i);
        return;
    }
}

void GestorEntidades::dispararMisil(QPointF origen) {
    if (!escena) {
        return;
    }

    Misil *nuevo = new Misil(anchoEscena);
    nuevo->setPos(origen);
    escena->addItem(nuevo);
    agregar(nuevo);
}

bool GestorEntidades::resolverImpactosMisiles() {
    for (int i = 0; i < cantidad; ++i) {
        if (!entidades[i] || entidades[i]->tipo() != TipoEntidad::Misil) {
            continue;
        }

        for (int j = 0; j < cantidad; ++j) {
            if (!entidades[j] || entidades[j]->tipo() != TipoEntidad::Enemigo) {
                continue;
            }

            if (!entidades[i]->collidesWithItem(entidades[j])) {
                continue;
            }

            Entidad *drone = entidades[j];
            Entidad *misil = entidades[i];

            if (j > i) {
                eliminarEntidad(drone);
                eliminarEntidad(misil);
            } else {
                eliminarEntidad(misil);
                eliminarEntidad(drone);
            }
            return true;
        }
    }
    return false;
}
