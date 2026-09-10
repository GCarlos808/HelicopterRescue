#include "ScoreManager.h"
#include <fstream>
#include <sstream>

ScoreManager::ScoreManager(const std::string& archivoPuntajes, const std::string& archivoPartidas)
    : rutaPuntajes_(archivoPuntajes)
    , rutaPartidas_(archivoPartidas)
    , puntajes_(nullptr)
    , cantidadPuntajes_(0)
    , capacidadPuntajes_(0)
    , partidas_(nullptr)
    , cantidadPartidas_(0)
    , capacidadPartidas_(0)
{
    cargarPuntajes();
    cargarPartidas();
}

ScoreManager::~ScoreManager()
{
    liberarTodo();
}

void ScoreManager::liberarTodo()
{
    delete[] puntajes_;
    puntajes_ = nullptr;
    cantidadPuntajes_ = 0;
    capacidadPuntajes_ = 0;

    delete[] partidas_;
    partidas_ = nullptr;
    cantidadPartidas_ = 0;
    capacidadPartidas_ = 0;
}

void ScoreManager::asegurarCapacidadPuntajes(int necesaria)
{
    if (necesaria <= capacidadPuntajes_) {
        return;
    }

    int nuevaCap = (capacidadPuntajes_ == 0) ? 4 : capacidadPuntajes_ * 2;
    while (nuevaCap < necesaria) {
        nuevaCap *= 2;
    }

    ScoreEntry* nuevo = new ScoreEntry[nuevaCap];
    for (int i = 0; i < cantidadPuntajes_; ++i) {
        nuevo[i] = puntajes_[i];
    }

    delete[] puntajes_;
    puntajes_ = nuevo;
    capacidadPuntajes_ = nuevaCap;
}

void ScoreManager::asegurarCapacidadPartidas(int necesaria)
{
    if (necesaria <= capacidadPartidas_) {
        return;
    }

    int nuevaCap = (capacidadPartidas_ == 0) ? 4 : capacidadPartidas_ * 2;
    while (nuevaCap < necesaria) {
        nuevaCap *= 2;
    }

    SavedGame* nuevo = new SavedGame[nuevaCap];
    for (int i = 0; i < cantidadPartidas_; ++i) {
        nuevo[i] = partidas_[i];
    }

    delete[] partidas_;
    partidas_ = nuevo;
    capacidadPartidas_ = nuevaCap;
}

// Formato scores.txt (una línea por registro):
// nombre|puntaje|tiempo|nivel
bool ScoreManager::cargarPuntajes()
{
    std::ifstream entrada(rutaPuntajes_);
    if (!entrada.is_open()) {
        // Primera vez: no hay archivo aún; no es error.
        return false;
    }

    limpiarPuntajes();

    std::string linea;
    while (std::getline(entrada, linea)) {
        if (linea.empty()) {
            continue;
        }

        std::stringstream ss(linea);
        std::string nombre, puntajeStr, tiempoStr, nivelStr;

        if (!std::getline(ss, nombre, '|')) continue;
        if (!std::getline(ss, puntajeStr, '|')) continue;
        if (!std::getline(ss, tiempoStr, '|')) continue;
        if (!std::getline(ss, nivelStr, '|')) continue;

        ScoreEntry e;
        e.playerName = nombre;
        e.score = std::stoi(puntajeStr);
        e.timeSeconds = std::stod(tiempoStr);
        e.level = std::stoi(nivelStr);
        agregarPuntaje(e);
    }

    entrada.close();
    return true;
}

bool ScoreManager::guardarPuntajes() const
{
    std::ofstream salida(rutaPuntajes_);
    if (!salida.is_open()) {
        return false;
    }

    for (int i = 0; i < cantidadPuntajes_; ++i) {
        salida << puntajes_[i].playerName << '|'
               << puntajes_[i].score << '|'
               << puntajes_[i].timeSeconds << '|'
               << puntajes_[i].level << '\n';
    }

    salida.close();
    return true;
}

void ScoreManager::agregarPuntaje(const ScoreEntry& entrada)
{
    asegurarCapacidadPuntajes(cantidadPuntajes_ + 1);

    // Insertar ordenado por puntaje descendente (mejor primero).
    int pos = cantidadPuntajes_;
    for (int i = 0; i < cantidadPuntajes_; ++i) {
        if (entrada.score > puntajes_[i].score) {
            pos = i;
            break;
        }
    }

    for (int i = cantidadPuntajes_; i > pos; --i) {
        puntajes_[i] = puntajes_[i - 1];
    }

    puntajes_[pos] = entrada;
    ++cantidadPuntajes_;

    // Limitar top 20
    const int kMaxPuntajes = 20;
    if (cantidadPuntajes_ > kMaxPuntajes) {
        cantidadPuntajes_ = kMaxPuntajes;
    }
}

