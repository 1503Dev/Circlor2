//
// Created by TheChuan1503 on 2025/4/26.
//

#include <cstddef>
#include <string>
#include "Circlor.h"

#define TAG "Native/Circlor"
#include "global.h"

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

std::string Circlor::invoke(std::string funcPath) {
    if (funcPath == "exp_level_change") {
        if (isInGame() && clientInstance->getLocalPlayer()){
            clientInstance->getLocalPlayer()->addLevels((int)Circlor::getFunctionValue("exp_level_change_value"));
        } else {
            return "not_in_game";
        }
    } else if (funcPath == "gamemode_change/s") {
        if (isInGame() && clientInstance->getLocalPlayer()){
            clientInstance->getLocalPlayer()->setPlayerGameType(GameType::Survival);
        } else {
            return "not_in_game";
        }
    } else if (funcPath == "gamemode_change/c") {
        if (isInGame() && clientInstance->getLocalPlayer()){
            clientInstance->getLocalPlayer()->setPlayerGameType(GameType::Creative);
        } else {
            return "not_in_game";
        }
    } else if (funcPath == "gamemode_change/a") {
        if (isInGame() && clientInstance->getLocalPlayer()){
            clientInstance->getLocalPlayer()->setPlayerGameType(GameType::Adventure);
        } else {
            return "not_in_game";
        }
    }



    else if (funcPath == "testcall") {
//        if (mc_gameMode) {
//            mc_gameMode->destroyBlock(BlockPos(0, 100, 0), mc_gameMode_destroyBlock_flag);
//        }
        return "testcalled";
    }
    return "";
}
