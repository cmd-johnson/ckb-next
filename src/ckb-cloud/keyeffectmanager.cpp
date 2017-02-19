#include "keyeffectmanager.h"

#include "keyeffects/fixedcolor.h"

#define MAX_KEY_EFFECTS 10

KeyEffectManager::KeyEffectManager(ckb_runctx *context)
{
    for (uint i = 0; i < context->keycount; ++i) {
        ckb_key key = context->keys[i];
        keys.insert(key.name);
    }
}

bool KeyEffectManager::addEffect(const QString &key, KeyEffect *effect)
{
    return addEffect(key, QSharedPointer<KeyEffect>(effect));
}

bool KeyEffectManager::addEffect(const QString &key, QSharedPointer<KeyEffect> effect)
{
    if (keyEffects[key].count() >= MAX_KEY_EFFECTS) {
        return false;
    }

    // Make sure every effect can only be added once.
    for (const auto& otherEffect : keyEffects[key]) {
        if (otherEffect->getUuid() == effect->getUuid()) {
            return false;
        }
    }

    keyEffects[key].append(effect);
    return true;
}

bool KeyEffectManager::clearEffect(const QString &key, const QUuid &uuid)
{
    if (!keyEffects.contains(key)) {
        return false;
    }
    bool removed = false;
    auto effectIt = keyEffects[key].begin();
    while (effectIt != keyEffects[key].end()) {
        if ((*effectIt)->getUuid() == uuid) {
            keyEffects[key].erase(effectIt);
            removed = true;
            break;
        } else {
            ++effectIt;
        }
    }

    return removed;
}

void KeyEffectManager::clearAllEffects(const QString &key)
{
    keyEffects.remove(key);
}

const QLinkedList<QSharedPointer<KeyEffect>> KeyEffectManager::getEffects(const QString &key) const
{
    return keyEffects[key];
}

const QSharedPointer<KeyEffect> KeyEffectManager::getEffect(const QString &key, const QUuid &uuid) const
{
    auto keyIt = keyEffects.find(key);
    if (keyIt != keyEffects.end()) {
        auto effectIt = keyIt->begin();
        while (effectIt != keyIt->end()) {
            if ((*effectIt)->getUuid() == uuid) {
                return *effectIt;
            } else {
                ++effectIt;
            }
        }
    }

    return QSharedPointer<KeyEffect>();
}

void KeyEffectManager::advance(double deltaT)
{
    auto keyIt = keyEffects.begin();
    while (keyIt != keyEffects.end()) {
        QLinkedList<QSharedPointer<KeyEffect>> effectList = *keyIt;

        auto effectIt = effectList.begin();
        while (effectIt != effectList.end()) {
            if (!(*effectIt)->advance(deltaT)) {
                effectIt = effectList.erase(effectIt);
            } else {
                ++effectIt;
            }
        }

        if (effectList.isEmpty()) {
            keyIt = keyEffects.erase(keyIt);
        } else {
            ++keyIt;
        }
    }
}

int KeyEffectManager::getFrame(ckb_runctx *context) const
{
    for (uint i = 0; i < context->keycount; ++i) {
        ckb_key& key = context->keys[i];
        auto keyIt = keyEffects.find(key.name);

        Color color(0, 0, 0, 0);
        if (keyIt != keyEffects.end()) {
            for (const auto& effect : *keyIt) {
                color = effect->getColor().blend(color);
            }
        }

        key.r = color.red();
        key.g = color.green();
        key.b = color.blue();
        key.a = color.alpha();
    }

    return 0;
}

bool KeyEffectManager::keyExists(const QString &key) const
{
    return keys.contains(key);
}

const QSet<QString>& KeyEffectManager::getKeys() const
{
    return keys;
}
