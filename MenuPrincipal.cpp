#include "MenuPrincipal.h"
#include "NucleoFisico.h"
#include "PantallaNiveles.h"
#include "VentanaPuntajes.h"

#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

MenuPrincipal::MenuPrincipal(QWidget* parent)
    : QMainWindow(parent)
    , gestorPuntajes_("scores.txt", "saves.txt")
    , progreso_("progress.txt")
    , accionPostCierre_(AccionPostCierre::MostrarMenu)
    , nivelPendiente_(1)
{
    setWindowTitle("Helicopter Rescue");
    setFixedSize(900, 580);
    crearInterfaz();
}

MenuPrincipal::~MenuPrincipal()
{
    gestorPuntajes_.guardarPuntajes();
    gestorPuntajes_.guardarPartidas();
    progreso_.guardar();
}

void MenuPrincipal::crearInterfaz()
{
    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);
    layout->setContentsMargins(40, 30, 40, 30);
    layout->setSpacing(16);

    auto* titulo = new QLabel("HELICOPTER RESCUE", central);
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet(
        "font-size: 42px; font-weight: 800; color: #f4f7fb;"
        "letter-spacing: 2px;");

    auto* subtitulo = new QLabel("Misión de rescate aereo", central);
    subtitulo->setAlignment(Qt::AlignCenter);
    subtitulo->setStyleSheet("font-size: 16px; color: #9eb6d4; margin-bottom: 10px;");

    auto* panel = new QFrame(central);
    panel->setObjectName("panelMenu");
    auto* layoutPanel = new QVBoxLayout(panel);
    layoutPanel->setContentsMargins(28, 24, 28, 24);
    layoutPanel->setSpacing(12);

    auto* etiquetaNombre = new QLabel("Nombre del piloto:", panel);
    etiquetaNombre->setStyleSheet("color: #d7e4f5; font-size: 14px;");

    entradaNombre_ = new QLineEdit(panel);
    entradaNombre_->setPlaceholderText("Escribe tu nombre...");
    entradaNombre_->setMaxLength(24);
    entradaNombre_->setMinimumHeight(36);

    auto* botonJugar = crearBotonMenu("Jugar");
    auto* botonNiveles = crearBotonMenu("Niveles desbloqueados");
    auto* botonPuntajes = crearBotonMenu("Puntajes y partidas");
    auto* botonInstrucciones = crearBotonMenu("Instrucciones");
    auto* botonSalir = crearBotonMenu("Salir");
    botonSalir->setObjectName("botonSalir");

    connect(botonJugar, &QPushButton::clicked, this, &MenuPrincipal::alJugar);
    connect(botonNiveles, &QPushButton::clicked, this, &MenuPrincipal::alVerNiveles);
    connect(botonPuntajes, &QPushButton::clicked, this, &MenuPrincipal::alVerPuntajes);
    connect(botonInstrucciones, &QPushButton::clicked, this, &MenuPrincipal::alVerInstrucciones);
    connect(botonSalir, &QPushButton::clicked, this, &MenuPrincipal::alSalir);

    layoutPanel->addWidget(etiquetaNombre);
    layoutPanel->addWidget(entradaNombre_);
    layoutPanel->addSpacing(8);
    layoutPanel->addWidget(botonJugar);
    layoutPanel->addWidget(botonNiveles);
    layoutPanel->addWidget(botonPuntajes);
    layoutPanel->addWidget(botonInstrucciones);
    layoutPanel->addWidget(botonSalir);

    layout->addStretch();
    layout->addWidget(titulo);
    layout->addWidget(subtitulo);
    layout->addWidget(panel, 0, Qt::AlignHCenter);
    layout->addStretch();

    setCentralWidget(central);

    setStyleSheet(
        "QMainWindow {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #0b1a2e, stop:0.55 #132842, stop:1 #1c3a57);"
        "}"
        "#panelMenu {"
        "  background: rgba(8, 18, 32, 180);"
        "  border: 1px solid #355578;"
        "  border-radius: 14px;"
        "  min-width: 360px;"
        "}"
        "QLineEdit {"
        "  background: #0f2138;"
        "  color: #e9f1fb;"
        "  border: 1px solid #3d6085;"
        "  border-radius: 8px;"
        "  padding: 6px 10px;"
        "  font-size: 14px;"
        "}"
        "QPushButton {"
        "  background: #2f6fed;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 12px 18px;"
        "  font-size: 15px;"
        "  font-weight: 600;"
        "}"
        "QPushButton:hover { background: #3d7fff; }"
        "QPushButton:pressed { background: #2559c0; }"
        "#botonSalir { background: #5b6b7c; }"
        "#botonSalir:hover { background: #6d7f92; }");
}

QPushButton* MenuPrincipal::crearBotonMenu(const QString& texto)
{
    auto* boton = new QPushButton(texto, this);
    boton->setCursor(Qt::PointingHandCursor);
    boton->setMinimumHeight(42);
    return boton;
}

QString MenuPrincipal::nombrePilotoActual() const
{
    return entradaNombre_->text().trimmed();
}

void MenuPrincipal::conectarSenalesJuego(NucleoFisico* juego)
{
    connect(juego, &QObject::destroyed, this, &MenuPrincipal::alCerrarJuego);
    connect(juego, &NucleoFisico::solicitarReiniciar, this, &MenuPrincipal::alProgramarReinicio);
    connect(juego, &NucleoFisico::solicitarNivel, this, &MenuPrincipal::alProgramarNivel);
    connect(juego, &NucleoFisico::solicitarNiveles, this, &MenuPrincipal::alProgramarNiveles);
    connect(juego, &NucleoFisico::solicitarMenu, this, &MenuPrincipal::alProgramarMenu);
}

