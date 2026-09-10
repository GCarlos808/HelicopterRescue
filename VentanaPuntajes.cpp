#include "VentanaPuntajes.h"

#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>

VentanaPuntajes::VentanaPuntajes(ScoreManager* gestor, QWidget* parent)
    : QMainWindow(parent)
    , gestor_(gestor)
{
    setWindowTitle("Helicopter Rescue - Puntajes y partidas");
    setMinimumSize(900, 600);
    resize(980, 640);
    crearInterfaz();
    recargar();
}

void VentanaPuntajes::recargar()
{
    if (!gestor_) {
        return;
    }
    gestor_->cargarPuntajes();
    gestor_->cargarPartidas();
    actualizarTablaPuntajes();
    actualizarTablaPartidas();
}

void VentanaPuntajes::crearInterfaz()
{
    auto* widgetCentral = new QWidget(this);
    auto* layoutPrincipal = new QVBoxLayout(widgetCentral);

    auto* titulo = new QLabel("Gestor de puntajes y partidas guardadas", widgetCentral);
    titulo->setStyleSheet("font-size: 20px; font-weight: bold; color: #1f3b63;");
    layoutPrincipal->addWidget(titulo);

    auto* subtitulo = new QLabel(
        "Persistencia con fstream: rankings en scores.txt y partidas en saves.txt",
        widgetCentral);
    subtitulo->setStyleSheet("color: #5a6b82; margin-bottom: 8px;");
    layoutPrincipal->addWidget(subtitulo);

    pestanas_ = new QTabWidget(widgetCentral);
    pestanas_->addTab(crearPestanaPuntajes(), "Top puntajes");
    pestanas_->addTab(crearPestanaPartidas(), "Partidas guardadas");
    layoutPrincipal->addWidget(pestanas_);

    auto* layoutBotones = new QHBoxLayout();
    auto* botonRecargar = new QPushButton("Recargar archivos", widgetCentral);
    auto* botonVolver = new QPushButton("Volver al menu", widgetCentral);
    botonVolver->setObjectName("botonVolver");

    connect(botonRecargar, &QPushButton::clicked, this, &VentanaPuntajes::alRecargarDatos);
    connect(botonVolver, &QPushButton::clicked, this, &VentanaPuntajes::alVolver);

    layoutBotones->addStretch();
    layoutBotones->addWidget(botonRecargar);
    layoutBotones->addWidget(botonVolver);
    layoutPrincipal->addLayout(layoutBotones);

    setCentralWidget(widgetCentral);

    setStyleSheet(
        "QMainWindow { background: #f4f7fb; }"
        "QGroupBox { font-weight: bold; border: 1px solid #c9d6e8; border-radius: 8px; margin-top: 10px; padding-top: 12px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }"
        "QPushButton { background: #2f6fed; color: white; border: none; border-radius: 6px; padding: 8px 14px; }"
        "QPushButton:hover { background: #2559c0; }"
        "QPushButton:pressed { background: #1d4698; }"
        "#botonVolver { background: #5b6b7c; }"
        "#botonVolver:hover { background: #6d7f92; }"
        "QTableWidget { background: white; border: 1px solid #d5deea; border-radius: 6px; gridline-color: #e6edf6; }"
        "QHeaderView::section { background: #e8f0fb; padding: 6px; border: none; font-weight: bold; }"
        "QLineEdit, QSpinBox, QDoubleSpinBox { padding: 6px; border: 1px solid #c9d6e8; border-radius: 4px; background: white; }"
        "QTabWidget::pane { border: 1px solid #c9d6e8; border-radius: 6px; background: white; }"
        "QTabBar::tab { background: #e8f0fb; padding: 8px 16px; margin-right: 2px; border-top-left-radius: 6px; border-top-right-radius: 6px; }"
        "QTabBar::tab:selected { background: white; font-weight: bold; }");
}

