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

private:
    KeyEffectManager* keyEffectManager;

    typedef std::function<void(const QString&, const QJsonObject&, KeyEffectManager*, Client*)> CommandExecutor;

    QHash<QString, CommandExecutor> commandHandlers;

    static void handleListKeys(const QString& reqId, const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);
    static void handleGetKey(const QString& reqId, const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);

    static void handleListEffects(const QString& reqId, const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);
    static void handleGetEffect(const QString& reqId, const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);

    static void handleAddColor(const QString& reqId, const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);
    static void handleAddGradient(const QString& reqId, const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);

    static void handleClearAllEffects(const QString& reqId, const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);
    static void handleClearEffect(const QString& reqId, const QJsonObject& command, KeyEffectManager* keyEffectManager, Client* sender);
};

#endif // COMMANDHANDLER_H
