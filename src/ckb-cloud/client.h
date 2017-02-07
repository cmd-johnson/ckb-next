#ifndef CLIENT_H
#define CLIENT_H

#include "commandhandler.h"

#include <QLocalSocket>
#include <QJsonDocument>
#include <QObject>

class KeyEffectManager;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(KeyEffectManager* keyEffectManager, QObject *parent = 0);

    QString getSocketPath() const;
    void setSocketPath(const QString &value);

    void sendMessage(const QJsonArray& json);
    void sendMessage(const QJsonObject& json);
    void sendMessage(const QJsonDocument& json);

signals:
    void messageReceived(const QJsonDocument& json);
    void connectionEstablished();
    void connectionClosed();

public slots:
    void openConnection();

private slots:
    void onReadyRead();
    void onConnected();
    void onError(QLocalSocket::LocalSocketError socketError);

private:
    QLocalSocket* socket;
    CommandHandler* commandHandler;

    QString socketPath;

    void closeConnection();
};

#endif // CLIENT_H