QWidget* VentanaPuntajes::crearPestanaPuntajes()
{
    auto* contenedor = new QWidget();
    auto* layout = new QVBoxLayout(contenedor);

    tablaPuntajes_ = new QTableWidget(0, 5, contenedor);
    tablaPuntajes_->setHorizontalHeaderLabels(
        {"#", "Nombre", "Puntaje", "Tiempo (s)", "Nivel"});
    tablaPuntajes_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaPuntajes_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaPuntajes_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaPuntajes_->verticalHeader()->setVisible(false);
    layout->addWidget(tablaPuntajes_);

    auto* grupoFormulario = new QGroupBox("Agregar puntaje", contenedor);
    auto* formulario = new QFormLayout(grupoFormulario);

    entradaNombrePuntaje_ = new QLineEdit(grupoFormulario);
    entradaNombrePuntaje_->setPlaceholderText("Nombre del jugador");
    formulario->addRow("Nombre:", entradaNombrePuntaje_);

    entradaPuntaje_ = new QSpinBox(grupoFormulario);
    entradaPuntaje_->setRange(0, 999999);
    formulario->addRow("Puntaje:", entradaPuntaje_);

    entradaTiempo_ = new QDoubleSpinBox(grupoFormulario);
    entradaTiempo_->setRange(0.0, 99999.0);
    entradaTiempo_->setDecimals(1);
    entradaTiempo_->setSuffix(" s");
    formulario->addRow("Tiempo:", entradaTiempo_);

    entradaNivelPuntaje_ = new QSpinBox(grupoFormulario);
    entradaNivelPuntaje_->setRange(1, 3);
    formulario->addRow("Nivel:", entradaNivelPuntaje_);

    auto* botonAgregar = new QPushButton("Agregar al ranking", grupoFormulario);
    connect(botonAgregar, &QPushButton::clicked, this, &VentanaPuntajes::alAgregarPuntaje);
    formulario->addRow(botonAgregar);

    layout->addWidget(grupoFormulario);
    return contenedor;
}

QWidget* VentanaPuntajes::crearPestanaPartidas()
{
    auto* contenedor = new QWidget();
    auto* layout = new QVBoxLayout(contenedor);

    tablaPartidas_ = new QTableWidget(0, 9, contenedor);
    tablaPartidas_->setHorizontalHeaderLabels(
        {"Nombre", "Nivel", "Puntaje", "Salud", "Combustible", "Rescatados", "Tiempo", "Pos X", "Pos Y"});
    tablaPartidas_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaPartidas_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaPartidas_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaPartidas_->verticalHeader()->setVisible(false);
    connect(tablaPartidas_, &QTableWidget::cellClicked, this, &VentanaPuntajes::alSeleccionarPartida);
    layout->addWidget(tablaPartidas_);

    auto* grupoFormulario = new QGroupBox("Guardar / editar partida", contenedor);
    auto* formulario = new QFormLayout(grupoFormulario);

    entradaNombrePartida_ = new QLineEdit(grupoFormulario);
    entradaNombrePartida_->setPlaceholderText("Nombre del jugador");
    formulario->addRow("Nombre:", entradaNombrePartida_);

    entradaNivelPartida_ = new QSpinBox(grupoFormulario);
    entradaNivelPartida_->setRange(1, 3);
    formulario->addRow("Nivel:", entradaNivelPartida_);

    entradaPuntajePartida_ = new QSpinBox(grupoFormulario);
    entradaPuntajePartida_->setRange(0, 999999);
    formulario->addRow("Puntaje:", entradaPuntajePartida_);

    entradaSalud_ = new QSpinBox(grupoFormulario);
    entradaSalud_->setRange(0, 100);
    entradaSalud_->setValue(100);
    formulario->addRow("Salud:", entradaSalud_);

    entradaCombustible_ = new QSpinBox(grupoFormulario);
    entradaCombustible_->setRange(0, 100);
    entradaCombustible_->setValue(100);
    formulario->addRow("Combustible:", entradaCombustible_);

    entradaRescatados_ = new QSpinBox(grupoFormulario);
    entradaRescatados_->setRange(0, 99);
    formulario->addRow("Rescatados:", entradaRescatados_);

    entradaTiempoPartida_ = new QDoubleSpinBox(grupoFormulario);
    entradaTiempoPartida_->setRange(0.0, 99999.0);
    entradaTiempoPartida_->setDecimals(1);
    entradaTiempoPartida_->setSuffix(" s");
    formulario->addRow("Tiempo:", entradaTiempoPartida_);

    entradaPosX_ = new QDoubleSpinBox(grupoFormulario);
    entradaPosX_->setRange(-99999.0, 99999.0);
    entradaPosX_->setDecimals(1);
    formulario->addRow("Posicion X:", entradaPosX_);

    entradaPosY_ = new QDoubleSpinBox(grupoFormulario);
    entradaPosY_->setRange(-99999.0, 99999.0);
    entradaPosY_->setDecimals(1);
    formulario->addRow("Posicion Y:", entradaPosY_);

    auto* layoutBotones = new QHBoxLayout();
    auto* botonGuardar = new QPushButton("Guardar partida", grupoFormulario);
    auto* botonCargar = new QPushButton("Cargar partida", grupoFormulario);
    auto* botonEliminar = new QPushButton("Eliminar partida", grupoFormulario);
    botonEliminar->setStyleSheet("QPushButton { background: #d64545; } QPushButton:hover { background: #b73737; }");

    connect(botonGuardar, &QPushButton::clicked, this, &VentanaPuntajes::alGuardarPartida);
    connect(botonCargar, &QPushButton::clicked, this, &VentanaPuntajes::alCargarPartida);
    connect(botonEliminar, &QPushButton::clicked, this, &VentanaPuntajes::alEliminarPartida);

    layoutBotones->addWidget(botonGuardar);
    layoutBotones->addWidget(botonCargar);
    layoutBotones->addWidget(botonEliminar);
    formulario->addRow(layoutBotones);

    layout->addWidget(grupoFormulario);

    etiquetaDetallePartida_ = new QLabel("Selecciona una fila para ver o editar una partida.", contenedor);
    etiquetaDetallePartida_->setStyleSheet("color: #5a6b82; padding: 4px;");
    layout->addWidget(etiquetaDetallePartida_);

    return contenedor;
}