int ScoreManager::obtenerCantidadPuntajes() const
{
    return cantidadPuntajes_;
}

ScoreEntry ScoreManager::obtenerPuntaje(int indice) const
{
    if (indice < 0 || indice >= cantidadPuntajes_) {
        return ScoreEntry{};
    }
    return puntajes_[indice];
}

void ScoreManager::limpiarPuntajes()
{
    cantidadPuntajes_ = 0;
}

// Formato saves.txt:
// nombre|nivel|puntaje|salud|combustible|rescatados|tiempo|posX|posY
bool ScoreManager::cargarPartidas()
{
    std::ifstream entrada(rutaPartidas_);
    if (!entrada.is_open()) {
        return false;
    }

    cantidadPartidas_ = 0;

    std::string linea;
    while (std::getline(entrada, linea)) {
        if (linea.empty()) {
            continue;
        }

        std::stringstream ss(linea);
        std::string partes[9];
        bool ok = true;
        for (int i = 0; i < 9; ++i) {
            if (!std::getline(ss, partes[i], '|')) {
                ok = false;
                break;
            }
        }
        if (!ok) {
            continue;
        }

        SavedGame g;
        g.playerName = partes[0];
        g.level = std::stoi(partes[1]);
        g.score = std::stoi(partes[2]);
        g.health = std::stoi(partes[3]);
        g.fuel = std::stoi(partes[4]);
        g.rescued = std::stoi(partes[5]);
        g.timeElapsed = std::stod(partes[6]);
        g.posX = std::stod(partes[7]);
        g.posY = std::stod(partes[8]);

        asegurarCapacidadPartidas(cantidadPartidas_ + 1);
        partidas_[cantidadPartidas_] = g;
        ++cantidadPartidas_;
    }

    entrada.close();
    return true;
}

bool ScoreManager::guardarPartidas() const
{
    std::ofstream salida(rutaPartidas_);
    if (!salida.is_open()) {
        return false;
    }

    for (int i = 0; i < cantidadPartidas_; ++i) {
        salida << partidas_[i].playerName << '|'
               << partidas_[i].level << '|'
               << partidas_[i].score << '|'
               << partidas_[i].health << '|'
               << partidas_[i].fuel << '|'
               << partidas_[i].rescued << '|'
               << partidas_[i].timeElapsed << '|'
               << partidas_[i].posX << '|'
               << partidas_[i].posY << '\n';
    }

    salida.close();
    return true;
}

int ScoreManager::buscarIndicePartida(const std::string& nombreJugador) const
{
    for (int i = 0; i < cantidadPartidas_; ++i) {
        if (partidas_[i].playerName == nombreJugador) {
            return i;
        }
    }
    return -1;
}

void ScoreManager::guardarPartida(const SavedGame& partida)
{
    int idx = buscarIndicePartida(partida.playerName);
    if (idx >= 0) {
        partidas_[idx] = partida;
    } else {
        asegurarCapacidadPartidas(cantidadPartidas_ + 1);
        partidas_[cantidadPartidas_] = partida;
        ++cantidadPartidas_;
    }
    guardarPartidas();
}

bool ScoreManager::cargarPartida(const std::string& nombreJugador, SavedGame& salida) const
{
    int idx = buscarIndicePartida(nombreJugador);
    if (idx < 0) {
        return false;
    }
    salida = partidas_[idx];
    return true;
}

bool ScoreManager::tienePartidaGuardada(const std::string& nombreJugador) const
{
    return buscarIndicePartida(nombreJugador) >= 0;
}

bool ScoreManager::eliminarPartidaGuardada(const std::string& nombreJugador)
{
    int idx = buscarIndicePartida(nombreJugador);
    if (idx < 0) {
        return false;
    }

    for (int i = idx; i < cantidadPartidas_ - 1; ++i) {
        partidas_[i] = partidas_[i + 1];
    }
    --cantidadPartidas_;
    guardarPartidas();
    return true;
}

int ScoreManager::obtenerCantidadPartidas() const
{
    return cantidadPartidas_;
}

SavedGame ScoreManager::obtenerPartida(int indice) const
{
    if (indice < 0 || indice >= cantidadPartidas_) {
        return SavedGame{};
    }
    return partidas_[indice];
}
