//
// Created by TheChuan1503 on 2025/7/17.
//

#include "MobEffectInstance.h"

#ifndef GLOBAL_H
#define TAG "Native/MobEffectInstance"
#include "../../../global.h"
#endif

MobEffectInstance::MobEffectInstance(unsigned int effectId, int durationTicks, int level, bool b1, bool effectVisible, bool b2) {
    MobEffectInstance_t mobEffectInstance = (MobEffectInstance_t) getMCFuncPtr("MobEffectInstance::MobEffectInstance");
    if (mobEffectInstance) {
        mobEffectInstance(this, effectId, durationTicks, level, b1, effectVisible, b2);
    }
}
