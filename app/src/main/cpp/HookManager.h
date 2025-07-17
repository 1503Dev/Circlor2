//
// Created by TheChuan1503 on 2025/7/16.
//

#ifndef CIRCLORALPHA_HOOKMANAGER_H
#define CIRCLORALPHA_HOOKMANAGER_H

#include "mc/client/ClientInstance.h"

class HookManager {
public:
    static void hook(const char *name, void *hook);
    static void hook(const char *name, void *hook, void **original);
    static void hook(long target, void *hook);
    static void hook(long target, void *hook, void **original);
    static void init();
};


#endif //CIRCLORALPHA_HOOKMANAGER_H
