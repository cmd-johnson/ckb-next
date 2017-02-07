#include "commandhandler.h"

CommandHandler::CommandHandler(KeyEffectManager *keyEffectManager, QObject *parent)
    : QObject(parent), keyEffectManager(keyEffectManager)
{
}

QByteArray CommandHandler::handleCommand(const QString &command)
{
    return (QString("echo: ") + command).toUtf8();
}
