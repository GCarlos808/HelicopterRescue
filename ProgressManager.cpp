#include "ProgressManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>

ProgressManager::ProgressManager(const std::string& archivo)
    : ruta_(archivo)
    , registros_(nullptr)
    , cantidad_(0)
    , capacidad_(0)
    , indiceActual_(-1)
{
    cargar();
}

ProgressManager::~ProgressManager()
{
    liberar();
}

void ProgressManager::liberar()
{
    delete[] registros_;
    registros_ = nullptr;
    cantidad_ = 0;
    capacidad_ = 0;
    indiceActual_ = -1;
}

void ProgressManager::asegurarCapacidad(int necesaria)
{
    if (necesaria <= capacidad_) {
        return;
    }

    int nuevaCap = (capacidad_ == 0) ? 4 : capacidad_ * 2;
    while (nuevaCap < necesaria) {
        nuevaCap *= 2;
    }

    Registro* nuevo = new Registro[nuevaCap];
    for (int i = 0; i < cantidad_; ++i) {
        nuevo[i] = registros_[i];
    }

    delete[] registros_;
    registros_ = nuevo;
    capacidad_ = nuevaCap;
}

int ProgressManager::buscarIndice(const std::string& nombre) const
{
    for (int i = 0; i < cantidad_; ++i) {
        if (registros_[i].nombre == nombre) {
            return i;
        }
    }
    return -1;
}

void ProgressManager::seleccionarPiloto(const std::string& nombre)
{
    if (nombre.empty()) {
        indiceActual_ = -1;
        return;
    }

    int idx = buscarIndice(nombre);
    if (idx < 0) {
        // Jugador nuevo: empieza desde el nivel 1.
        asegurarCapacidad(cantidad_ + 1);
        registros_[cantidad_].nombre = nombre;
        registros_[cantidad_].nivelMaximo = 1;
        idx = cantidad_;
        cantidad_++;
        guardar();
    }
    indiceActual_ = idx;
}

bool ProgressManager::hayPilotoSeleccionado() const
{
    return indiceActual_ >= 0 && indiceActual_ < cantidad_;
}

std::string ProgressManager::pilotoActual() const
{
    if (!hayPilotoSeleccionado()) {
        return {};
    }
    return registros_[indiceActual_].nombre;
}

int ProgressManager::nivelMaximoDesbloqueado() const
{
    if (!hayPilotoSeleccionado()) {
        return 1;
    }
    return registros_[indiceActual_].nivelMaximo;
}

void ProgressManager::desbloquearNivel(int nivel)
{
    if (!hayPilotoSeleccionado()) {
        return;
    }

    nivel = std::clamp(nivel, 1, 4);
    if (nivel > registros_[indiceActual_].nivelMaximo) {
        registros_[indiceActual_].nivelMaximo = nivel;
        guardar();
    }
}

bool ProgressManager::estaDesbloqueado(int nivel) const
{
    return nivel >= 1 && nivel <= nivelMaximoDesbloqueado();
}

bool ProgressManager::cargar()
{
    liberar();

    std::ifstream in(ruta_);
    if (!in) {
        return false;
    }

    std::string linea;
    while (std::getline(in, linea)) {
        if (linea.empty()) {
            continue;
        }

        // Compatibilidad con formato antiguo: solo un numero global.
        if (linea.find('|') == std::string::npos) {
            std::istringstream legacy(linea);
            int valor = 1;
            if (legacy >> valor) {
                // Se ignora el progreso global; cada piloto empezara en 1.
            }
            continue;
        }

        const auto sep = linea.find('|');
        const std::string nombre = linea.substr(0, sep);
        int nivel = 1;
        try {
            nivel = std::stoi(linea.substr(sep + 1));
        } catch (...) {
            nivel = 1;
        }

        if (nombre.empty()) {
            continue;
        }

        asegurarCapacidad(cantidad_ + 1);
        registros_[cantidad_].nombre = nombre;
        registros_[cantidad_].nivelMaximo = std::clamp(nivel, 1, 4);
        cantidad_++;
    }

    return true;
}

bool ProgressManager::guardar() const
{
    std::ofstream out(ruta_, std::ios::trunc);
    if (!out) {
        return false;
    }

    for (int i = 0; i < cantidad_; ++i) {
        out << registros_[i].nombre << '|' << registros_[i].nivelMaximo << '\n';
    }
    return true;
}