void VentanaPuntajes::actualizarTablaPuntajes()
{
    tablaPuntajes_->setRowCount(0);
    for (int i = 0; i < gestor_->obtenerCantidadPuntajes(); ++i) {
        const ScoreEntry e = gestor_->obtenerPuntaje(i);
        const int fila = tablaPuntajes_->rowCount();
        tablaPuntajes_->insertRow(fila);
        tablaPuntajes_->setItem(fila, 0, new QTableWidgetItem(QString::number(i + 1)));
        tablaPuntajes_->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(e.playerName)));
        tablaPuntajes_->setItem(fila, 2, new QTableWidgetItem(QString::number(e.score)));
        tablaPuntajes_->setItem(fila, 3, new QTableWidgetItem(QString::number(e.timeSeconds, 'f', 1)));
        tablaPuntajes_->setItem(fila, 4, new QTableWidgetItem(QString::number(e.level)));
    }
}

void VentanaPuntajes::actualizarTablaPartidas()
{
    tablaPartidas_->setRowCount(0);
    for (int i = 0; i < gestor_->obtenerCantidadPartidas(); ++i) {
        const SavedGame g = gestor_->obtenerPartida(i);
        const int fila = tablaPartidas_->rowCount();
        tablaPartidas_->insertRow(fila);
        tablaPartidas_->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(g.playerName)));
        tablaPartidas_->setItem(fila, 1, new QTableWidgetItem(QString::number(g.level)));
        tablaPartidas_->setItem(fila, 2, new QTableWidgetItem(QString::number(g.score)));
        tablaPartidas_->setItem(fila, 3, new QTableWidgetItem(QString::number(g.health)));
        tablaPartidas_->setItem(fila, 4, new QTableWidgetItem(QString::number(g.fuel)));
        tablaPartidas_->setItem(fila, 5, new QTableWidgetItem(QString::number(g.rescued)));
        tablaPartidas_->setItem(fila, 6, new QTableWidgetItem(QString::number(g.timeElapsed, 'f', 1)));
        tablaPartidas_->setItem(fila, 7, new QTableWidgetItem(QString::number(g.posX, 'f', 1)));
        tablaPartidas_->setItem(fila, 8, new QTableWidgetItem(QString::number(g.posY, 'f', 1)));
    }
}

void VentanaPuntajes::limpiarFormularioPuntaje()
{
    entradaNombrePuntaje_->clear();
    entradaPuntaje_->setValue(0);
    entradaTiempo_->setValue(0.0);
    entradaNivelPuntaje_->setValue(1);
}

void VentanaPuntajes::llenarFormularioPartida(const SavedGame& partida)
{
    entradaNombrePartida_->setText(QString::fromStdString(partida.playerName));
    entradaNivelPartida_->setValue(partida.level);
    entradaPuntajePartida_->setValue(partida.score);
    entradaSalud_->setValue(partida.health);
    entradaCombustible_->setValue(partida.fuel);
    entradaRescatados_->setValue(partida.rescued);
    entradaTiempoPartida_->setValue(partida.timeElapsed);
    entradaPosX_->setValue(partida.posX);
    entradaPosY_->setValue(partida.posY);
}

