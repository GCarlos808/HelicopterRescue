#ifndef PROGRESSMANAGER_H
#define PROGRESSMANAGER_H

#include <string>

// Progreso por piloto: un jugador nuevo empieza en nivel 1.
// Formato progress.txt: nombre|nivelMaximo  (una linea por piloto)
class ProgressManager {
public:
    explicit ProgressManager(const std::string& archivo = "progress.txt");
    ~ProgressManager();

    ProgressManager(const ProgressManager&) = delete;
    ProgressManager& operator=(const ProgressManager&) = delete;

    // Activa el piloto actual. Si no existe, lo crea con nivel 1.
    void seleccionarPiloto(const std::string& nombre);

    bool hayPilotoSeleccionado() const;
    std::string pilotoActual() const;

    int nivelMaximoDesbloqueado() const;
    void desbloquearNivel(int nivel);
    bool estaDesbloqueado(int nivel) const;

    bool cargar();
    bool guardar() const;

private:
    struct Registro {
        std::string nombre;
        int nivelMaximo = 1;
    };

    std::string ruta_;
    Registro* registros_;
    int cantidad_;
    int capacidad_;
    int indiceActual_;

    void asegurarCapacidad(int necesaria);
    int buscarIndice(const std::string& nombre) const;
    void liberar();
};

#endif // PROGRESSMANAGER_H
