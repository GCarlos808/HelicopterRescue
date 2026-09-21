#ifndef DISPARABLE_H
#define DISPARABLE_H

#include <QPointF>

class Disparable {
public:
    virtual ~Disparable() {}
    virtual bool listoParaDisparar() const = 0;
    virtual void reiniciarCooldownDisparo() = 0;
    virtual QPointF origenDisparo() const = 0;
};

#endif // DISPARABLE_H
