#include "bootstrapper.h"

#include "client.h"

#include <QCoreApplication>
#include <QThread>
#include <QTimer>

Bootstrapper::Bootstrapper(KeyEffectManager* keyEffectManager, QObject *parent)
    : QObject(parent), keyEffectManager(keyEffectManager) { }

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

    Client client(keyEffectManager);
    QTimer::singleShot(0, &client, &Client::start);

    app->exec();
}
