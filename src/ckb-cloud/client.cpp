#include "client.h"

#include <QJsonObject>
#include <QTimer>
#include <QFile>

#define SOCKET_RECONNECTION_TIMEOUT 1000

Client::Client(QObject *parent)
    : QObject(parent), socket(new QLocalSocket(this)),
      socketPath("/tmp/ckb-cloud")
{
    connect(socket, &QLocalSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QLocalSocket::connected, this, &Client::onConnected);

    auto error = static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error);
    connect(socket, error, this, &Client::onError);

    connect(socket, &QLocalSocket::disconnected, this, &Client::connectionClosed);
}


QString Client::getSocketPath() const
{
    return socketPath;
}

void Client::setSocketPath(const QString &value)
{
    // If the path changed, close the current connection and open a new one.
    if (socketPath == value) {
        return;
    }
    socketPath = value;
    socket->close();
    socket->waitForDisconnected();
    openConnection();
}

void Client::sendMessage(const QJsonArray &json)
{
    sendMessage(QJsonDocument(json));
}

void Client::sendMessage(const QJsonObject &json)
{
    sendMessage(QJsonDocument(json));
}

void Client::sendMessage(const QJsonDocument& json)
{
    // Discard messages when no connection is open
    if (!socket->isOpen()) {
        return;
    }

    socket->write(">>>");
    socket->write(json.toJson(QJsonDocument::Compact));
    socket->write("\n");
}


void Client::openConnection()
{
    if (socket->isOpen()) {
        return;
    }
    socket->connectToServer(getSocketPath());
}

void Client::closeConnection()
{
    if (!socket->isOpen()) {
        return;
    }
    socket->disconnectFromServer();
}


void Client::onReadyRead()
{
    while (socket->canReadLine()) {
        QJsonParseError error;
        QJsonDocument message = QJsonDocument::fromJson(socket->readLine(), &error);
        if (error.error != QJsonParseError::NoError) {
            sendMessage(QJsonObject {
                            { "error", "invalid JSON" },
                            { "code", error.error },
                            { "message", error.errorString() }
                        });
        }

        emit messageReceived(message);
    }
}

void Client::onConnected()
{
    emit connectionEstablished();
}

void Client::onError(QLocalSocket::LocalSocketError socketError)
{
    Q_UNUSED(socketError);

    if (socket->isOpen()) {
        socket->close();
    }
    QTimer::singleShot(SOCKET_RECONNECTION_TIMEOUT, this, &Client::openConnection);
}
