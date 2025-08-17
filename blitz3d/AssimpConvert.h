#ifndef ASSIMPCONVERT_H
#define ASSIMPCONVERT_H

#include "MD_Math.h"
#include <assimp/matrix4x4.h>

static inline MD_Math::MATRIX ConvertMatrix(const aiMatrix4x4& from) {
    MD_Math::MATRIX to;
    to._11 = from.a1; to._12 = from.a2; to._13 = from.a3; to._14 = from.a4;
    to._21 = from.b1; to._22 = from.b2; to._23 = from.b3; to._24 = from.b4;
    to._31 = from.c1; to._32 = from.c2; to._33 = from.c3; to._34 = from.c4;
    to._41 = from.d1; to._42 = from.d2; to._43 = from.d3; to._44 = from.d4;
    return to;
}

#endif