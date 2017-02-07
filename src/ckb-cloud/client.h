#ifndef CLIENT_H
#define CLIENT_H

#include "commandhandler.h"

#include <QLocalSocket>
#include <QObject>

class KeyEffectManager;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(KeyEffectManager* keyEffectManager, QObject *parent = 0);

public slots:
    void start();

private slots:
    void readData();

private:
    QLocalSocket* socket;
    CommandHandler* commandHandler;
};

#endif // CLIENT_H
