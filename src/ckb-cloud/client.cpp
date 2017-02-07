#include "client.h"

Client::Client(KeyEffectManager *keyEffectManager, QObject *parent)
    : QObject(parent), socket(new QLocalSocket(this)),
      commandHandler(new CommandHandler(keyEffectManager, this))
{
}

void Client::start()
{
    connect(socket, &QLocalSocket::readyRead, this, &Client::readData);

    socket->connectToServer("/tmp/ckb-cloud");
    if (socket->waitForConnected()) {
        socket->write("Hello from ckb-cloud");
    }
}

void Client::readData()
{
    while (socket->canReadLine()) {
        QByteArray response = commandHandler->handleCommand(socket->readLine());
        socket->write(response);
    }
}
