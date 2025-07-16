//
// Created by TheChuan1503 on 2025/7/16.
//

#ifndef CIRCLORALPHA_BLOCKPOS_H
#define CIRCLORALPHA_BLOCKPOS_H


class BlockPos {
public:
    int x, y, z;

    BlockPos() { x = y = z = 0; }
    BlockPos(int x, int y, int z) : x(x), y(y), z(z) { }
    BlockPos(float, float, float);
};


#endif //CIRCLORALPHA_BLOCKPOS_H
