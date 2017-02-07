#include "../ckb/ckb-anim.h"

#include "bootstrapper.h"
#include "keyeffectmanager.h"


void ckb_info()
{
    // Plugin info
    CKB_NAME("TCP Client");
    CKB_VERSION("0.0");
    CKB_COPYRIGHT("2017", "Jonas Auer");
    CKB_LICENSE("MIT");
    CKB_GUID("{43455eeb-d5b1-4a97-9f82-7e4b8ecb8609}");
    CKB_DESCRIPTION("TCP interface to connect ckb to the outside world.");

    // Presets
    CKB_PRESET_START("Local Only");
    CKB_PRESET_END;
}

KeyEffectManager* keyEffectManager;

void ckb_init(ckb_runctx* context)
{
    if (!keyEffectManager) {
        keyEffectManager = new KeyEffectManager(context);
    }

    Bootstrapper* bootstrapper = new Bootstrapper(keyEffectManager);
    bootstrapper->start();
}

void ckb_parameter(ckb_runctx* context, const char* name, const char* value)
{
    Q_UNUSED(context);
    Q_UNUSED(name);
    Q_UNUSED(value);
}

void ckb_keypress(ckb_runctx* context, ckb_key* key, int x, int y, int state)
{
    Q_UNUSED(context);
    Q_UNUSED(key);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(state);
}

void ckb_start(ckb_runctx* context, int state)
{
    Q_UNUSED(context);
    Q_UNUSED(state);
}

void ckb_time(ckb_runctx* context, double deltaT)
{
    Q_UNUSED(context);

    if (keyEffectManager) {
        keyEffectManager->advance(deltaT);
    }
}

int ckb_frame(ckb_runctx* context)
{
    return keyEffectManager ? keyEffectManager->getFrame(context) : 0;
}
