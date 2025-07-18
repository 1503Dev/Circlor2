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
void GuiData::setGuiScale(float scale) {
    setGuiScale_t setGuiScale = (setGuiScale_t) getMCFuncPtr("GuiData::setGuiScale");
    if (setGuiScale) {
        setGuiScale(this, scale);
    }
}
void GuiData::setSubtitle(const std::string &message) {
    setSubtitle_t setSubtitle = (setSubtitle_t) getMCFuncPtr("GuiData::setSubtitle");
    if (setSubtitle) {
        setSubtitle(this, message);
    }
}

void GuiData::setTitle(const std::string &message) {
    setTitle_t setTitle = (setTitle_t) getMCFuncPtr("GuiData::setTitle");
    if (setTitle) {
        setTitle(this, message);
    }
}

void GuiData::showTipMessage(const std::string &message) {
    showTipMessage_t showTipMessage = (showTipMessage_t) getMCFuncPtr("GuiData::showTipMessage");
    if (showTipMessage) {
        showTipMessage(this, message);
    }
}
