//
// Created by TheChuan1503 on 2025/4/26.
//

#ifndef CIRCLOR2_CIRCLOR_H
#define CIRCLOR2_CIRCLOR_H


class Circlor {
public:
    static void hook(const char *name, void *hook);
    static void hook(const char *name, void *hook, void **original);
    static void hook(long target, void *hook);
    static void hook(long target, void *hook, void **original);

    static double getFunctionValue(const char *path);
    static void setFunctionValue(const char *path, double value);
    static std::string getFunctionStringValue(const char *path);
    static void setFunctionStringValue(const char *path, const char *value);
};


#endif //CIRCLOR2_CIRCLOR_H
