#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <QHash>
#include <QObject>

#include <functional>

class Client;
class KeyEffectManager;

class CommandHandler : public QObject
{
    Q_OBJECT
public:
    CommandHandler(KeyEffectManager* keyEffectManager, QString clientId, QObject *parent = 0);

public slots:
    void onMessageReceived(const QJsonDocument& json);
    void onConnectionEstablished();

private:
    KeyEffectManager* keyEffectManager;
    QString clientId;

    typedef std::function<void(const QJsonObject&, KeyEffectManager*, Client*)> CommandExecutor;

    QHash<QString, CommandExecutor> commandHandlers;

    static void handleListKeys(const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);
    static void handleSetColor(const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);
    static void handleSetGradient(const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);
    static void handleClear(const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);
};

#endif // COMMANDHANDLER_H
