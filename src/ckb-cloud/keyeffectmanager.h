#ifndef KEYEFFECTMANAGER_H
#define KEYEFFECTMANAGER_H

#define CKB_NO_MAIN
#include "../ckb/ckb-anim.h"

#include "keyeffects/keyeffect.h"

#include <QHash>
#include <QLinkedList>
#include <QSet>
#include <QSharedPointer>
#include <QString>


class KeyEffectManager
{
public:
    KeyEffectManager(ckb_runctx* context);

    bool addEffect(const QString& key, KeyEffect* effect);
    bool addEffect(const QString& key, QSharedPointer<KeyEffect> effect);

    bool clearEffect(const QString& key, const QUuid& uuid);
    void clearAllEffects(const QString& key);

    const QLinkedList<QSharedPointer<KeyEffect>> getEffects(const QString& key) const;
    const QSharedPointer<KeyEffect> getEffect(const QString& key, const QUuid& uuid) const;

    void advance(double deltaT);
    int getFrame(ckb_runctx* context) const;

    bool keyExists(const QString& key) const;
    const QSet<QString>& getKeys() const;

private:
    QSet<QString> keys;
    QHash<QString, QLinkedList<QSharedPointer<KeyEffect>>> keyEffects;
};

#endif // KEYEFFECTMANAGER_H
