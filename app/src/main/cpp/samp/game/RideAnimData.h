/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "Enums/AnimationEnums.h"
#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)


class CRideAnimData {
public:
    AssocGroupId m_nAnimGroup{};
    float        m_fSteerAngle{};
    float        m_fAnimLean{};
    int32_t        dwordC{};
    float        dword10{};
    float        m_fHandlebarsAngle{};
    float        m_fAnimPercentageState{};

public:
    CRideAnimData() = default; // 0x6D0B10
    CRideAnimData(AssocGroupId animGroup) : m_nAnimGroup(animGroup) {} // NOTSA
};

VALIDATE_SIZE(CRideAnimData, 0x1C);
