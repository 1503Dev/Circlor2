//
// Created by TheChuan1503 on 2025/4/26.
//

#include <cstddef>
#include <string>
#include "Circlor.h"

#define TAG "Native/Circlor"
#include "global.h"
#include "mc/gamemode/GameMode.h"

double Circlor::getFunctionValue(const char *path) {
    if (circlor2FunctionsSave.find(path) != circlor2FunctionsSave.end()) {
        return circlor2FunctionsSave[path];
    }
    return 0;
}
bool Circlor::getFunctionBoolValue(const char *path) {
    return getFunctionValue(path) != 0;
}
std::string Circlor::getFunctionStringValue(const char *path) {
    if (circlor2FunctionsStringSave.find(path)!= circlor2FunctionsStringSave.end()) {
        return circlor2FunctionsStringSave[path];
    }
    return "";
}
void Circlor::setFunctionValue(const char *path, double value) {
    circlor2FunctionsSave[path] = value;
    LOGD("Set %s to %f", path, value);
}
void Circlor::setFunctionStringValue(const char *path, const char *value) {
    circlor2FunctionsStringSave[path] = value;
    LOGD("Set %s to %s", path, value);
}

double Circlor::getValue(const char *path) {
    return getFunctionValue(path);
}
void Circlor::setValue(const char *path, double value) {
    setFunctionValue(path, value);
}
std::string Circlor::getString(const char *path) {
    return getFunctionStringValue(path);
}
void Circlor::setString(const char *path, const char *value) {
    setFunctionStringValue(path, value);
}
bool Circlor::getBool(const char *path) {
    return getFunctionBoolValue(path);
}
void Circlor::setBool(const char *path, bool value) {
    setFunctionValue(path, value);
}

std::string changeGameMode(GameType gameType, bool bypass) {
    if (isInGame() && clientInstance->getLocalPlayer()) {
        if (bypass) {
            clientInstance->getLocalPlayer()->setPlayerGameTypeWithoutServerNotification(gameType);
        } else {
            clientInstance->getLocalPlayer()->setPlayerGameType(gameType);
        }
        return "";
    } else {
        return "not_in_game";
    }
}
bool Circlor::addEffect(unsigned int effectId, int durationTicks, int level, bool effectVisible) {
    if (effectId >= 1 && effectId <= 29 && durationTicks >= 1 && level >= 0 && level <= 255) {
        if (isInGame() && clientInstance->getLocalPlayer()) {
            MobEffectInstance mei = MobEffectInstance(effectId, durationTicks, level, false, effectVisible, false);
            clientInstance->getLocalPlayer()->addEffect(&mei);
            return true;
        }
    }
    return false;
}
bool Circlor::addEffect(Actor *actor, unsigned int effectId, int durationTicks, int level, bool effectVisible) {
    if (effectId >= 1 && effectId <= 29 && durationTicks >= 1 && level >= 0 && level <= 255) {
        MobEffectInstance mei = MobEffectInstance(effectId, durationTicks, level, false, effectVisible, false);
        actor->addEffect(&mei);
        return true;
    }
    return false;
}

bool onTicked = false;
void Circlor::onDoubleTick() {
    if (isInGame() && clientInstance->getLocalPlayer()) {
        LocalPlayer *lp = clientInstance->getLocalPlayer();
        for (unsigned int i = 2; i <= 29; ++i) {
            std::string path = "effect/" + std::to_string(i) + "/everlasting";
            int level = (int) Circlor::getValue(path.c_str());
            level = level - 1;
            int duration = 9;
            if (i == 16) {
                duration = 204;
            } else if (i == 15 || i == 9) {
                duration = 64;
            }
            addEffect(lp, i, duration, level, false);
        }
    }
}
void Circlor::onTick() {
    if (onTicked) {
        onTicked = false;
        return;
    }
    onTicked = true;
    onDoubleTick();
}

std::string Circlor::invoke(std::string funcPath) {
    bool isChangeGameMode = false;
    GameType gameModeChangedTo = GameType::Creative;

    if (funcPath == "exp_level_change") {
        if (isInGame() && clientInstance->getLocalPlayer()){
            clientInstance->getLocalPlayer()->addLevels((int)Circlor::getFunctionValue("exp_level_change_value"));
        } else {
            return "not_in_game";
        }
    } else if (funcPath == "gamemode_change/s") {
        isChangeGameMode = true;
        gameModeChangedTo = GameType::Survival;
    } else if (funcPath == "gamemode_change/c") {
        isChangeGameMode = true;
        gameModeChangedTo = GameType::Creative;
    } else if (funcPath == "gamemode_change/a") {
        isChangeGameMode = true;
        gameModeChangedTo = GameType::Adventure;
    }

    if (isChangeGameMode) {
        return changeGameMode(gameModeChangedTo, Circlor::getBool("gamemode_change/bypass"));
    }

    return "";
}
