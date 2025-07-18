//
// Created by TheChuan1503 on 2025/4/26.
//

#ifndef CIRCLOR2_GUIDATA_H
#define CIRCLOR2_GUIDATA_H

#include <string>
class GuiData {
public:
    typedef void (*displayClientMessage_t) (GuiData* guiData, std::string const&);
    typedef void (*showTipMessage_t) (GuiData* guiData, std::string const&);

    void displayClientMessage(std::string const&);
    void showTipMessage(std::string const&); // 显示提示信息(物品栏上方)
};

#endif //CIRCLOR2_GUIDATA_H
