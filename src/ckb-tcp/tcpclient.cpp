#include "tcpclient.h"

#include "keyeffects/fixedcolor.h"

#include <QFile>
#include <QHostAddress>

TcpClient::TcpClient(KeyEffectManager* keyEffectManager, QObject *parent)
    : QObject(parent),
      keyEffectManager(keyEffectManager),
      socket(new QTcpSocket(this))
{
    using namespace std::placeholders;
    commandHandlers["set_color"] = std::bind(&TcpClient::handleSetColor, this, _1, _2, _3);
    commandHandlers["clear"] = std::bind(&TcpClient::handleClear, this, _1, _2, _3);
}

void TcpClient::start()
{
    connect(socket, &QTcpSocket::readyRead, this, &TcpClient::readTcpData);

    socket->connectToHost(QHostAddress::LocalHost, 3007);
    if (socket->waitForConnected())
    {
        socket->write("Hello, World\n");
    }
}

void TcpClient::readTcpData()
{
    QTcpSocket* connection = qobject_cast<QTcpSocket*>(sender());

    while(connection->canReadLine())
    {
        handleCommand(connection, connection->readLine());
    }
}

#define WRITE_ERR(type) connection->write("ERR " type "\n"); return
#define WRITE_ACK connection->write("ACK\n"); return
#define POP_NEXT_PARAM(NAME, PARAMETERS, ERROR_MSG) \
    if (PARAMETERS.size() < 1) { WRITE_ERR(ERROR_MSG); } \
    QString NAME = PARAMETERS.first(); \
    PARAMETERS.removeFirst()

void TcpClient::handleCommand(QTcpSocket* connection, const QString &command)
{
    QStringList elements = command.simplified().split(' ');
    POP_NEXT_PARAM(cmd, elements, "no_command");

    auto commandHandler = commandHandlers.find(cmd);
    if (cmd == "get_keys") {
        handleGetKeys(connection);
        return;
    } else if (commandHandler == commandHandlers.end()) {
        WRITE_ERR("invalid_command");
    }

    POP_NEXT_PARAM(key, elements, "no_key");
    if (!keyEffectManager->keyExists(key)) {
        WRITE_ERR("invalid_key");
    }

    // Call the command handler.
    (*commandHandler)(connection, key, elements);
}

void TcpClient::handleSetColor(QTcpSocket* connection, const QString& key, QStringList &parameters)
{
    if (parameters.size() > 1) {
        WRITE_ERR("too_many_arguments");
    }

    POP_NEXT_PARAM(colorString, parameters, "no_color");

    QColor color(colorString);
    if (!color.isValid()) {
        WRITE_ERR("invalid_color");
    }

    keyEffectManager->setEffect(key, new FixedColor(color));

    WRITE_ACK;
}

void TcpClient::handleClear(QTcpSocket* connection, const QString& key, QStringList &parameters)
{
    if (parameters.size() != 0) {
        WRITE_ERR("too_many_arguments");
    }

    keyEffectManager->clearEffect(key);

    WRITE_ACK;
}

void TcpClient::handleGetKeys(QTcpSocket *connection)
{
    QStringList keys = keyEffectManager->getKeys().toList();
    std::sort(keys.begin(), keys.end());
    connection->write("KEYS ");
    connection->write(keys.join(',').toUtf8());
    connection->write("\n");
}
