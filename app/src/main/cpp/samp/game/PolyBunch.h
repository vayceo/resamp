/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "common.h"

class CPolyBunch {
public:
    CVector      m_avecPosn[7];
    CPolyBunch*  m_pNext;
    int16        m_wNumVerts; // 100% it's signed
    uint8        m_aU[7]; /// Divide by 200 to get the actual coords
    uint8        m_aV[7]; /// // Divide by 200 to get the actual coords

    auto GetVerts() const {
        return std::span<const CVector>(m_avecPosn, static_cast<size_t>(m_wNumVerts));
    }
};

VALIDATE_SIZE(CPolyBunch, (VER_x32 ? 0x68 : 0x70));
