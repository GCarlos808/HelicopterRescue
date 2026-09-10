#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include "ScoreEntry.h"
#include "SavedGame.h"
#include <string>

// Persistencia con fstream: rankings + partidas guardadas.
// Usa arreglo dinámico (new/delete) para practicar gestión de memoria.
class ScoreManager {
public:
    explicit ScoreManager(const std::string& archivoPuntajes = "scores.txt",
                          const std::string& archivoPartidas = "saves.txt");
    ~ScoreManager();

    // Regla de 3: bloquear copia (posee memoria dinámica)
    ScoreManager(const ScoreManager&) = delete;
    ScoreManager& operator=(const ScoreManager&) = delete;

    //  Rankings
    bool cargarPuntajes();
    bool guardarPuntajes() const;
    void agregarPuntaje(const ScoreEntry& entrada);  // inserta ordenado por puntaje desc
    int obtenerCantidadPuntajes() const;
    ScoreEntry obtenerPuntaje(int indice) const;     // índice 0 = mejor puntaje
    void limpiarPuntajes();

    // Partidas guardadas
    bool cargarPartidas();
    bool guardarPartidas() const;
    void guardarPartida(const SavedGame& partida);   // crea o actualiza por nombre
    bool cargarPartida(const std::string& nombreJugador, SavedGame& salida) const;
    bool tienePartidaGuardada(const std::string& nombreJugador) const;
    bool eliminarPartidaGuardada(const std::string& nombreJugador);
    int obtenerCantidadPartidas() const;
    SavedGame obtenerPartida(int indice) const;

private:
    std::string rutaPuntajes_;
    std::string rutaPartidas_;

    ScoreEntry* puntajes_;   // arreglo dinámico
    int cantidadPuntajes_;
    int capacidadPuntajes_;

    SavedGame* partidas_;
    int cantidadPartidas_;
    int capacidadPartidas_;

    void asegurarCapacidadPuntajes(int necesaria);
    void asegurarCapacidadPartidas(int necesaria);
    int buscarIndicePartida(const std::string& nombreJugador) const;
    void liberarTodo();
};

#endif
