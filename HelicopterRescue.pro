QT += widgets

CONFIG += c++17

SOURCES += \
    Civil.cpp \
    Drone.cpp \
    Edificio.cpp \
    Entidad.cpp \
    FondoScroll.cpp \
    GestorEntidades.cpp \
    Helicoptero.cpp \
    HelicopteroEnemigo.cpp \
    Inputs.cpp \
    MenuPrincipal.cpp \
    Misil.cpp \
    MisilEnemigo.cpp \
    MotorJuego.cpp \
    NucleoFisico.cpp \
    ScoreManager.cpp \
    Soldado.cpp \
    VentanaPuntajes.cpp \
    main.cpp

HEADERS += \
    Civil.h \
    Disparable.h \
    Drone.h \
    Edificio.h \
    Entidad.h \
    FondoScroll.h \
    GestorEntidades.h \
    Helicoptero.h \
    HelicopteroEnemigo.h \
    Inputs.h \
    MenuPrincipal.h \
    Misil.h \
    MisilEnemigo.h \
    MotorJuego.h \
    NucleoFisico.h \
    SavedGame.h \
    ScoreEntry.h \
    ScoreManager.h \
    Soldado.h \
    VentanaPuntajes.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
