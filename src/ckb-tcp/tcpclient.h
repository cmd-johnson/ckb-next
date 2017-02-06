#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "keyeffectmanager.h"

#include <QHash>
#include <QObject>
#include <QTcpSocket>

#include <functional>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    TcpClient(KeyEffectManager* keyEffectManager, QObject *parent = 0);

public slots:
    void start();

private slots:
    void readTcpData();

private:
    KeyEffectManager* keyEffectManager;
    QTcpSocket* socket;

    typedef std::function<void(QTcpSocket*, const QString&, QStringList&)> CommandHandler;
    QHash<QString, CommandHandler> commandHandlers;

    void handleCommand(QTcpSocket* connection, const QString& command);

    void handleSetColor(QTcpSocket* connection, const QString& key, QStringList& parameters);
    void handleClear(QTcpSocket* connection, const QString& key, QStringList& parameters);
};

#endif // TCPCLIENT_H
