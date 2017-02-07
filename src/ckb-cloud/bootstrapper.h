#ifndef BOOTSTRAPPER_H
#define BOOTSTRAPPER_H

#include <QObject>

class KeyEffectManager;
class Config;

class Bootstrapper : public QObject
{
    Q_OBJECT
public:
    Bootstrapper(KeyEffectManager* keyEffectManager, Config* config, QObject *parent = 0);

    // Initiates startup of the Qt event loop on a new thread.
    void start();

public slots:
    void startApplication();

private:
    KeyEffectManager* keyEffectManager;
    Config* config;
};

#endif // BOOTSTRAPPER_H