void VentanaPuntajes::mostrarMensaje(const QString& titulo, const QString& mensaje, bool esError)
{
    if (esError) {
        QMessageBox::warning(this, titulo, mensaje);
    } else {
        QMessageBox::information(this, titulo, mensaje);
    }
}

void VentanaPuntajes::alAgregarPuntaje()
{
    const QString nombre = entradaNombrePuntaje_->text().trimmed();
    if (nombre.isEmpty()) {
        mostrarMensaje("Dato faltante", "Escribe el nombre del jugador.", true);
        return;
    }

    ScoreEntry entrada;
    entrada.playerName = nombre.toStdString();
    entrada.score = entradaPuntaje_->value();
    entrada.timeSeconds = entradaTiempo_->value();
    entrada.level = entradaNivelPuntaje_->value();

    gestor_->agregarPuntaje(entrada);
    gestor_->guardarPuntajes();
    actualizarTablaPuntajes();
    limpiarFormularioPuntaje();
    mostrarMensaje("Puntaje guardado", "El puntaje se agrego al ranking y se guardo en scores.txt.");
}

void VentanaPuntajes::alGuardarPartida()
{
    const QString nombre = entradaNombrePartida_->text().trimmed();
    if (nombre.isEmpty()) {
        mostrarMensaje("Dato faltante", "Escribe el nombre del jugador.", true);
        return;
    }

    SavedGame partida;
    partida.playerName = nombre.toStdString();
    partida.level = entradaNivelPartida_->value();
    partida.score = entradaPuntajePartida_->value();
    partida.health = entradaSalud_->value();
    partida.fuel = entradaCombustible_->value();
    partida.rescued = entradaRescatados_->value();
    partida.timeElapsed = entradaTiempoPartida_->value();
    partida.posX = entradaPosX_->value();
    partida.posY = entradaPosY_->value();

    gestor_->guardarPartida(partida);
    actualizarTablaPartidas();
    etiquetaDetallePartida_->setText("Partida de " + nombre + " guardada en saves.txt.");
    mostrarMensaje("Partida guardada", "La partida quedo registrada en saves.txt.");
}

void VentanaPuntajes::alCargarPartida()
{
    const QString nombre = entradaNombrePartida_->text().trimmed();
    if (nombre.isEmpty()) {
        mostrarMensaje("Dato faltante", "Escribe el nombre del jugador a cargar.", true);
        return;
    }

    SavedGame partida;
    if (!gestor_->cargarPartida(nombre.toStdString(), partida)) {
        mostrarMensaje("No encontrada", "No existe una partida guardada con ese nombre.", true);
        return;
    }

    llenarFormularioPartida(partida);
    etiquetaDetallePartida_->setText(
        "Partida cargada: " + nombre + " | Nivel " + QString::number(partida.level)
        + " | Puntaje " + QString::number(partida.score));
    mostrarMensaje("Partida cargada", "Los datos de la partida se cargaron en el formulario.");
}

void VentanaPuntajes::alEliminarPartida()
{
    const QString nombre = entradaNombrePartida_->text().trimmed();
    if (nombre.isEmpty()) {
        mostrarMensaje("Dato faltante", "Escribe el nombre de la partida a eliminar.", true);
        return;
    }

    if (!gestor_->eliminarPartidaGuardada(nombre.toStdString())) {
        mostrarMensaje("No encontrada", "No existe una partida guardada con ese nombre.", true);
        return;
    }

    actualizarTablaPartidas();
    etiquetaDetallePartida_->setText("Partida eliminada: " + nombre);
    mostrarMensaje("Partida eliminada", "La partida se elimino de saves.txt.");
}

void VentanaPuntajes::alSeleccionarPartida(int fila, int /*columna*/)
{
    if (fila < 0 || fila >= gestor_->obtenerCantidadPartidas()) {
        return;
    }

    const SavedGame partida = gestor_->obtenerPartida(fila);
    llenarFormularioPartida(partida);
    etiquetaDetallePartida_->setText(
        "Editando partida de " + QString::fromStdString(partida.playerName));
}

void VentanaPuntajes::alRecargarDatos()
{
    recargar();
    mostrarMensaje("Datos recargados", "Se volvieron a leer scores.txt y saves.txt.");
}

void VentanaPuntajes::alVolver()
{
    close();
}
