#include "../ckb/ckb-anim.h"

#include "bootstrapper.h"
#include "config.h"
#include "keyeffectmanager.h"

#include <QUuid>

void ckb_info()
{
    // Plugin info
    CKB_NAME("ckb-cloud client");
    CKB_VERSION("0.1.0");
    CKB_COPYRIGHT("2017", "Jonas Auer");
    CKB_LICENSE("MIT");
    CKB_GUID("{43455eeb-d5b1-4a97-9f82-7e4b8ecb8609}");
    CKB_DESCRIPTION("Client plugin used to connect ckb to the outside world.");

    // Client parameters
    CKB_PARAM_STRING("socket", "Socket Path:", "", "/tmp/ckb-cloud");
    CKB_PARAM_STRING("clientid", "Client ID:", "", "");

    // Presets
    CKB_PRESET_START("ckb-cloud");
    CKB_PRESET_PARAM("socket", "/tmp/ckb-cloud");
    QString uuid = QUuid::createUuid().toString();
    CKB_PRESET_PARAM("clientid", uuid.toStdString().c_str());
    CKB_PRESET_END;
}

Config* config;
KeyEffectManager* keyEffectManager;

void ckb_init(ckb_runctx* context)
{
    config = new Config();
    keyEffectManager = new KeyEffectManager(context);

    Bootstrapper* bootstrapper = new Bootstrapper(keyEffectManager, config);
    bootstrapper->start();
}

void ckb_parameter(ckb_runctx* context, const char* name, const char* value)
{
    Q_UNUSED(context);
    Q_UNUSED(value);

    CKB_PARSE_STRING("clientid") {
        config->clientId = value;
    }
    CKB_PARSE_STRING("socket") {
        config->socketPath = value;
    }
}

void ckb_keypress(ckb_runctx* context, ckb_key* key, int x, int y, int state)
{
    // We don't send keypresses over the socket. We wouldn't want someone to use
    // this plugin as a key-logger, would we?
    Q_UNUSED(context);
    Q_UNUSED(key);
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(state);
}

void ckb_start(ckb_runctx* context, int state)
{
    // This function doesn't apply for this plugin either.
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
