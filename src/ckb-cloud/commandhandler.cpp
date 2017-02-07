#include "commandhandler.h"

#include "client.h"
#include "keyeffectmanager.h"
#include "keyeffects/fixedcolor.h"
#include "keyeffects/colorgradient.h"

#include <QJsonObject>
#include <QJsonArray>

namespace {

/* General errors */

QJsonObject ERR_NOT_AN_OBJECT {
    { "error", "invalid_format" },
    { "message", "Root element has to be a non-empty JSON object." }
};

QJsonObject ERR_NO_COMMAND {
    { "error", "no_command_specified" },
    { "message", "The message does not contain a command." }
};

QJsonObject ERR_INVALID_COMMAND {
    { "error", "invalid_command" },
    { "message", "The message does not contain a valid command." }
};

QJsonObject ERR_INVALID_PARAMETER(const QString& parameter) {
    return QJsonObject {
        { "error", "invalid_parameter" },
        { "message", QString("Encountered an unexpected value for parameter '%1'.").arg(parameter) }
    };
}

}

CommandHandler::CommandHandler(KeyEffectManager *keyEffectManager,
                               QString clientId, QObject *parent)
    : QObject(parent), keyEffectManager(keyEffectManager),
      clientId(clientId)
{
    commandHandlers["list_keys"] = &CommandHandler::handleListKeys;
    commandHandlers["set_key_color"] = &CommandHandler::handleSetColor;
    commandHandlers["set_key_gradient"] = &CommandHandler::handleSetGradient;
    commandHandlers["clear_key"] = &CommandHandler::handleClear;
}

void CommandHandler::onMessageReceived(const QJsonDocument &json)
{
    Client* client = qobject_cast<Client*>(sender());
    if (!client) return;

    QJsonObject object = json.object();
    if (object.isEmpty()) {
        client->sendMessage(ERR_NOT_AN_OBJECT);
        return;
    }

    if (!object.contains("command")) {
        client->sendMessage(ERR_NO_COMMAND);
        return;
    }

    QString command = object["command"].toString();
    if (command.isEmpty()) {
        client->sendMessage(ERR_INVALID_COMMAND);
        return;
    }

    auto cmd = commandHandlers.find(command);
    if (cmd== commandHandlers.end()) {
        client->sendMessage(ERR_INVALID_COMMAND);
    } else {
        cmd.value()(object, keyEffectManager, client);
    }
}

void CommandHandler::onConnectionEstablished()
{
    Client* client = qobject_cast<Client*>(sender());
    if (!client) return;

    client->sendMessage(QJsonObject
    {
        { "register_client", clientId }
    });
}

void CommandHandler::handleListKeys(const QJsonObject &command, KeyEffectManager *keyEffectManager, Client *sender)
{
    Q_UNUSED(command);

    QJsonArray keys;
    for (auto& key : keyEffectManager->getKeys()) {
        keys.append(key);
    }
    sender->sendMessage(keys);
}

void CommandHandler::handleSetColor(const QJsonObject &command, KeyEffectManager *keyEffectManager, Client *sender)
{
    QString key = command["key"].toString();
    if (key.isEmpty() || !keyEffectManager->keyExists(key)) {
        sender->sendMessage(ERR_INVALID_PARAMETER("key"));
        return;
    }

    QColor color = QColor(command["color"].toString());
    if (!color.isValid()) {
        sender->sendMessage(ERR_INVALID_PARAMETER("color"));
        return;
    }

    keyEffectManager->setEffect(key, new FixedColor(color));
    sender->sendMessage(command);
}

void CommandHandler::handleSetGradient(const QJsonObject &command, KeyEffectManager *keyEffectManager, Client *sender)
{
    QString key = command["key"].toString();
    if (key.isEmpty() || !keyEffectManager->keyExists(key)) {
        sender->sendMessage(ERR_INVALID_PARAMETER("key"));
        return;
    }

    if (command.contains("duration") && !command["duration"].isDouble()) {
        sender->sendMessage(ERR_INVALID_PARAMETER("duration"));
        return;
    }
    double duration = command["duration"].toDouble(1.0);
    if (duration <= 0.0) {
        sender->sendMessage(ERR_INVALID_PARAMETER("duration"));
        return;
    }

    if (command.contains("loop_count") && !command["loop_count"].isDouble()) {
        sender->sendMessage(ERR_INVALID_PARAMETER("loop_count"));
        return;
    }
    int loopCount = command["loop_count"].toInt(1);
    if (loopCount < 0) {
        sender->sendMessage(ERR_INVALID_PARAMETER("loop_count"));
        return;
    }

    QJsonArray jsonColorStops = command["color_stops"].toArray();
    if (jsonColorStops.isEmpty()) {
        sender->sendMessage(ERR_INVALID_PARAMETER("color_stops"));
        return;
    }

    QList<ColorGradient::ColorStop> colorStops;
    for (int i = 0; i < jsonColorStops.size(); i++) {
        QJsonObject stop = jsonColorStops[i].toObject();

        if (stop.isEmpty()) {
            sender->sendMessage(ERR_INVALID_PARAMETER(QString("color_stops[%1]").arg(i)));
            return;
        }

        if (!stop["position"].isDouble()) {
            sender->sendMessage(ERR_INVALID_PARAMETER(QString("color_stops[%1].position").arg(i)));
            return;
        }
        double position = stop["position"].toDouble();

        QColor color(stop["color"].toString());
        if (!color.isValid()) {
            sender->sendMessage(ERR_INVALID_PARAMETER(QString("color_stops[%1].color").arg(i)));
            return;
        }

        colorStops.append({ position, color });
    }

    keyEffectManager->setEffect(key, new ColorGradient(colorStops, duration, (uint)loopCount));
    sender->sendMessage(command);
    sender->sendMessage(QJsonObject {{"loop_count", QString::number(loopCount)}});
}

void CommandHandler::handleClear(const QJsonObject &command, KeyEffectManager *keyEffectManager, Client *sender)
{
    QString key = command["key"].toString();
    if (key.isEmpty() || !keyEffectManager->keyExists(key)) {
        sender->sendMessage(ERR_INVALID_PARAMETER("key"));
        return;
    }

    keyEffectManager->clearEffect(key);
    sender->sendMessage(command);
}
