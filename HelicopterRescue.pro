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
    Inputs.cpp \
    MenuPrincipal.cpp \
    MotorJuego.cpp \
    NucleoFisico.cpp \
    ScoreManager.cpp \
    VentanaPuntajes.cpp \
    main.cpp

HEADERS += \
    Civil.h \
    Drone.h \
    Edificio.h \
    Entidad.h \
    FondoScroll.h \
    GestorEntidades.h \
    Helicoptero.h \
    Inputs.h \
    MenuPrincipal.h \
    MotorJuego.h \
    NucleoFisico.h \
    SavedGame.h \
    ScoreEntry.h \
    ScoreManager.h \
    VentanaPuntajes.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
