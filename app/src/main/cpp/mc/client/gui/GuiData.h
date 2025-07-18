//
// Created by TheChuan1503 on 2025/4/26.
//

#ifndef CIRCLOR2_GUIDATA_H
#define CIRCLOR2_GUIDATA_H

#include <string>
class GuiData {
public:
    typedef void (*displayClientMessage_t) (GuiData* guiData, std::string const&);
    typedef void (*setGuiScale_t) (GuiData* guiData, float scale);
    typedef void (*setSubtitle_t) (GuiData* guiData, std::string const&);
    typedef void (*setTitle_t) (GuiData* guiData, std::string const&);
    typedef void (*showTipMessage_t) (GuiData* guiData, std::string const&);

    void displayClientMessage(std::string const&);
    void setGuiScale(float scale);
    void setSubtitle(std::string const&);
    void setTitle(std::string const&);
    void showTipMessage(std::string const&); // 显示提示信息(物品栏上方)
};

#endif //CIRCLOR2_GUIDATA_H
