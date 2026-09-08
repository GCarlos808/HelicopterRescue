#ifndef MOTORJUEGO_H
#define MOTORJUEGO_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

class MotorJuego : public QObject {
    Q_OBJECT

public:
    explicit MotorJuego(QObject *parent = nullptr);

    void iniciar();
    void detener();

private slots:
    void tick();

private:
    QTimer timer;
    QElapsedTimer cronometro;

signals:
    void tickFisica(qreal deltaTime);
};

#endif // MOTORJUEGO_H