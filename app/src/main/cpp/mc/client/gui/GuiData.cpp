//
// Created by TheChuan1503 on 2025/4/26.
//

#include <string>
#include <dlfcn.h>
#include "GuiData.h"

#define TAG "Native/objects/GuiData"

#include "../../../global.h"

void GuiData::displayClientMessage(const std::string &message) {
    displayClientMessage_t displayClientMessage = (displayClientMessage_t) getMCFuncPtr("GuiData::displayClientMessage");
    if (displayClientMessage) {
        displayClientMessage(this, message);
    }
}
void GuiData::showTipMessage(const std::string &message) {
    showTipMessage_t showTipMessage = (showTipMessage_t) getMCFuncPtr("GuiData::showTipMessage");
    if (showTipMessage) {
        showTipMessage(this, message);
    }
}
