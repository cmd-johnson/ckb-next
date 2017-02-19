#include "commandhandler.h"

#include "client.h"
#include "keyeffectmanager.h"
#include "keyeffects/fixedcolor.h"
#include "keyeffects/colorgradient.h"

#include <QJsonObject>
#include <QJsonArray>

namespace {

/* Success messages */

QJsonObject SUCCESS(const QString& reqId) {
    return {
        { "id", reqId },
        { "success", true }
    };
}

/* General errors */

QJsonObject ERR_NOT_AN_OBJECT {
    { "success", false },
    { "error", "invalid_format" },
    { "message", "Root element has to be a non-empty JSON object." }
};

QJsonObject ERR_NO_REQ_ID {
    { "success", false },
    { "error", "no_request_id" },
    { "message", "The request is missing an id or it is not a string." }
};

QJsonObject ERR_NO_COMMAND(const QString& reqId) {
    return {
        { "id", reqId },
        { "success", false },
        { "error", "no_command_specified" },
        { "message", "The message does not contain a command." }
    };
}

QJsonObject ERR_INVALID_COMMAND(const QString& reqId) {
    return {
        { "id", reqId },
        { "success", false },
        { "error", "invalid_command" },
        { "message", "The message does not contain a command." }
    };
}

QJsonObject ERR_INVALID_COMMAND(const QString& reqId, const QString& command) {
    return {
        { "id", reqId },
        { "success", false },
        { "error", "invalid_command" },
        { "message", QString("The message does not contain a valid command. Was given '%1'.").arg(command) }
    };
}

QJsonObject ERR_INVALID_PARAMETER(const QString& reqId, const QString& parameter) {
    return {
        { "id", reqId },
        { "success", false },
        { "error", "invalid_parameter" },
        { "message", QString("Encountered an unexpected value for parameter '%1'.").arg(parameter) }
    };
}

}

CommandHandler::CommandHandler(KeyEffectManager *keyEffectManager,
                               QString clientId, QObject *parent)
    : QObject(parent), keyEffectManager(keyEffectManager)
{
    commandHandlers["identify"] = [clientId](const QString& reqId, const QJsonObject&, KeyEffectManager*, Client* sender) {
        sender->sendMessage({
            { "id", reqId },
            { "success", true },
            { "client_id", clientId }
        });
    };

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

    QString reqId = object["id"].toString();
    if (reqId.isEmpty()) {
        client->sendMessage(ERR_NO_REQ_ID);
        return;
    }

    if (!object.contains("command")) {
        client->sendMessage(ERR_NO_COMMAND(reqId));
        return;
    }

    QString command = object["command"].toString();
    if (command.isEmpty()) {
        client->sendMessage(ERR_INVALID_COMMAND(reqId));
        return;
    }

    auto cmd = commandHandlers.find(command);
    if (cmd == commandHandlers.end()) {
        client->sendMessage(ERR_INVALID_COMMAND(reqId, command));
    } else {
        cmd.value()(reqId, object, keyEffectManager, client);
    }
}

void CommandHandler::handleListKeys(const QString& reqId, const QJsonObject &command, KeyEffectManager *keyEffectManager, Client *sender)
{
    Q_UNUSED(command);

    QJsonArray keys;
    for (auto& key : keyEffectManager->getKeys()) {
        keys.append(key);
    }

    sender->sendMessage({
        { "success", true },
        { "id", reqId },
        { "keys", keys }
    });
}

void CommandHandler::handleSetColor(const QString& reqId, const QJsonObject &command, KeyEffectManager *keyEffectManager, Client *sender)
{
    QString key = command["key"].toString();
    if (key.isEmpty() || !keyEffectManager->keyExists(key)) {
        sender->sendMessage(ERR_INVALID_PARAMETER(reqId, "key"));
        return;
    }

    Color color = Color(command["color"].toString());
    if (!color.isValid()) {
        sender->sendMessage(ERR_INVALID_PARAMETER(reqId, "color"));
        return;
    }

    keyEffectManager->setEffect(key, new FixedColor(color));

    sender->sendMessage(SUCCESS(reqId));
}

void CommandHandler::handleSetGradient(const QString& reqId, const QJsonObject &command, KeyEffectManager *keyEffectManager, Client *sender)
{
    QString key = command["key"].toString();
    if (key.isEmpty() || !keyEffectManager->keyExists(key)) {
        sender->sendMessage(ERR_INVALID_PARAMETER(reqId, "key"));
        return;
    }

    if (command.contains("duration") && !command["duration"].isDouble()) {
        sender->sendMessage(ERR_INVALID_PARAMETER(reqId, "duration"));
        return;
    }
    double duration = command["duration"].toDouble(1.0);
    if (duration <= 0.0) {
        sender->sendMessage(ERR_INVALID_PARAMETER(reqId, "duration"));
        return;
    }

    if (command.contains("loop_count") && !command["loop_count"].isDouble()) {
        sender->sendMessage(ERR_INVALID_PARAMETER(reqId, "loop_count"));
        return;
    }
    int loopCount = command["loop_count"].toInt(1);
    if (loopCount < 0) {
        sender->sendMessage(ERR_INVALID_PARAMETER(reqId, "loop_count"));
        return;
    }

    QJsonArray jsonColorStops = command["color_stops"].toArray();
    if (jsonColorStops.isEmpty()) {
        sender->sendMessage(ERR_INVALID_PARAMETER(reqId, "color_stops"));
        return;
    }

    QList<ColorGradient::ColorStop> colorStops;
    for (int i = 0; i < jsonColorStops.size(); i++) {
        QJsonObject stop = jsonColorStops[i].toObject();

        if (stop.isEmpty()) {
            sender->sendMessage(ERR_INVALID_PARAMETER(reqId, QString("color_stops[%1]").arg(i)));
            return;
        }

        if (!stop["position"].isDouble()) {
            sender->sendMessage(ERR_INVALID_PARAMETER(reqId, QString("color_stops[%1].position").arg(i)));
            return;
        }
        double position = stop["position"].toDouble();

        Color color(stop["color"].toString());
        if (!color.isValid()) {
            sender->sendMessage(ERR_INVALID_PARAMETER(reqId, QString("color_stops[%1].color").arg(i)));
            return;
        }

        colorStops.append({ position, color });
    }

    keyEffectManager->setEffect(key, new ColorGradient(colorStops, duration, (uint)loopCount));

    sender->sendMessage(SUCCESS(reqId));
}

void CommandHandler::handleClear(const QString& reqId, const QJsonObject &command, KeyEffectManager *keyEffectManager, Client *sender)
{
    QString key = command["key"].toString();
    if (key.isEmpty() || !keyEffectManager->keyExists(key)) {
        sender->sendMessage(ERR_INVALID_PARAMETER(reqId, "key"));
        return;
    }

    keyEffectManager->clearEffect(key);

    sender->sendMessage(SUCCESS(reqId));
}
