#ifndef KEYEFFECTMANAGER_H
#define KEYEFFECTMANAGER_H

#define CKB_NO_MAIN
#include "../ckb/ckb-anim.h"

#include "keyeffects/keyeffect.h"

#include <QHash>
#include <QSet>
#include <QSharedPointer>
#include <QString>


class KeyEffectManager
{
public:
    KeyEffectManager(ckb_runctx* context);

    void setEffect(const QString& key, KeyEffect* effect);
    void setEffect(const QString& key, QSharedPointer<KeyEffect> effect);
    void clearEffect(const QString& key);

    void advance(double deltaT);
    int getFrame(ckb_runctx* context) const;

    bool keyExists(const QString& key) const;

private:
    QSet<QString> keys;
    QHash<QString, QSharedPointer<KeyEffect>> keyEffects;
};

#endif // KEYEFFECTMANAGER_H
