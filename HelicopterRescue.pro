QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Edificio.cpp \
    Entidad.cpp \
    FondoScroll.cpp \
    GestorEntidades.cpp \
    Helicoptero.cpp \
    Inputs.cpp \
    MotorJuego.cpp \
    NucleoFisico.cpp \
    main.cpp

HEADERS += \
    Edificio.h \
    Entidad.h \
    FondoScroll.h \
    GestorEntidades.h \
    Helicoptero.h \
    Inputs.h \
    MotorJuego.h \
    NucleoFisico.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
