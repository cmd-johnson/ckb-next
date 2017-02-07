#ifndef CLIENT_H
#define CLIENT_H

#include <QLocalSocket>
#include <QJsonDocument>
#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

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

    QString socketPath;

    void closeConnection();
};

#endif // CLIENT_H
