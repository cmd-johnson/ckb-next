#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "keyeffectmanager.h"

#include <QObject>

class CommandHandler : public QObject
{
    Q_OBJECT
public:
    explicit CommandHandler(KeyEffectManager* keyEffectManager, QObject *parent = 0);

    QByteArray handleCommand(const QString& command);

private:
    KeyEffectManager* keyEffectManager;
};

#endif // COMMANDHANDLER_H
