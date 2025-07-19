//
// Created by TheChuan1503 on 2025/7/19.
//

#include "AABB.h"

#define TAG "Native/AABB"
#include "../../global.h"

void AABB::expand(const Vec3 &vec3) {
    expand_t expand = (expand_t) getMCFuncPtr("AABB::expand");
    if (expand) {
        expand(this, vec3);
    }
}