void MenuPrincipal::alJugar()
{
    const QString nombre = nombrePilotoActual();
    if (nombre.isEmpty()) {
        QMessageBox::warning(this, "Nombre requerido",
                             "Escribe tu nombre de piloto antes de jugar.");
        entradaNombre_->setFocus();
        return;
    }

    progreso_.seleccionarPiloto(nombre.toStdString());
    // Un piloto nuevo solo tiene el nivel 1; si ya avanzo, continua en su maximo.
    alIniciarNivel(progreso_.nivelMaximoDesbloqueado());
}

void MenuPrincipal::alVerNiveles()
{
    const QString nombre = nombrePilotoActual();
    if (nombre.isEmpty()) {
        QMessageBox::warning(this, "Nombre requerido",
                             "Escribe tu nombre de piloto antes de elegir nivel.");
        entradaNombre_->setFocus();
        return;
    }

    progreso_.seleccionarPiloto(nombre.toStdString());

    if (ventanaNiveles_) {
        ventanaNiveles_->close();
        ventanaNiveles_ = nullptr;
    }

    ventanaNiveles_ = new PantallaNiveles(&progreso_, this);
    ventanaNiveles_->setAttribute(Qt::WA_DeleteOnClose);
    connect(ventanaNiveles_, &PantallaNiveles::nivelElegido, this, &MenuPrincipal::alIniciarNivel);
    ventanaNiveles_->exec();
}

void MenuPrincipal::alProgramarReinicio(int nivel)
{
    accionPostCierre_ = AccionPostCierre::AbrirNivel;
    nivelPendiente_ = nivel;
}

void MenuPrincipal::alProgramarNivel(int nivel)
{
    accionPostCierre_ = AccionPostCierre::AbrirNivel;
    nivelPendiente_ = nivel;
}

void MenuPrincipal::alProgramarNiveles()
{
    accionPostCierre_ = AccionPostCierre::AbrirNiveles;
}

void MenuPrincipal::alProgramarMenu()
{
    accionPostCierre_ = AccionPostCierre::MostrarMenu;
}

void MenuPrincipal::alIniciarNivel(int nivel)
{
    const QString nombre = nombrePilotoActual();
    if (nombre.isEmpty()) {
        QMessageBox::warning(this, "Nombre requerido", "Escribe tu nombre de piloto antes de jugar.");
        entradaNombre_->setFocus();
        return;
    }

    progreso_.seleccionarPiloto(nombre.toStdString());

    if (!progreso_.estaDesbloqueado(nivel)) {
        QMessageBox::information(this, "Nivel bloqueado", "Ese nivel aun no esta desbloqueado para este piloto.");
        return;
    }

    if (ventanaJuego_) {
        accionPostCierre_ = AccionPostCierre::AbrirNivel;
        nivelPendiente_ = nivel;
        ventanaJuego_->close();
        return;
    }

    accionPostCierre_ = AccionPostCierre::MostrarMenu;
    ventanaJuego_ = new NucleoFisico(nombre, nivel, &gestorPuntajes_, &progreso_, nullptr);
    ventanaJuego_->setAttribute(Qt::WA_DeleteOnClose);
    conectarSenalesJuego(ventanaJuego_);

    hide();
    ventanaJuego_->show();
    ventanaJuego_->setFocus();
}

void MenuPrincipal::alVerPuntajes()
{
    if (!ventanaPuntajes_) {
        ventanaPuntajes_ = new VentanaPuntajes(&gestorPuntajes_, this);
        ventanaPuntajes_->setAttribute(Qt::WA_DeleteOnClose);
    }

    ventanaPuntajes_->recargar();
    ventanaPuntajes_->show();
    ventanaPuntajes_->raise();
    ventanaPuntajes_->activateWindow();
}

void MenuPrincipal::alVerInstrucciones()
{
    QMessageBox::information(
        this,
        "Instrucciones",
        "Objetivo:\n"
        "Controla el helicoptero, evita obstaculos y rescata civiles.\n\n"
        "Niveles:\n"
        "- Nivel 1: rescata 3 civiles\n"
        "- Nivel 2: rescata 5 civiles\n"
        "- Nivel 3: rescata 7 civiles\n"
        "- Historia (Nivel 4): 3 fases con objetivos distintos,\n"
        "  8 de vida y obstaculos variables\n\n"
        "Controles:\n"
        "- Flechas: mover / ascender\n"
        "- Espacio: disparar misil\n"
        "Despega, avanza por los 11 sectores del mapa, rescata civiles\n"
        "y aterriza en el hangar final para completar el nivel 1.\n"
        "Cada civil rescatado suma 100 puntos.\n\n"
        "Rescate:\n"
        "- Debes aterrizar con suavidad cerca de un civil.\n"
        "- Si tocas el suelo a gran velocidad, pierdes toda la vida.\n"
        "- En el sector 11, aterriza en el hangar para entregar a los civiles.\n\n"
        "Controles:\n"
        "- Flecha Arriba: despegar / ascender\n"
        "- Flechas Izquierda/Derecha: moverse e inclinarse\n"
        "- Espacio: disparar (cooldown de 3 segundos)\n"
        "- El escenario no avanza hasta que despegues.\n"
        );
}

void MenuPrincipal::alSalir()
{
    QApplication::quit();
}

void MenuPrincipal::alCerrarJuego()
{
    ventanaJuego_ = nullptr;

    const AccionPostCierre accion = accionPostCierre_;
    const int nivel = nivelPendiente_;
    accionPostCierre_ = AccionPostCierre::MostrarMenu;

    if (accion == AccionPostCierre::AbrirNivel) {
        alIniciarNivel(nivel);
        return;
    }

    show();
    raise();
    activateWindow();

    if (accion == AccionPostCierre::AbrirNiveles) {
        alVerNiveles();
    }
}