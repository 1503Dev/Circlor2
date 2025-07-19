//
// Created by TheChuan1503 on 2025/7/19.
//

#ifndef CIRCLORALPHA_AABB_H
#define CIRCLORALPHA_AABB_H


#include "Vec3.h"

class AABB {
public:
    typedef void (*expand_t) (AABB*, Vec3 const &);

    void expand(Vec3 const &);
};


#endif //CIRCLORALPHA_AABB_H
