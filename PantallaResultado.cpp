#include "PantallaResultado.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PantallaResultado::PantallaResultado(Tipo tipo,
                                     const QString& nombrePiloto,
                                     int nivel,
                                     int puntaje,
                                     int rescatados,
                                     double tiempoSegundos,
                                     QWidget* parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setFixedSize(520, 360);
    crearInterfaz(tipo, nombrePiloto, nivel, puntaje, rescatados, tiempoSegundos);
}

void PantallaResultado::crearInterfaz(Tipo tipo,
                                      const QString& nombrePiloto,
                                      int nivel,
                                      int puntaje,
                                      int rescatados,
                                      double tiempoSegundos)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 28, 28, 28);
    layout->setSpacing(14);

    QString titulo;
    QString subtitulo;
    QString colorTitulo = "#e9f1fb";

    switch (tipo) {
    case Tipo::Derrota:
        titulo = "MISION FALLIDA";
        subtitulo = "El helicoptero fue destruido.";
        colorTitulo = "#ff6b6b";
        break;
    case Tipo::SiguienteNivel:
        if (nivel >= 3) {
            titulo = "NIVEL 3 COMPLETADO";
            subtitulo = "Modo Historia desbloqueado: Operacion Amanecer.";
        } else {
            titulo = QString("NIVEL %1 COMPLETADO").arg(nivel);
            subtitulo = QString("Nivel %1 desbloqueado. Preparate para lo que sigue.").arg(nivel + 1);
        }
        colorTitulo = "#5ad67d";
        break;
    case Tipo::VictoriaFinal:
        if (nivel >= 4) {
            titulo = "HISTORIA COMPLETADA";
            subtitulo = "Operacion Amanecer exitosa. Completaste las 3 fases de la mision.";
        } else {
            titulo = "MISION COMPLETADA";
            subtitulo = "Rescataste a todos en el Nivel 3. Excelente trabajo, piloto.";
        }
        colorTitulo = "#ffd166";
        break;
    }

    auto* labelTitulo = new QLabel(titulo, this);
    labelTitulo->setAlignment(Qt::AlignCenter);
    labelTitulo->setStyleSheet(
        QString("font-size: 28px; font-weight: 800; color: %1; letter-spacing: 1px;").arg(colorTitulo));

    auto* labelSub = new QLabel(subtitulo, this);
    labelSub->setAlignment(Qt::AlignCenter);
    labelSub->setWordWrap(true);
    labelSub->setStyleSheet("font-size: 14px; color: #9eb6d4;");

    auto* panel = new QFrame(this);
    panel->setObjectName("panelStats");
    auto* layoutStats = new QVBoxLayout(panel);
    layoutStats->setContentsMargins(18, 14, 18, 14);
    layoutStats->setSpacing(6);

    auto agregarStat = [&](const QString& texto) {
        auto* lab = new QLabel(texto, panel);
        lab->setAlignment(Qt::AlignCenter);
        lab->setStyleSheet("color: #d7e4f5; font-size: 15px;");
        layoutStats->addWidget(lab);
    };

    agregarStat("Piloto: " + nombrePiloto);
    agregarStat(QString("Nivel: %1").arg(nivel));
    agregarStat(QString("Puntaje: %1").arg(puntaje));
    agregarStat(QString("Rescatados: %1").arg(rescatados));
    agregarStat(QString("Tiempo: %1 s").arg(tiempoSegundos, 0, 'f', 1));

    auto* layoutBotones = new QHBoxLayout();
    layoutBotones->setSpacing(10);

    if (tipo == Tipo::Derrota) {
        auto* botonReiniciar = crearBoton("Reintentar");
        auto* botonNiveles = crearBoton("Niveles", "botonSecundario");
        auto* botonMenu = crearBoton("Menu", "botonSecundario");
        connect(botonReiniciar, &QPushButton::clicked, this, &PantallaResultado::reiniciarSolicitado);
        connect(botonNiveles, &QPushButton::clicked, this, &PantallaResultado::nivelesSolicitados);
        connect(botonMenu, &QPushButton::clicked, this, &PantallaResultado::menuSolicitado);
        layoutBotones->addWidget(botonReiniciar);
        layoutBotones->addWidget(botonNiveles);
        layoutBotones->addWidget(botonMenu);
    } else if (tipo == Tipo::SiguienteNivel) {
        const QString textoContinuar = (nivel >= 3) ? "Ir a Historia" : "Siguiente nivel";
        auto* botonContinuar = crearBoton(textoContinuar);
        auto* botonNiveles = crearBoton("Niveles", "botonSecundario");
        auto* botonMenu = crearBoton("Menu", "botonSecundario");
        connect(botonContinuar, &QPushButton::clicked, this, &PantallaResultado::continuarSolicitado);
        connect(botonNiveles, &QPushButton::clicked, this, &PantallaResultado::nivelesSolicitados);
        connect(botonMenu, &QPushButton::clicked, this, &PantallaResultado::menuSolicitado);
        layoutBotones->addWidget(botonContinuar);
        layoutBotones->addWidget(botonNiveles);
        layoutBotones->addWidget(botonMenu);
    } else {
        auto* botonNiveles = crearBoton("Ver niveles");
        auto* botonMenu = crearBoton("Menu", "botonSecundario");
        connect(botonNiveles, &QPushButton::clicked, this, &PantallaResultado::nivelesSolicitados);
        connect(botonMenu, &QPushButton::clicked, this, &PantallaResultado::menuSolicitado);
        layoutBotones->addWidget(botonNiveles);
        layoutBotones->addWidget(botonMenu);
    }

    layout->addWidget(labelTitulo);
    layout->addWidget(labelSub);
    layout->addWidget(panel);
    layout->addStretch();
    layout->addLayout(layoutBotones);

    setStyleSheet(
        "QDialog {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #0b1a2e, stop:1 #1c3a57);"
        "  border: 2px solid #355578;"
        "  border-radius: 14px;"
        "}"
        "#panelStats {"
        "  background: rgba(8, 18, 32, 180);"
        "  border: 1px solid #355578;"
        "  border-radius: 10px;"
        "}"
        "QPushButton {"
        "  background: #2f6fed;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 10px 16px;"
        "  font-size: 14px;"
        "  font-weight: 600;"
        "  min-height: 38px;"
        "}"
        "QPushButton:hover { background: #3d7fff; }"
        "QPushButton:pressed { background: #2559c0; }"
        "#botonSecundario { background: #5b6b7c; }"
        "#botonSecundario:hover { background: #6d7f92; }");
}

QPushButton* PantallaResultado::crearBoton(const QString& texto, const QString& objectName)
{
    auto* boton = new QPushButton(texto, this);
    boton->setCursor(Qt::PointingHandCursor);
    if (!objectName.isEmpty()) {
        boton->setObjectName(objectName);
    }
    return boton;
}
