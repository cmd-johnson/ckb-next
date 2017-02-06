#include "keyeffectmanager.h"

#include "keyeffects/fixedcolor.h"


KeyEffectManager::KeyEffectManager(ckb_runctx *context)
{
    for (uint i = 0; i < context->keycount; ++i) {
        ckb_key key = context->keys[i];
        keys.insert(key.name);
    }
}

void KeyEffectManager::setEffect(const QString &key, KeyEffect *effect)
{
    setEffect(key, QSharedPointer<KeyEffect>(effect));
}

void KeyEffectManager::setEffect(const QString &key, QSharedPointer<KeyEffect> effect)
{
    keyEffects[key] = effect;
}

void KeyEffectManager::clearEffect(const QString &key)
{
    keyEffects.remove(key);
}

void KeyEffectManager::advance(double deltaT)
{
    auto it = keyEffects.begin();
    while (it != keyEffects.end()) {
        if (!(*it)->advance(deltaT)) {
            it = keyEffects.erase(it);
        } else {
            ++it;
        }
    }
}

int KeyEffectManager::getFrame(ckb_runctx *context) const
{
    for (uint i = 0; i < context->keycount; ++i) {
        ckb_key& key = context->keys[i];
        auto keyIt = keyEffects.find(key.name);

        QColor color;
        if (keyIt != keyEffects.end()) {
            color = (*keyIt)->getColor();
        } else {
            color = Qt::transparent;
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
