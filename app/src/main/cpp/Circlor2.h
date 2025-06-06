//
// Created by TheChuan1503 on 2025/4/26.
//

#ifndef CIRCLOR2_CIRCLOR2_H
#define CIRCLOR2_CIRCLOR2_H


class Circlor2 {
public:
    static void hook(const char *name, void *hook);
    static void hook(long target, void *hook);
    static void hook(long target, void *hook, void **original);
    static void hookAndOverride(const char *name, void *hook);

    static double getFunctionValue(const char *path);
    static void setFunctionValue(const char *path, double value);
    static std::string getFunctionStringValue(const char *path);
    static void setFunctionStringValue(const char *path, const char *value);
};


#endif //CIRCLOR2_CIRCLOR2_H
