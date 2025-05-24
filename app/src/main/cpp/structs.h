//
// Created by TheChuan1503 on 2025/4/28.
//

#ifndef CIRCLOR2_STRUCTS_H
#define CIRCLOR2_STRUCTS_H

struct Vec3 {
    float x;
    float y;
    float z;
    Vec3(float x_, float y_, float z_): x(x_), y(y_), z(z_) {};
    Vec3(): x(0), y(0), z(0) {};
};

struct BlockPos {
    int x;
    int y;
    int z;
    BlockPos(int x_, int y_, int z_): x(x_), y(y_), z(z_) {};
    BlockPos(): x(0), y(0), z(0) {};
};

#endif //CIRCLOR2_STRUCTS_H
