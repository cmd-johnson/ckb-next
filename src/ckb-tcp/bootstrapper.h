#ifndef BOOTSTRAPPER_H
#define BOOTSTRAPPER_H

#include "keyeffectmanager.h"

#include <QObject>

class Bootstrapper : public QObject
{
    Q_OBJECT
public:
    Bootstrapper(KeyEffectManager* keyEffectManager, QObject *parent = 0);

    // Initiates startup of the Qt event loop on a new thread.
    void start();

public slots:
    void startApplication();

private:
    KeyEffectManager* keyEffectManager;
};

#endif // BOOTSTRAPPER_H
