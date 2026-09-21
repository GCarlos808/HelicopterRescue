#include "PantallaNiveles.h"
#include "ProgressManager.h"

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

PantallaNiveles::PantallaNiveles(ProgressManager* progreso, QWidget* parent)
    : QDialog(parent)
    , progreso_(progreso)
{
    setWindowTitle("Helicopter Rescue - Niveles");
    setModal(true);
    setFixedSize(540, 520);
    crearInterfaz();
    actualizarEstadoBotones();
}

void PantallaNiveles::crearInterfaz()
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(14);

    auto* titulo = new QLabel("NIVELES DESBLOQUEADOS", this);
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("font-size: 24px; font-weight: 800; color: #e9f1fb;");

    auto* subtitulo = new QLabel(
        "Cada piloto tiene su propio progreso.\n"
        "Completa el Nivel 3 para desbloquear el modo Historia.",
        this);
    subtitulo->setAlignment(Qt::AlignCenter);
    subtitulo->setStyleSheet("font-size: 13px; color: #9eb6d4;");

    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("QScrollArea { background: transparent; }");

    auto* panel = new QFrame();
    panel->setObjectName("panelNiveles");
    auto* layoutPanel = new QVBoxLayout(panel);
    layoutPanel->setContentsMargins(16, 14, 16, 14);
    layoutPanel->setSpacing(10);

    const QString descripciones[CANTIDAD_NIVELES] = {
        "Nivel 1 — Entrenamiento\nRescata 3 civiles",
        "Nivel 2 — Zona hostil\nRescata 5 civiles",
        "Nivel 3 — Evacuacion final\nRescata 7 civiles",
        "Nivel 4 — HISTORIA: Operacion Amanecer\n3 fases, 8 de vida, obstaculos variables"
    };

    for (int i = 0; i < CANTIDAD_NIVELES; ++i) {
        botonesNivel_[i] = new QPushButton(descripciones[i], panel);
        botonesNivel_[i]->setCursor(Qt::PointingHandCursor);
        botonesNivel_[i]->setMinimumHeight(62);
        const int nivel = i + 1;
        connect(botonesNivel_[i], &QPushButton::clicked, this, [this, nivel]() {
            emit nivelElegido(nivel);
            accept();
        });
        layoutPanel->addWidget(botonesNivel_[i]);
    }

    scroll->setWidget(panel);

    auto* botonVolver = new QPushButton("Volver al menu", this);
    botonVolver->setObjectName("botonVolver");
    botonVolver->setCursor(Qt::PointingHandCursor);
    botonVolver->setMinimumHeight(40);
    connect(botonVolver, &QPushButton::clicked, this, [this]() {
        emit volverMenu();
        reject();
    });

    layout->addWidget(titulo);
    layout->addWidget(subtitulo);
    layout->addWidget(scroll, 1);
    layout->addWidget(botonVolver);

    setStyleSheet(
        "QDialog {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #0b1a2e, stop:1 #1c3a57);"
        "}"
        "#panelNiveles {"
        "  background: rgba(8, 18, 32, 180);"
        "  border: 1px solid #355578;"
        "  border-radius: 12px;"
        "}"
        "QPushButton {"
        "  background: #2f6fed;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 10px 14px;"
        "  font-size: 13px;"
        "  font-weight: 600;"
        "  text-align: center;"
        "}"
        "QPushButton:hover { background: #3d7fff; }"
        "QPushButton:disabled {"
        "  background: #2a3a4d;"
        "  color: #7f95b3;"
        "}"
        "#botonVolver { background: #5b6b7c; }"
        "#botonVolver:hover { background: #6d7f92; }");
}

void PantallaNiveles::actualizarEstadoBotones()
{
    const QString descripciones[CANTIDAD_NIVELES] = {
        "Nivel 1 — Entrenamiento\nRescata 3 civiles",
        "Nivel 2 — Zona hostil\nRescata 5 civiles",
        "Nivel 3 — Evacuacion final\nRescata 7 civiles",
        "Nivel 4 — HISTORIA: Operacion Amanecer\n3 fases, 8 de vida, obstaculos variables"
    };

    for (int i = 0; i < CANTIDAD_NIVELES; ++i) {
        const int nivel = i + 1;
        const bool libre = progreso_ && progreso_->estaDesbloqueado(nivel);
        botonesNivel_[i]->setEnabled(libre);
        if (libre) {
            botonesNivel_[i]->setText(descripciones[i]);
        } else {
            botonesNivel_[i]->setText(
                QString("Nivel %1 — BLOQUEADO\nCompleta el nivel %2 primero")
                    .arg(nivel)
                    .arg(nivel - 1));
        }
    }
}
