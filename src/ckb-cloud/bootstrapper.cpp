#include "bootstrapper.h"

#include "client.h"
#include "commandhandler.h"
#include "config.h"

#include <QCoreApplication>
#include <QThread>
#include <QTimer>

Bootstrapper::Bootstrapper(KeyEffectManager* keyEffectManager, Config* config, QObject *parent)
    : QObject(parent), keyEffectManager(keyEffectManager), config(config) { }

void Bootstrapper::start()
{
    QThread* thread = new QThread();

    connect(thread, &QThread::started, this, &Bootstrapper::startApplication, Qt::DirectConnection);
    thread->start();
}

void Bootstrapper::startApplication()
{
    int argc = 0;
    char* argv[] = { };

    QCoreApplication* app = new QCoreApplication(argc, argv);

    Client client(config->socketPath);
    CommandHandler commandHandler(keyEffectManager, config->clientId);

    connect(&client, &Client::messageReceived,
            &commandHandler, &CommandHandler::onMessageReceived);

    QTimer::singleShot(0, &client, &Client::openConnection);

    app->exec();
}
